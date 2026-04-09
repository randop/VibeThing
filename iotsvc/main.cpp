#define HAVE_SNI
#include <iostream>
#include <thread>
#include <uv.h>
#include <wolfssl/ssl.h>
#include "ArduinoJson.h"
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

extern "C" {
#include "picohttpparser.h"
}

void server_func();
void client_func();
void on_new_connection(uv_stream_t* server, int status);
void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
void on_read(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf);
void on_write(uv_write_t* req, int status);
void on_close(uv_handle_t* handle);
void on_timer(uv_timer_t* handle);
void do_request(const char* url);

int main() {
    std::cout << "Starting iotsvc" << std::endl;

    wolfSSL_Init();

    std::thread server_thread(server_func);
    std::thread client_thread(client_func);

    server_thread.join();
    client_thread.join();

    wolfSSL_Cleanup();

    std::cout << "iotsvc exiting" << std::endl;
    return 0;
}

void server_func() {
    uv_loop_t loop;
    if (uv_loop_init(&loop) != 0) {
        std::cerr << "Loop init failed" << std::endl;
        return;
    }
    uv_tcp_t server;
    memset(&server, 0, sizeof(uv_tcp_t));
    if (uv_tcp_init(&loop, &server) != 0) {
        std::cerr << "TCP init failed" << std::endl;
        uv_loop_close(&loop);
        return;
    }
    struct sockaddr_in addr;
    uv_ip4_addr("127.0.0.1", 8080, &addr);
    if (uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0) != 0) {
        std::cerr << "Bind failed" << std::endl;
        uv_loop_close(&loop);
        return;
    }
    if (uv_listen((uv_stream_t*)&server, 128, on_new_connection) != 0) {
        std::cerr << "Listen failed" << std::endl;
        uv_loop_close(&loop);
        return;
    }
    std::cout << "Server listening on port 8080" << std::endl;
    uv_run(&loop, UV_RUN_DEFAULT);
    uv_loop_close(&loop);
}

void on_new_connection(uv_stream_t* server, int status) {
    if (status < 0) return;
    uv_tcp_t* client = new uv_tcp_t();
    memset(client, 0, sizeof(uv_tcp_t));
    uv_tcp_init(uv_handle_get_loop((uv_handle_t*)server), client);
    if (uv_accept(server, (uv_stream_t*)client) == 0) {
        uv_read_start((uv_stream_t*)client, alloc_buffer, on_read);
    }
}

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

void on_read(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
    if (nread < 0) {
        uv_close((uv_handle_t*)client, on_close);
        delete[] buf->base;
        return;
    }
    const char* method;
    size_t method_len;
    const char* path;
    size_t path_len;
    int minor_version;
    struct phr_header headers[100];
    size_t num_headers = sizeof(headers)/sizeof(headers[0]);
    const char* body;
    size_t body_len;
    int ret = phr_parse_request(buf->base, nread, &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, 0);
    if (ret <= 0) {
        uv_close((uv_handle_t*)client, on_close);
        delete[] buf->base;
        return;
    }
    body = buf->base + ret;
    body_len = nread - ret;
    ArduinoJson::JsonDocument doc;
    doc["method"] = std::string(method, method_len);
    doc["path"] = std::string(path, path_len);
    doc["body"] = std::string(body, body_len);
    std::string response_body;
    ArduinoJson::serializeJson(doc, response_body);
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: " + std::to_string(response_body.size()) + "\r\n\r\n" + response_body;
    uv_buf_t res_buf = uv_buf_init(const_cast<char*>(response.c_str()), response.size());
    uv_write_t* req = new uv_write_t();
    uv_write(req, client, &res_buf, 1, on_write);
    delete[] buf->base;
}

void on_write(uv_write_t* req, int status) {
    uv_close((uv_handle_t*)req->handle, on_close);
    delete req;
}

void on_close(uv_handle_t* handle) {
    delete handle;
}

void client_func() {
    uv_loop_t loop;
    if (uv_loop_init(&loop) != 0) {
        std::cerr << "Client loop init failed" << std::endl;
        return;
    }
    uv_timer_t timer;
    memset(&timer, 0, sizeof(uv_timer_t));
    if (uv_timer_init(&loop, &timer) != 0) {
        std::cerr << "Timer init failed" << std::endl;
        uv_loop_close(&loop);
        return;
    }
    if (uv_timer_start(&timer, on_timer, 0, 3000) != 0) {
        std::cerr << "Timer start failed" << std::endl;
        uv_loop_close(&loop);
        return;
    }
    std::cout << "Client timer started" << std::endl;
    uv_run(&loop, UV_RUN_DEFAULT);
    uv_loop_close(&loop);
}

void on_timer(uv_timer_t* handle) {
    std::cout << "on_timer called" << std::endl;
    std::thread([]{ do_request("http://httpbin.org/ip"); }).detach();
    std::thread([]{ do_request("https://httpbin.org/ip"); }).detach();
}

void do_request(const char* url) {
    bool is_https = strncmp(url, "https", 5) == 0;
    int port = is_https ? 443 : 80;
    std::cout << "Making request to " << url << std::endl;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, "34.192.25.42", &addr.sin_addr);
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Connect failed" << std::endl;
        close(sock);
        return;
    }
    if (is_https) {
        WOLFSSL_CTX* ctx = wolfSSL_CTX_new(wolfSSLv23_client_method());
        wolfSSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, 0);
        WOLFSSL* ssl = wolfSSL_new(ctx);
        wolfSSL_UseSNI(ssl, 0, "httpbin.org", strlen("httpbin.org"));
        wolfSSL_set_fd(ssl, sock);
        if (wolfSSL_connect(ssl) != SSL_SUCCESS) {
            std::cerr << "SSL connect failed" << std::endl;
            wolfSSL_free(ssl);
            wolfSSL_CTX_free(ctx);
            close(sock);
            return;
        }
        std::string request = "GET /ip HTTP/1.1\r\nHost: httpbin.org\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0.3 Safari/605.1.15\r\nConnection: close\r\n\r\n";
        int written = wolfSSL_write(ssl, request.c_str(), request.size());
        if (written != (int)request.size()) {
            std::cerr << "SSL write failed" << std::endl;
            wolfSSL_free(ssl);
            wolfSSL_CTX_free(ctx);
            close(sock);
            return;
        }
        char buffer[4096] = {0};
        int len = wolfSSL_read(ssl, buffer, sizeof(buffer) - 1);
        if (len > 0) {
            std::string response(buffer, len);
            std::cout << "HTTPS Response: " << response << std::endl;
            size_t pos = response.find("\r\n\r\n");
            if (pos != std::string::npos) {
                std::string body = response.substr(pos + 4);
                ArduinoJson::JsonDocument doc;
                ArduinoJson::deserializeJson(doc, body);
                if (doc["origin"].isNull()) {
                    std::cout << "HTTPS IP: unavailable" << std::endl;
                } else {
                    std::string ip = doc["origin"];
                    std::cout << "HTTPS IP: " << ip << std::endl;
                }
            }
        } else {
            std::cerr << "SSL read failed" << std::endl;
        }
        wolfSSL_free(ssl);
        wolfSSL_CTX_free(ctx);
    } else {
        std::string request = "GET /ip HTTP/1.1\r\nHost: httpbin.org\r\nConnection: close\r\n\r\n";
        send(sock, request.c_str(), request.size(), 0);
        char buffer[4096] = {0};
        int len = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (len > 0) {
            std::string response(buffer, len);
            size_t pos = response.find("\r\n\r\n");
            if (pos != std::string::npos) {
                std::string body = response.substr(pos + 4);
                ArduinoJson::JsonDocument doc;
                ArduinoJson::deserializeJson(doc, body);
                std::string ip = doc["origin"];
                std::cout << "HTTP IP: " << ip << std::endl;
            }
        }
    }
    close(sock);
}
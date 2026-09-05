#define BOOST_ASIO_USE_WOLFSSL
#define ASIO_USE_WOLFSSL
#define OPENSSL_VERSION_NUMBER 0x10101000L
#include <wolfssl/options.h>
#include <wolfssl/ssl.h>

#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/lexical_cast.hpp>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>
#include <thread>
#include <vector>

#include "config.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = net::ip::tcp;

std::string read_file(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    return "Unknown";
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

void https_tls13_get() {
  try {
    const char *host = "www.howsmyssl.com";
    const char *port = "443";
    const char *target = "/a/check";

    net::io_context ioc;

    // Generic TLS client context, then force TLS 1.3 only via WolfSSL API
    ssl::context ctx{ssl::context::tls_client};
    SSL_CTX *native_ctx = ctx.native_handle();
    if (wolfSSL_CTX_SetMinVersion(native_ctx, WOLFSSL_TLSV1_3) !=
        WOLFSSL_SUCCESS) {
      std::cerr << "Failed to set minimum TLS version to 1.3\n";
      return;
    }

    // Barebones: skip certificate verification.
    // For production uncomment the three lines below and remove verify_none.
    // ctx.set_verify_mode(ssl::verify_none);

    // verify:
    ctx.set_default_verify_paths();
    ctx.load_verify_file("/etc/ssl/certs/ca-certificates.crt");
    ctx.set_verify_mode(ssl::verify_peer);

    tcp::resolver resolver{ioc};
    beast::ssl_stream<beast::tcp_stream> stream{ioc, ctx};

    // SNI is required by most modern hosts
    if (!SSL_set_tlsext_host_name(stream.native_handle(), host)) {
      beast::error_code ec{static_cast<int>(::ERR_get_error()),
                           net::error::get_ssl_category()};
      throw beast::system_error{ec};
    }

    auto const results = resolver.resolve(host, port);
    beast::get_lowest_layer(stream).connect(results);

    // TLS handshake (will negotiate only TLS 1.3)
    stream.handshake(ssl::stream_base::client);

    // HTTP GET
    http::request<http::string_body> req{http::verb::get, target, 11};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, APP_USER_AGENT);

    http::write(stream, req);

    beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(stream, buffer, res);

    std::cout << res << std::endl;

    beast::error_code ec;
    stream.shutdown(ec);
    // EOF on shutdown is normal for some servers
    if (ec == net::error::eof)
      ec = {};
    if (ec)
      throw beast::system_error{ec};
  } catch (std::exception const &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void http_httpbin_get() {
  try {
    const char *host = "httpbin.org";
    const char *port = "80";
    const char *target = "/get";

    net::io_context ioc;
    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);

    auto const results = resolver.resolve(host, port);
    stream.connect(results);

    http::request<http::string_body> req{http::verb::get, target, 11};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, APP_USER_AGENT);

    http::write(stream, req);

    beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(stream, buffer, res);

    std::cout << res << std::endl;

    beast::error_code ec;
    stream.socket().shutdown(tcp::socket::shutdown_both, ec);
  } catch (std::exception const &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

int main() {
  char msg_buf[128];
  std::snprintf(msg_buf, sizeof(msg_buf), "Initializing LuckFox v%s (%s) ...",
                PROJECT_VERSION, BUILD_DATE);
  std::cout << msg_buf << std::endl;

  std::string hardware_info = read_file("/sys/firmware/devicetree/base/model");
  std::cout << "Hardware: " << hardware_info << std::endl;

  for (int i = 5; i > 0; --i) {
    std::cout << "Exiting in " << i << "..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  try {
    std::string str = "42";
    int num = boost::lexical_cast<int>(str);
    std::cout << "The number is: " << num << std::endl;

    std::string str2 = "hello";
    double dbl = boost::lexical_cast<double>(str2);
  } catch (const boost::bad_lexical_cast &e) {
    std::cout << "Bad cast: " << e.what() << std::endl;
  }

  http_httpbin_get();
  https_tls13_get();

  return EXIT_SUCCESS;
}

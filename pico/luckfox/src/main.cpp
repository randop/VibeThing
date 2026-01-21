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

std::string read_file(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    return "Unknown";
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
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
  return EXIT_SUCCESS;
}

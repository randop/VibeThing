#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string_view>
#include <thread>

#include "config.h"

int main() {
  char msg_buf[128];
  std::snprintf(msg_buf, sizeof(msg_buf), "Initializing LuckFox v%s (%s) ...",
                PROJECT_VERSION, BUILD_DATE);
  std::cout << msg_buf << std::endl;

  for (int i = 5; i > 0; --i) {
    std::cout << "Exiting in " << i << "..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return EXIT_SUCCESS;
}

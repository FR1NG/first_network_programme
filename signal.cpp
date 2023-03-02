#include <cstdlib>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include  "networking.hpp"

void handle_sig(int signal) {
  (void) signal;
  std::cout << std::endl;
  std::cout << "interupting the server" << std::endl;
  close(sock);
  exit(0);
}

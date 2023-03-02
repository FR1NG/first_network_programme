#include "networking.hpp"
#include <sys/_select.h>
#include <sys/signal.h>
#include <sys/socket.h>

std::string ress() {
  std::string response = "HTTP/1.1 200 OK\r\n"
           "Connection: close\r\n"
           "Content-Type: text/plain\r\n\r\n"
           "Local time is: ";
  // trying to get time of now
  std::time_t now = std::time(NULL);

  char buf[100];
  std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

  response += buf;
  return response;
}

int openSocket() {
  struct addrinfo hints;
  struct addrinfo *bind_address;
  int sock;

  std::cout << "local address configuration " << std::endl;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  if (getaddrinfo(NULL, "9000", &hints, &bind_address)) {
    std::cerr << "getaddrinfo call failed: " << strerror(errno) << std::endl;
    return 1;
  }

  sock = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
  if (sock < 0) {
    std::cerr << "can't open socket " << strerror(errno) << std::endl;
    return 1;
  }
  if (bind(sock, bind_address->ai_addr, bind_address->ai_addrlen)) {
    std::cerr << "can't bind address to socket: " << strerror(errno) << std::endl;
  }
  freeaddrinfo(bind_address);
  return sock;
}

int main() {
  int sock;
  fd_set sockets_set;
  int max_socket;

  signal(SIGINT, handle_sig);
  sock = openSocket();
  FD_ZERO(&sockets_set);
  FD_SET(sock, &sockets_set);
  max_socket = sock;
  if (listen(sock, 10) < 0) {
    std::cerr << "listen call failed: " << strerror(errno) << std::endl;
    return 1;
  }
  std::cout << "waiting for new connections ..." << std::endl;
  
  // here
  while (true) {
    fd_set sets_copy;
    int trigred_socket;
    sets_copy = sockets_set;
    select(max_socket + 1, &sets_copy, NULL, NULL, NULL);
    for (int i = 0; i <= max_socket; i++) {
      if (FD_ISSET(i, &sets_copy)) {
        if (i == sock) {
          trigred_socket = accept(sock, NULL, NULL);
          FD_SET(trigred_socket, &sockets_set);
          std::cout << "connection accepted" << std::endl;
          if (trigred_socket > max_socket)
            max_socket = trigred_socket;
        } else {
          std::cout << "client connected" << std::endl;
          std::cout << "reading request..." << std::endl;

          char request[1024];
          int bytes_recieved = recv(i, request, 1024, 0);
          std::cout << "bytes recieve: " << bytes_recieved << std::endl;
          // std::cout << request << std::endl;

          std::string response = ress();
          std::cout << "trying to send response" << std::endl;
          int bytes_sent =
              send(trigred_socket, response.data(), response.length(), 0);
          std::cout << "server sent " << bytes_sent << " bytes" << std::endl;
          std::cout << "terminating connection" << std::endl;
          close(i);
        }
      }
    }
  }
  close(sock);
  return 0;
}


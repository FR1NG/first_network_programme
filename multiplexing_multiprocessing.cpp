#include "networking.hpp"

int main() {

  struct addrinfo hints;
  struct addrinfo *bind_address;

  std::cout << "local address configuration " << std::endl;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  if (getaddrinfo(NULL, "8080", &hints, &bind_address)) {
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
  if (listen(sock, 10) < 0) {
    std::cerr << "listen call failed: " << strerror(errno) << std::endl;
    return 1;
  }
  std::cout << "waiting for new connections ..." << std::endl;
  
  struct sockaddr_storage client_address;
  socklen_t client_len = sizeof(client_address);
  while (true) {
  int client_sock = accept(sock, (struct sockaddr*)&client_address, &client_len);
  if (client_sock < 0) {
    std::cerr << "accept call failed: " << strerror(errno) << std::endl;
  }
  std::cout << "client connected" << std::endl;
  char address_buffer[100];
  char service_buffer[100];
 if (getnameinfo((struct sockaddr*)&client_address, client_len, address_buffer, sizeof(address_buffer), service_buffer, sizeof(service_buffer), NI_NUMERICHOST)) {
    std::cerr << "getnameinfo call failed: " << strerror(errno) << std::endl;
    return 1;
  }
  std::cout << "client address is: " << address_buffer << " service is: " << service_buffer << std::endl;
  std::cout << "reading request..." << std::endl;
  char request[1024];

    int pid = fork();
    if (pid == 0) {
    std::cout << "on child \n";
      close(sock);
      std::cout << "waiting for recv" << std::endl;
      int bytes_recieved = recv(client_sock, request, 1024, 0);
      if(bytes_recieved < 0)
        exit(0);
      std::cout << "bytes recieve: " << bytes_recieved << std::endl;
      std::cout << request << std::endl;

      std::string response = "HTTP/1.1 200 OK\r\n"
               "Connection: close\r\n"
               "Content-Type: text/plain\r\n\r\n"
               "Local time is: ";
      // trying to get time of now
      std::time_t now = std::time(NULL);

      char buf[100];
      std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

      response += buf;
      int bytes_sent = send(client_sock, response.data(), response.length(), 0);
      std::cout << "server sent " << bytes_sent << " bytes" << std::endl;
      std::cout << "terminating connection" << std::endl;
      close(client_sock);
      exit(0);
        }
    close(client_sock);
    }
  return 0;
}

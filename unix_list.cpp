#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdlib.h>
#include <iostream>


int main ()
{
  struct ifaddrs *addresses;
  if(getifaddrs(&addresses) < 0) {
    std::cout << "getifaddrs call failed" << std::endl;
    return -1;
  }
  while (addresses) {

    int family = addresses->ifa_addr->sa_family;

    if (family == AF_INET || family == AF_INET6) {
      std::cout << addresses->ifa_name << (family == AF_INET ? " ipv4" : " ipv6") << std::endl;
      std::cout << addresses->ifa_addr->sa_data << std::endl;
    }
    addresses = addresses->ifa_next;
  }
  return 0;
}

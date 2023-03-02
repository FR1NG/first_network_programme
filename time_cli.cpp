#include <time.h>
#include <iostream>


int main() {
  time_t timer;
  time(&timer);

  std::cout << ctime(&timer) << std::endl;
  return 0;
}

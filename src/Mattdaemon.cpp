#include "Mattdaemon.hpp"

int main(int argc, char *argv[])
{
  // checl if the user is root
  if (getuid() != 0)
  {
    std::cout << "You must be root to run this program" << '\n';
    return 1;
  }
  std::cout << "hello world from mattdeamon" << '\n';
  return 0;
}
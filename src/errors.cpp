#include "errors.hpp"

void print_error(const char *err, int exit_code)
{
  std::cerr << err << std::endl;
  exit(exit_code);
}

#include "utils.hpp"
  #include <iostream>

int ReturnDigit(const std::string &s)
{
  char *remain = nullptr;
  int nb = strtol(s.c_str(), &remain, 10);
  if (*remain != 0 || nb < 1 || nb > 100)
    return -1;
  return nb;
}

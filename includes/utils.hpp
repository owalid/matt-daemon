#ifndef UTILS_HPP
# define UTILS_HPP

#include <sys/file.h>

#include "tintin_reporter.hpp"

int CreateLockFile(TintinReporter &logger);
void ReleaseLockFile(TintinReporter &logger, int fd_lock);

#endif

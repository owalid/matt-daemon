#ifndef LOCKFILE_HPP
# define LOCKFILE_HPP

#include <sys/file.h>
#include <sys/socket.h>

#include "tintin_reporter.hpp"

int CreateLockFile(TintinReporter &logger);
void ReleaseLockFile(TintinReporter &logger, int fd_lock);

#endif

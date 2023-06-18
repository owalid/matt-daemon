#ifndef DAEMONIZE_HPP
# define DAEMONIZE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

#include "tintin_reporter.hpp"
#include "errors.hpp"

void CheckPid(pid_t pid);
void Daemonize(TintinReporter &logger);

#endif

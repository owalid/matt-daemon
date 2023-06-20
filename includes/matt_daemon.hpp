#ifndef MATT_DAEMON_HPP
# define MATT_DAEMON_HPP

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <csignal>
#include <thread>

#include "proc_connector.hpp"
#include "tintin_reporter.hpp"
#include "server.hpp"
#include "daemonize.hpp"
#include "lockfile.hpp"
#include "utils.hpp"
#include "errors.hpp"
#include "aes_decrypter.hpp"

# define USAGE "Usage : ./Matt_daemon\n[-c/--client MAX_ACCEPTED_CONN]. 1 <= MAX_ACCEPTED_CONN <= 100.\n[-k/--key KEY]. KEY path for aes encryption. [-p/--procevent] Enable linux kernel process event connection."

#endif

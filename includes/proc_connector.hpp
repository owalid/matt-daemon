#ifndef PROC_CONNECTOR_HPP
#define PROC_CONNECTOR_HPP

#include <iostream>
#include <algorithm>
#include <string>

#include <sys/socket.h>
#include <linux/cn_proc.h>
#include <linux/connector.h>
#include <linux/netlink.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "tintin_reporter.hpp"

  int NetlinkConnector();
  void InitEventListener(bool enable_listener, int netlink_socket);
  std::string ReadLink(pid_t pid);
  std::string CmdLine(pid_t pid);

#endif

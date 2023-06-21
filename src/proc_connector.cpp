#include "proc_connector.hpp"

int NetlinkConnector()
{
  struct sockaddr_nl sa_netlink;
  int netlink_socket;
  netlink_socket = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
  if (netlink_socket == -1)
    throw std::runtime_error("Unable to create Dgram socket for netlink connector.");
  sa_netlink.nl_family = AF_NETLINK;
  sa_netlink.nl_groups = CN_IDX_PROC;
  sa_netlink.nl_pid = getpid();
  if (bind(netlink_socket, (struct sockaddr *)&sa_netlink, sizeof(sa_netlink)) == -1)
    throw std::runtime_error("Unable to bind the netlink socket.");
  return netlink_socket;
}

/**
 * @brief This function init the listener. Source of this code : https://bewareofgeek.livejournal.com/2945.html
 * Basically, we send a message to the netlink proc event kernel API to subscribe to proc events.
 * @param enabler_listener // enable listening
 */
void InitEventListener(bool enable_listener, int netlink_socket)
{
  struct __attribute__((aligned(NLMSG_ALIGNTO)))
  {
    struct nlmsghdr nl_hdr;
    struct __attribute__((__packed__))
    {
      struct cn_msg cn_msg;
      enum proc_cn_mcast_op cn_mcast;
    };
  } nlcn_msg;

  memset(&nlcn_msg, 0, sizeof(nlcn_msg));
  nlcn_msg.nl_hdr.nlmsg_len = sizeof(nlcn_msg);
  nlcn_msg.nl_hdr.nlmsg_pid = getpid();
  nlcn_msg.nl_hdr.nlmsg_type = NLMSG_DONE;

  nlcn_msg.cn_msg.id.idx = CN_IDX_PROC;
  nlcn_msg.cn_msg.id.val = CN_VAL_PROC;
  nlcn_msg.cn_msg.len = sizeof(enum proc_cn_mcast_op);

  nlcn_msg.cn_mcast = enable_listener == true ? PROC_CN_MCAST_LISTEN : PROC_CN_MCAST_IGNORE;

  if (send(netlink_socket, &nlcn_msg, sizeof(nlcn_msg), 0) == -1)
    throw std::runtime_error("Unable to subscribe to proc");
}

std::string ReadLink(pid_t pid)
{
  char full_path[1024];
  char buff[1024];
  std::string res;

  snprintf(full_path, sizeof(full_path), "/proc/%d/exe", pid);
  size_t len = readlink(full_path, buff, sizeof(buff));
  if (len == -1)
    res = std::string("N/A");
  else
  {
    buff[len] = '\0';
    res = std::string(buff);
  }
  return res;
}

std::string CmdLine(pid_t pid)
{
  char full_path[1024];
  std::string res("");
  std::ifstream cmd_line_file;

  snprintf(full_path, sizeof(full_path), "/proc/%d/cmdline", pid);
  cmd_line_file.open(full_path);
  if (cmd_line_file.is_open())
  {
    cmd_line_file >> res;
    std::replace(res.begin(), res.end(), '\0', ' ');
    cmd_line_file.close();
  }
  return res;
}

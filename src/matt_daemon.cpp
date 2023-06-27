#include "matt_daemon.hpp"

int SIGNALS[] = { SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGBUS, SIGFPE, SIGKILL, SIGUSR1, SIGSEGV, SIGUSR2, SIGPIPE, SIGALRM, SIGTERM, SIGCHLD, SIGCONT, SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU, SIGURG, SIGXCPU, SIGXFSZ, SIGVTALRM, SIGPROF, SIGWINCH, SIGIO, SIGSYS };
static TintinReporter logger;
int nl_socket;
bool break_loop_thread = false;
bool should_quit = false;

void SignalHandler(int signum)
{
  logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(signalHandler), std::to_string(signum));
  should_quit = true;
}


/* NOTE ADDITIONNELLE SUR UNE CONFUSION POSSIBLE (linux/cn_proc.h)
 * From the user's point of view, the process
 * ID is the thread group ID and thread ID is the internal
 * kernel "pid". So, fields are assigned as follow:
 *
 *  In user space     -  In  kernel space
 *
 * parent process ID  =  parent->tgid
 * parent thread  ID  =  parent->pid
 * child  process ID  =  child->tgid
 * child  thread  ID  =  child->pid
 */
void HandleProcEvent()
{
  struct __attribute__ ((aligned(NLMSG_ALIGNTO)))
  {
    struct nlmsghdr nl_hdr;
    struct __attribute__ ((__packed__)) {
        struct cn_msg cn_msg;
        struct proc_event proc_ev;
    };
  } nlcn_msg;

  logger.MakeNewEvent(logger.GetCategoryFromEnum(procConnector), logger.GetEventFromEnum(procConnectorStarted), "");

  while (break_loop_thread == false)

  {
    int recv_size = recv(nl_socket, &nlcn_msg, sizeof(nlcn_msg), 0);
    if (recv_size == 0)
    {
      logger.MakeNewEvent(logger.GetCategoryFromEnum(procConnector), logger.GetEventFromEnum(procConnectorDisconnected), "");
      return ;
    }
    if (recv_size == -1)
      throw std::runtime_error("recv failed.");
    switch (nlcn_msg.proc_ev.what)
    {
      case nlcn_msg.proc_ev.PROC_EVENT_NONE:
      {
        logger.MakeNewEvent(logger.GetCategoryFromEnum(procConnector), logger.GetEventFromEnum(procEventNone), "");
        break;
      }
      case nlcn_msg.proc_ev.PROC_EVENT_FORK:
      {
        std::string ev_fork("[PARENT TID : " + std::to_string(nlcn_msg.proc_ev.event_data.fork.parent_pid) + " PID : " \
          + std::to_string(nlcn_msg.proc_ev.event_data.fork.parent_tgid) + \
        "][CHILD TID : " + std::to_string(nlcn_msg.proc_ev.event_data.fork.child_pid) + \
        " PID : " + std::to_string(nlcn_msg.proc_ev.event_data.fork.child_tgid) + "]");
        logger.MakeNewEvent(logger.GetCategoryFromEnum(procConnector), logger.GetEventFromEnum(procEventFork), ev_fork);
        break;
      }
      case nlcn_msg.proc_ev.PROC_EVENT_EXEC:
      {
        std::string ev_exec("[FULL COMMAND : " + CmdLine(nlcn_msg.proc_ev.event_data.exec.process_tgid) + " PATH : " + ReadLink(nlcn_msg.proc_ev.event_data.exec.process_tgid) + " PROCESS TID : " + std::to_string(nlcn_msg.proc_ev.event_data.exec.process_pid) + " PID : " + \
        std::to_string(nlcn_msg.proc_ev.event_data.exec.process_tgid) + "]");
        logger.MakeNewEvent(logger.GetCategoryFromEnum(procConnector), logger.GetEventFromEnum(procEventExec), ev_exec);
        break;
      }
      case nlcn_msg.proc_ev.PROC_EVENT_UID:
      {
        std::string ev_uid("[FROM RUID :" + std::to_string(nlcn_msg.proc_ev.event_data.id.r.ruid) + \
          " EUID : " + std::to_string(nlcn_msg.proc_ev.event_data.id.e.euid) + \
          " TO TID : " + std::to_string(nlcn_msg.proc_ev.event_data.id.process_pid) + \
          " PID : " + std::to_string(nlcn_msg.proc_ev.event_data.id.process_tgid) + "]");
        logger.MakeNewEvent(logger.GetCategoryFromEnum(procConnector), logger.GetEventFromEnum(procEventUid), ev_uid);
        break;
      }
      case nlcn_msg.proc_ev.PROC_EVENT_GID :
      {
        std::string ev_gid("[FROM RGID :" + std::to_string(nlcn_msg.proc_ev.event_data.id.r.rgid) + \
          " EGID : " + std::to_string(nlcn_msg.proc_ev.event_data.id.e.egid) + \
          " TO TID : " + std::to_string(nlcn_msg.proc_ev.event_data.id.process_pid) + \
          " PID : " + std::to_string(nlcn_msg.proc_ev.event_data.id.process_tgid) + "]");
        logger.MakeNewEvent(logger.GetCategoryFromEnum(procConnector), logger.GetEventFromEnum(procEventGid), ev_gid);
        break;
      }
      case nlcn_msg.proc_ev.PROC_EVENT_SID :
      {
        std::string ev_sid("[SESSION LEADER PID :" + std::to_string(nlcn_msg.proc_ev.event_data.sid.process_tgid) + \
          " TID : " + std::to_string(nlcn_msg.proc_ev.event_data.sid.process_pid) + "]");
        logger.MakeNewEvent(logger.GetCategoryFromEnum(procConnector), logger.GetEventFromEnum(procEventGid), ev_sid);
        break;
      }
      case nlcn_msg.proc_ev.PROC_EVENT_PTRACE :
      {
        std::string ev_ptrace("[PROCESS PID :" + std::to_string(nlcn_msg.proc_ev.event_data.ptrace.process_tgid) + \
          " TID : " + std::to_string(nlcn_msg.proc_ev.event_data.ptrace.process_pid) + \
          " TRACER PID :" + std::to_string(nlcn_msg.proc_ev.event_data.ptrace.tracer_tgid) + \
          " TRACER TID : " + std::to_string(nlcn_msg.proc_ev.event_data.ptrace.tracer_pid) + "]");
        logger.MakeNewEvent(logger.GetCategoryFromEnum(procConnector), logger.GetEventFromEnum(procEventPtrace), ev_ptrace);
        break;
      }
      case nlcn_msg.proc_ev.PROC_EVENT_COMM :
      {
        std::string ev_comm("[NEW COMMAND NAME : " + std::string(nlcn_msg.proc_ev.event_data.comm.comm) + " PID "  + \
          std::to_string(nlcn_msg.proc_ev.event_data.comm.process_tgid) + \
          " TID : " + std::to_string(nlcn_msg.proc_ev.event_data.comm.process_pid) + "]");
        logger.MakeNewEvent(logger.GetCategoryFromEnum(procConnector), logger.GetEventFromEnum(procEventComm), ev_comm);
        break;
      }
      case nlcn_msg.proc_ev.PROC_EVENT_COREDUMP :
      {
        std::string ev_coredump("[PARENT TID : " + std::to_string(nlcn_msg.proc_ev.event_data.coredump.parent_pid) + " PID : " \
          + std::to_string(nlcn_msg.proc_ev.event_data.coredump.parent_tgid) + \
        "][PROCESS TID : " + std::to_string(nlcn_msg.proc_ev.event_data.coredump.process_pid) + \
        " PID : " + std::to_string(nlcn_msg.proc_ev.event_data.coredump.process_tgid) + "]");
        logger.MakeNewEvent(logger.GetCategoryFromEnum(procConnector), logger.GetEventFromEnum(procEventCoreDump), ev_coredump);
        break;
      }
      case nlcn_msg.proc_ev.PROC_EVENT_EXIT :
      {
        std::string ev_exit("[TID : " + std::to_string(nlcn_msg.proc_ev.event_data.exit.process_pid) + \
          " PID : " + std::to_string(nlcn_msg.proc_ev.event_data.exit.process_tgid) + \
          " EXIT CODE : " + std::to_string(nlcn_msg.proc_ev.event_data.exit.exit_code) + "]");
          nlcn_msg.proc_ev.PROC_EVENT_SID;
        logger.MakeNewEvent(logger.GetCategoryFromEnum(procConnector), logger.GetEventFromEnum(procEventExit), ev_exit);
        break;
      }
      default:
      {
        logger.MakeNewEvent(logger.GetCategoryFromEnum(procConnector), logger.GetEventFromEnum(procEventUnknown), "");
        break;
      }
    }
  }
  logger.MakeNewEvent(logger.GetCategoryFromEnum(procConnector), logger.GetEventFromEnum(procConnectorStopped), "");
}

void ValidateArgs(int argc, char *argv[], int have_args[])
{
  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
    {
      have_args[0] = i;
      return;
    }
    if (strcmp(argv[i], "--client") == 0 || strcmp(argv[i], "-c") == 0)
    {
      if (have_args[0] != -1)
        print_error(USAGE, EXIT_FAILURE);
      have_args[1] = i;
    }
    else if (strcmp(argv[i], "--key") == 0 || strcmp(argv[i], "-k") == 0)
    {
      if (have_args[1] != -1)
        print_error(USAGE, EXIT_FAILURE);
      have_args[2] = i;
    }
    else if (strcmp(argv[i], "--procevent") == 0 || strcmp(argv[i], "-p") == 0)
    {
      have_args[3] = i;
    }
  }
}

int main(int argc, char *argv[])
{
  Server srv;
  AesDecrypter aes;
  char remote_ip[INET6_ADDRSTRLEN];
  char buffer[BUFFER_LEN];
  int new_fd;
  int len_of_received_datas = 0;
  int fd_lockfile = -1;
  int number_of_max_client = 3;
  size_t client_id = 1;
  std::map<int, int> map_of_client_ids;
  bool server_launched = false;
  bool encrypt_mode = false;
  bool proc_event_mode = false;
  bool proc_event_init_launched = false;
  std::thread proc_event_thread;

  // check if the user is root
  if (getuid() != 0)
    print_error("You must be root to run this program.", EXIT_FAILURE);
  if (argc > 4 && argc != 1)
    print_error(USAGE, EXIT_FAILURE);
  if (argc >= 2)
  {
    try {
      int have_args[] = {-1, -1, -1, -1};
      ValidateArgs(argc, argv, have_args);
      if (have_args[0] > 0)
        print_error(USAGE, EXIT_SUCCESS);
      else if (have_args[1] > 0)
      {
        if (argv[have_args[1]+1] == nullptr)
          print_error(USAGE, EXIT_FAILURE);
        std::string s(argv[have_args[1]+1]);
        number_of_max_client = ReturnDigit(s);
        if (number_of_max_client == -1)
          print_error(USAGE, EXIT_FAILURE);
      }
      else if (have_args[2] > 0)
      {
        if (argv[have_args[2]+1] == nullptr)
          print_error(USAGE, EXIT_FAILURE);
        aes.SetKey(argv[have_args[2]+1]);
        encrypt_mode = true;
      }
      else if (have_args[3] > 0)
      {
        proc_event_mode = true;
      }
      else
        print_error(USAGE, EXIT_FAILURE);
    }
    catch (std::exception &e)
    {
      std::cerr << e.what() << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  try
  {
    try
    {
      logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(programStart), "");
    }
    catch (const std::runtime_error &e)
    {
      print_error(e.what(), EXIT_FAILURE);
    }
    fd_lockfile = CreateLockFile(logger);
    Daemonize(logger);
    srv.InitServer();
    srv.SetNumberOfMaxConn(number_of_max_client);
    logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(serverCreated), "");

    for (int i = 0; i < sizeof(SIGNALS)/sizeof(SIGNALS[0]); i++)
      signal(SIGNALS[i], SignalHandler);

    memset(&buffer, 0, BUFFER_LEN);

    if (proc_event_mode == true)
    {
      nl_socket = NetlinkConnector();
      InitEventListener(true, nl_socket);
      proc_event_thread = std::thread(HandleProcEvent);
      proc_event_init_launched = true;
    }

    while (should_quit == false)
    {
      srv.SetReadFd(srv.GetMasterFd());
      if (select(srv.GetMaxFd() + 1, &srv.read_fd_, NULL, NULL, NULL) < 0)
        continue;
      for (int fd = 0; fd <= srv.GetMaxFd(); ++fd)
      {
        //connexion
        if (FD_ISSET(fd, &srv.read_fd_))
        {
          std::string buffer_string;
          //nouvelle connexion
          if (fd == srv.GetListenerFd())
          {
            socklen_t remote_addr_len = sizeof(srv.GetClientAddr());
            new_fd = accept(fd, (struct sockaddr *)&srv.client_addr_, &remote_addr_len);
            if (new_fd == -1)
              throw std::runtime_error("accept() failed.");
            if (srv.GetNumberOfConnectedCli() + 1 > srv.GetNumberOfMaxConn())
            {
              close(new_fd);
              continue;
            }
            FD_SET(new_fd, &srv.master_fd_);
            if (new_fd > srv.GetMaxFd())
              srv.SetMaxFd(new_fd);
            map_of_client_ids.insert(std::pair<int,size_t>(new_fd, client_id++));
            buffer_string.append(inet_ntop(srv.GetClientAddr().ss_family, srv.GetInAddr(), remote_ip , INET6_ADDRSTRLEN));
            buffer_string.append(" [CLIENT_ID " + std::to_string(map_of_client_ids[new_fd]) + "].");
            logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(userConnection), \
            buffer_string);
            srv.SetNumberOfConnectedCli(srv.GetNumberOfConnectedCli() + 1);
          }
          else // on reçoit des données.
          {
            len_of_received_datas = recv(fd, buffer, BUFFER_LEN - 1, 0);
            if (len_of_received_datas <= 0)
            {
              buffer_string.append(std::to_string(map_of_client_ids[fd]) + "].");
              logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(connectionClosed), buffer_string);
              close(fd);
              FD_CLR(fd, &srv.master_fd_);
              srv.SetNumberOfConnectedCli(srv.GetNumberOfConnectedCli() - 1);
            }
            else
            {
              if (encrypt_mode)
              {
                try
                {
                  std::string decryptedtext(aes.DecryptContent(buffer));
                  memset(&buffer, 0, sizeof(buffer));
                  strcpy(buffer, decryptedtext.c_str());
                }
                catch (const std::runtime_error &e)
                {
                  logger.MakeNewEvent(logger.GetCategoryFromEnum(error), logger.GetEventFromEnum(programQuit), " Hard failure : " + std::string(e.what()));
                }
              }
              if (strcmp(buffer, "quit\n") == 0 || strcmp(buffer, "quit\r\n") == 0 || strcmp(buffer, "quit") == 0)
              {
                if (proc_event_init_launched)
                {
                  break_loop_thread = true;
                  proc_event_thread.join();
                  InitEventListener(false, nl_socket);
                  close(nl_socket);
                }
                buffer_string.append(std::to_string(map_of_client_ids[fd]) + "] : " + buffer);
                logger.MakeNewEvent(logger.GetCategoryFromEnum(log), logger.GetEventFromEnum(userRequest), buffer_string);
                logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(programQuit), "");
                memset(&buffer, 0, sizeof(buffer));
                for (const auto &client : map_of_client_ids)
                {
                  FD_CLR(client.first, &srv.master_fd_);
                  close(client.first);
                }
                ReleaseLockFile(logger, fd_lockfile);
                exit(EXIT_SUCCESS);
              }
              else if (strcmp(buffer, "clear\n") == 0 || strcmp(buffer, "clear\r\n") == 0 || strcmp(buffer, "clear") == 0)
              {
                buffer_string.append(std::to_string(map_of_client_ids[fd]) + "] : " + buffer);
                logger.MakeNewEvent(logger.GetCategoryFromEnum(log), logger.GetEventFromEnum(userRequest), buffer_string);
                logger.ClearLogFile();
              }
              else if (strcmp(buffer, "archive\n") == 0 || strcmp(buffer, "archive\r\n") == 0 || strcmp(buffer, "archive") == 0)
              {
                buffer_string.append(std::to_string(map_of_client_ids[fd]) + "] : " + buffer);
                logger.MakeNewEvent(logger.GetCategoryFromEnum(log), logger.GetEventFromEnum(userRequest), buffer_string);
                logger.MakeArchive();
              }
              else if (strcmp(buffer, "procevent start\n") == 0 || strcmp(buffer, "procevent start\r\n") == 0 || strcmp(buffer, "procevent start") == 0)
              {
                buffer_string.append(std::to_string(map_of_client_ids[fd]) + "] : " + buffer);
                logger.MakeNewEvent(logger.GetCategoryFromEnum(log), logger.GetEventFromEnum(userRequest), buffer_string);
                if (proc_event_init_launched == false)
                {
                  nl_socket = NetlinkConnector();
                  InitEventListener(true, nl_socket);
                  proc_event_thread = std::thread(HandleProcEvent);
                  proc_event_init_launched = true;

                }
                else if (proc_event_init_launched == true && break_loop_thread == true)
                {
                  break_loop_thread = false;
                  proc_event_thread = std::thread(HandleProcEvent);
                }
                else
                {
                  logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(userRequestRejected), " PROC CONNECTOR already launched.");
                }
              }
              else if (strcmp(buffer, "procevent stop\n") == 0 || strcmp(buffer, "procevent stop\r\n") == 0 || strcmp(buffer, "procevent stop") == 0)
              {
                buffer_string.append(std::to_string(map_of_client_ids[fd]) + "] : " + buffer);
                logger.MakeNewEvent(logger.GetCategoryFromEnum(log), logger.GetEventFromEnum(userRequest), buffer_string);
                if (proc_event_init_launched == true && break_loop_thread == false)
                {
                  break_loop_thread = true;
                  proc_event_thread.join();
                }
                else
                {
                  logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(userRequestRejected), " PROC CONNECTOR already stopped or not started.");
                }
              }
              else
              {
                buffer_string.append(std::to_string(map_of_client_ids[fd]) + "] : " + buffer);
                logger.MakeNewEvent(logger.GetCategoryFromEnum(log), logger.GetEventFromEnum(userInput), buffer_string);
              }
            }
            buffer_string.clear();
            memset(&buffer, 0, BUFFER_LEN);
          }
        }
      }
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    logger.MakeNewEvent(logger.GetCategoryFromEnum(error), logger.GetEventFromEnum(programQuit), " Hard failure : " + std::string(e.what()));
    if (proc_event_init_launched)
    {
      break_loop_thread = true;
      proc_event_thread.join();
      InitEventListener(false, nl_socket);
      close(nl_socket);
    }
    for (const auto &client : map_of_client_ids)
    {
      FD_CLR(client.first, &srv.master_fd_);
      close(client.first);
    }
    ReleaseLockFile(logger, fd_lockfile);
    exit(EXIT_FAILURE);
  }
  if (proc_event_init_launched)
  {
    break_loop_thread = true;
    proc_event_thread.join();
    InitEventListener(false, nl_socket);
    close(nl_socket);
  }
  for (const auto &client : map_of_client_ids)
  {
    FD_CLR(client.first, &srv.master_fd_);
    close(client.first);
  }
  ReleaseLockFile(logger, fd_lockfile);
  logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(programQuit), "");
  exit(EXIT_SUCCESS);
}

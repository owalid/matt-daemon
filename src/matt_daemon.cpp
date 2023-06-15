#include "matt_daemon.hpp"

int SIGNALS[] = { SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGBUS, SIGFPE, SIGKILL, SIGUSR1, SIGSEGV, SIGUSR2, SIGPIPE, SIGALRM, SIGTERM, SIGCHLD, SIGCONT, SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU, SIGURG, SIGXCPU, SIGXFSZ, SIGVTALRM, SIGPROF, SIGWINCH, SIGIO, SIGSYS };

static TintinReporter logger;

void SignalHandler(int signum)
{
  logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(signalHandler), "");
}

int main(int argc, char *argv[])
{
  Server srv;
  char remote_ip[INET6_ADDRSTRLEN];
  char buffer[BUFFER_LEN];
  int new_fd;
  int len_of_received_datas = 0;
  int fd_lockfile = -1;
  int number_of_max_client = 3;
  size_t client_id = 1;
  std::map<int, int> map_of_client_ids;
  bool server_launched = false;

  // check if the user is root
  if (getuid() != 0)
    print_error("You must be root to run this program.", EXIT_FAILURE);
  if (argc != 3 && argc != 1)
    print_error("Usage : ./Matt_daemon [-c/--client MAX_ACCEPTED_CONN]. 1 <= MAX_ACCEPTED_CONN <= 100.", EXIT_FAILURE);
  if (argc == 3)
  {
    if (strcmp(argv[1], "--client") == 0 ||  strcmp(argv[1], "-c") == 0)
    {
      std::string s(argv[2]);

      number_of_max_client = ReturnDigit(s);
      if (number_of_max_client == -1)
        print_error("Usage : ./Matt_daemon [-c/--client MAX_ACCEPTED_CONN]. 1 <= MAX_ACCEPTED_CONN <= 100.", EXIT_FAILURE);
    }
    else
      print_error("Usage : ./Matt_daemon [-c/--client MAX_ACCEPTED_CONN]. 1 <= MAX_ACCEPTED_CONN <= 100.", EXIT_FAILURE);
  }
  try
  {

    logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(programStart), "");
    fd_lockfile = CreateLockFile(logger);
    Daemonize(logger);
    srv.InitServer();
    srv.SetNumberOfMaxConn(number_of_max_client);
    logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(serverCreated), "");

    for (int i = -1; i < sizeof(SIGNALS)/sizeof(SIGNALS[0]); i++)
      signal(SIGNALS[i], SignalHandler);

    memset(&buffer, 0, BUFFER_LEN);
    for (;;)
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
              if (strcmp(buffer, "quit\n") == 0 || strcmp(buffer, "quit\r\n") == 0 || strcmp(buffer, "quit") == 0)
              {
                buffer_string.append(std::to_string(map_of_client_ids[fd]) + "] : " + buffer);
                logger.MakeNewEvent(logger.GetCategoryFromEnum(log), logger.GetEventFromEnum(userRequest), buffer_string);
                logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(programQuit), "");
                close(fd);
                memset(&buffer, 0, sizeof(buffer));
                FD_CLR(fd, &srv.master_fd_);
                ReleaseLockFile(logger, fd_lockfile);
                exit(0);
              }
              else if (strcmp(buffer, "clear\n") == 0 || strcmp(buffer, "clear\r\n") == 0 || strcmp(buffer, "clear") == 0)
              {
                logger.MakeNewEvent(logger.GetCategoryFromEnum(log), logger.GetEventFromEnum(userRequest), buffer_string);
                logger.ClearLogFile();
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
  catch (const std::runtime_error &e)
  {
    std::cerr << e.what() << std::endl;
    logger.MakeNewEvent(logger.GetCategoryFromEnum(error), logger.GetEventFromEnum(programQuit), " Hard failure : " + std::string(e.what()));
    ReleaseLockFile(logger, fd_lockfile);
    exit(EXIT_FAILURE);
  }
  ReleaseLockFile(logger, fd_lockfile);
  exit(EXIT_SUCCESS);
}

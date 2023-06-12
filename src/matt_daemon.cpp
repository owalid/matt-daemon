#include "matt_daemon.hpp"
#include "tintin_reporter.hpp"
#include "utils.hpp"
#include "server.hpp"

int SIGNALS[] = { SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGBUS, SIGFPE, SIGKILL, SIGUSR1, SIGSEGV, SIGUSR2, SIGPIPE, SIGALRM, SIGTERM, SIGCHLD, SIGCONT, SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU, SIGURG, SIGXCPU, SIGXFSZ, SIGVTALRM, SIGPROF, SIGWINCH, SIGIO, SIGSYS };

static TintinReporter logger;

void CheckPid(pid_t pid)
{
  if (pid < 0)
  {
    exit(EXIT_FAILURE);
  }
  if (pid > 0)
    exit(EXIT_SUCCESS);
}

void Daemonize(TintinReporter &logger)
{
  pid_t pid, sid;

  logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(daemonModeStarted), "");
  pid = fork(); // Parent process will terminated, the child process now runs in the background.
  CheckPid(pid);
  sid = setsid(); // Create a new session.
  if (sid < 0)
    exit(EXIT_FAILURE);

  pid = fork(); // Fork again to ensure that the daemon never reacquires a controlling terminal. (PID != SID)
  CheckPid(pid);

  // Close all open file descriptors
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(serverStartedPid), std::to_string(getpid()));
}

void SignalHandler(int signum) {
  logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(signalHandler), "");
}

int main(int argc, char *argv[])
{
  Server srv;
  bool server_launched = false;
  char remote_ip[INET6_ADDRSTRLEN];
  int new_fd;
  int len_of_received_datas;
  char buffer[4096];
  int fd_lockfile = -1;
  int FAKE_SIGNAL = 0;

  try
  {
    // check if the user is root
    if (getuid() != 0)
    {
      std::cout << "You must be root to run this program" << '\n';
      logger.MakeNewEvent(logger.GetCategoryFromEnum(error), logger.GetEventFromEnum(errorNonRootUser), "");
      logger.MakeNewEvent(logger.GetCategoryFromEnum(error), logger.GetEventFromEnum(programQuit), \
        "Exit : " + std::to_string(EXIT_FAILURE));
      exit(EXIT_FAILURE);
    }
    Daemonize(logger);
    fd_lockfile = CreateLockFile(logger);
    srv.InitServer();
    logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(serverCreated), "");

    for (int i = -1; i < sizeof(SIGNALS)/sizeof(SIGNALS[0]); i++)
      signal(SIGNALS[i], SignalHandler);

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
            logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(userConnection), \
            inet_ntop(srv.GetClientAddr().ss_family, srv.GetInAddr(), remote_ip , INET6_ADDRSTRLEN));
            srv.SetNumberOfConnectedCli(srv.GetNumberOfConnectedCli() + 1);
          }
          else // on reçoit des données.
          {
            len_of_received_datas = recv(fd, buffer, sizeof(buffer), 0);
            if (len_of_received_datas <= 0)
            {
              if (len_of_received_datas == 0)
              {
                logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(connectionClosed), "");
                close(fd);
                FD_CLR(fd, &srv.master_fd_);
              }
              srv.SetNumberOfConnectedCli(srv.GetNumberOfConnectedCli() - 1);
            }
            else
            {
              logger.MakeNewEvent(logger.GetCategoryFromEnum(log), logger.GetEventFromEnum(userRequest), buffer);
              // Ici on devra traiter les données.
              //clear du buffer.
              memset(&buffer, 0, sizeof(buffer));
            }
          }
        }
      }
    }
  }
  catch (const std::runtime_error &e)
  {
    std::cerr << e.what() << std::endl;
    ReleaseLockFile(logger, fd_lockfile);
    exit(EXIT_FAILURE);
  }
  ReleaseLockFile(logger, fd_lockfile);
  exit(EXIT_SUCCESS);
}

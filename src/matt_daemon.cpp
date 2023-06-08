#include "matt_daemon.hpp"
#include "tintin_reporter.hpp"
#include "utils.hpp"

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

int main(int argc, char *argv[])
{
  TintinReporter logger;
  int fd_lockfile = -1;

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
    while (1)
      ;
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

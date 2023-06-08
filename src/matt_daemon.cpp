#include "matt_daemon.hpp"
#include "tintin_reporter.hpp"

void check_pid(pid_t pid)
{
  if (pid < 0)
    exit(1);
  if (pid > 0)
    exit(0);
}

void daemonize(TintinReporter &logger)
{
  pid_t pid, sid;

  logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(daemonModeStarted), "");
  pid = fork(); // Parent process will terminated, the child process now runs in the background.
  check_pid(pid);
  sid = setsid(); // Create a new session.
  if (sid < 0)
    exit(1);

  pid = fork(); // Fork again to ensure that the daemon never reacquires a controlling terminal. (PID != SID)
  check_pid(pid);

  // Close all open file descriptors
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(serverStartedPid), std::to_string(getpid()));
}

int main(int argc, char *argv[])
{
  try
  {
    TintinReporter logger;

    // check if the user is root
    if (getuid() != 0)
    {
      std::cout << "You must be root to run this program" << '\n';
      return 1;
    }
    daemonize(logger);
    while (1)
      ;
  }
  catch (const std::runtime_error &e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}

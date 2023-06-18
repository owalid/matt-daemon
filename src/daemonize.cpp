#include "daemonize.hpp"


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
  try
  {
    logger.MakeNewEvent(logger.GetCategoryFromEnum(info), logger.GetEventFromEnum(daemonModeStarted), "");
  }
  catch (const std::runtime_error &e)
  {
    print_error(e.what(), EXIT_FAILURE);
  }

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

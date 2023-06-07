#include "Mattdaemon.hpp"
#include "Tintin_reporter.hpp"

void check_pid(pid_t pid)
{
  if (pid < 0)
    exit(1);
  if (pid > 0)
    exit(0);
}

void daemonize(Tintin_reporter &logger)
{
  pid_t pid, sid;

  logger.makeNewEvent(logger.getCategoryValueFromEnum(INFO), logger.getEventValueFromEnum(DEAMON_MODE_STARTED), "");
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
  logger.makeNewEvent(logger.getCategoryValueFromEnum(INFO), logger.getEventValueFromEnum(SERVER_STARTED_PID), std::to_string(getpid()));
}

int main(int argc, char *argv[])
{
  Tintin_reporter logger;

  //check if the user is root
  if (getuid() != 0)
  {
    std::cout << "You must be root to run this program" << '\n';
    return 1;
  }
  std::cout << "hello world from mattdeamon" << '\n';
  daemonize(logger);
  while (1)
    ;
  return 0;
}

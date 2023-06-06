#include "Mattdaemon.hpp"

void check_pid(pid_t pid)
{
  if (pid < 0)
    exit(1);
  if (pid > 0)
    exit(0);
}

void daemonize()
{
  pid_t pid, sid;

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
}

int main(int argc, char *argv[])
{
  daemonize();
  while (1)
    ;
  return 0;
}
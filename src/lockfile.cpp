#include "lockfile.hpp"

int CreateLockFile(TintinReporter &logger)
{
  int fd_lock = open(LOCKFILE_PATH, O_RDWR | O_CREAT, 0600);

  std::cout << "fd_lock = " << fd_lock << std::endl;
  if (fd_lock == -1)
    throw std::runtime_error("Unable to create or read the lock file. The program will exit.");
  if (flock(fd_lock, LOCK_EX | LOCK_NB) == -1)
    throw std::runtime_error("Lockfile locked by another process.");
  return fd_lock;
}

void ReleaseLockFile(TintinReporter &logger, int fd_lock)
{
  if (fd_lock == -1)
    return;
  if (flock(fd_lock, LOCK_UN) == -1)
  {
    std::cerr << "Lockfile cannot be unlocked. This is a hard failure." << std::endl;
    logger.MakeNewEvent(logger.GetCategoryFromEnum(error), logger.GetEventFromEnum(errorFileUnlock), "");
    logger.MakeNewEvent(logger.GetCategoryFromEnum(error), logger.GetEventFromEnum(programQuit), \
      " Failure : " + std::string("Lockfile cannot be unlocked. This is a hard failure."));
    std::remove(LOCKFILE_PATH);
    exit(EXIT_FAILURE);
  }
  std::remove(LOCKFILE_PATH);
}

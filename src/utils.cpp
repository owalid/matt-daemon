#include "utils.hpp"
#include "matt_daemon.hpp"

int CreateLockFile(TintinReporter &logger)
{
  int fd_lock = open(LOCKFILE_PATH, O_RDWR | O_CREAT, 0600);

  if (fd_lock == -1)
  {
    std::cerr << "Unable to create or read the lock file. The program will exit." << std::endl;
    exit(EXIT_FAILURE);
  }
  if (flock(fd_lock, LOCK_EX | LOCK_NB) == -1)
  {
    std::cerr << "Lockfile locked by another process." << std::endl;
    logger.MakeNewEvent(logger.GetCategoryFromEnum(error), logger.GetEventFromEnum(errorFileLock), "");
    logger.MakeNewEvent(logger.GetCategoryFromEnum(error), logger.GetEventFromEnum(programQuit), \
      " Failure : " + std::to_string(EXIT_FAILURE));
    exit(EXIT_FAILURE);
  }
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
      " Failure : " + std::to_string(EXIT_FAILURE));
    std::remove(LOCKFILE_PATH);
    exit(EXIT_FAILURE);
  }
  std::remove(LOCKFILE_PATH);
}


void CloseLockFile(TintinReporter &logger, int fd_lock)
{
  if (fd_lock == -1)
    return;
  if (close(fd_lock) == 0)
    std::remove(LOCKFILE_PATH);
}
#include "tintin_reporter.hpp"

// Constructors
TintinReporter::TintinReporter() {}

TintinReporter::TintinReporter(const TintinReporter &copy)
{
  this->event_list_ = copy.getEventList();
}

// Destructor
TintinReporter::~TintinReporter() {}

// Operators
TintinReporter & TintinReporter::operator=(const TintinReporter &assign)
{
  this->event_list_ = assign.getEventList();
  return *this;
}

// Getters / Setters
std::vector<Event> TintinReporter::getEventList() const
{
  return this->event_list_;
}

/**
 * @brief This will create a new event of type Event and add it to the vector and append it to log file.
 *
 * @param cat
 * @param eventValue
 * @param additionalValue
 */
void TintinReporter::MakeNewEvent(const std::string &cat, const std::string &eventValue, const std::string &additionalValue)
{
  Event new_event(CreateTimestamp(), cat, eventValue + additionalValue);

  this->AddEventToList(new_event);
  this->AddEventToLogFile(new_event);
}

void TintinReporter::AddEventToList(Event &ev)
{
  this->event_list_.push_back(ev);
}

/**
 * @brief This function add the event to the log file.
 *
 * @param ev
 */
void TintinReporter::AddEventToLogFile(Event &ev)
{
  std::lock_guard<std::mutex> lock(this->log_mutex_);

  if (std::filesystem::exists(LOG_DIRECTORY) == false)
    std::filesystem::create_directory(LOG_DIRECTORY);
  if (std::filesystem::exists(LOG_DIRECTORY) == false)
    throw std::runtime_error("Could not create the directory /var/log/matt_daemon.");
  std::ofstream logfile(LOG_PATH, std::ios_base::app);
  if (logfile.is_open())
  {
    logfile << ev.MakeEventString();
    logfile.close();
  }
  else
    throw std::runtime_error("Could not create the file matt_daemon.log.");
}

/**
 * @brief This function create a timestamp.
 *
 * @return std::string
 */
std::string TintinReporter::CreateTimestamp()
{
  auto time = std::chrono::system_clock::now();
  std::time_t timeInSecond = std::chrono::system_clock::to_time_t(time);
  std::tm *timeStruct = std::localtime(&timeInSecond);
  std::string time_string;

  time_string.append("[");
  if (timeStruct->tm_mday < 10)
    time_string.append("0");
  time_string.append(std::to_string(timeStruct->tm_mday) + "/");
  if (timeStruct->tm_mon < 10)
    time_string.append("0");
  time_string.append(std::to_string(timeStruct->tm_mon + 1) + "/");
  time_string.append(std::to_string(timeStruct->tm_year + 1900) + "-");
  if (timeStruct->tm_hour < 10)
    time_string.append("0");
  time_string.append(std::to_string(timeStruct->tm_hour) + ":");
  if (timeStruct->tm_min < 10)
    time_string.append("0");
  time_string.append(std::to_string(timeStruct->tm_min) + ":");
  if (timeStruct->tm_sec < 10)
    time_string.append("0");
  time_string.append(std::to_string(timeStruct->tm_sec) + "]");
  return time_string;
}

std::string TintinReporter::CreateTimestampForNameFile()
{
  auto time = std::chrono::system_clock::now();
  std::time_t timeInSecond = std::chrono::system_clock::to_time_t(time);
  std::tm *timeStruct = std::localtime(&timeInSecond);
  std::string time_string;

  if (timeStruct->tm_mday < 10)
    time_string.append("0");
  time_string.append(std::to_string(timeStruct->tm_mday) + "_");
  if (timeStruct->tm_mon < 10)
    time_string.append("0");
  time_string.append(std::to_string(timeStruct->tm_mon + 1) + "_");
  time_string.append(std::to_string(timeStruct->tm_year + 1900) + "_");
  if (timeStruct->tm_hour < 10)
    time_string.append("0");
  time_string.append(std::to_string(timeStruct->tm_hour) + "-");
  if (timeStruct->tm_min < 10)
    time_string.append("0");
  time_string.append(std::to_string(timeStruct->tm_min) + "-");
  if (timeStruct->tm_sec < 10)
    time_string.append("0");
  time_string.append(std::to_string(timeStruct->tm_sec));
  return time_string;
}

/**
 * @brief This function return the string corresponding to the event enum.
 *
 * @param ev
 * @return std::string
 */
std::string TintinReporter::GetEventFromEnum(const EventEnum &ev)
{
  switch (ev)
  {
    case programStart:
      return "Start.";
      break;
    case programQuit:
      return "Quitting.";
    case serverCreated:
      return "Server created";
      break;
    case daemonModeStarted:
      return "Entering daemon mode.";
      break;
    case serverStartedPid:
      return "Started. PID : ";
      break;
    case signalHandler:
      return "Signal handler.";
      break;
    case errorFileLock:
      return "Cannot lock the lockfile.";
      break;
    case errorFileUnlock:
      return "cannot unlock the lockfile.";
      break;
    case errorNonRootUser:
      return "Non root user launched the program.";
      break;
    case userConnection:
      return "A new user is connected from : ";
      break;
    case userInput:
      return "Input from [CLIENT_ID ";
      break;
    case userRequest:
      return "Request from [CLIENT_ID ";
      break;
    case userRequestRejected:
      return "The user request is rejected : ";
      break;
    case archiveCreated:
      return "Archive created : ";
      break;
    case errorArchive:
      return "The archive could not be created.";
      break;
    case connectionClosed:
      return "Connection closed from [CLIENT_ID ";
      break;
    case procConnectorStarted:
      return "Proc connector started.";
      break;
    case procConnectorStopped:
      return "Proc connector stopped.";
      break;
    case procConnectorDisconnected:
      return "Proc connector disconnected.";
      break;
    case procEventFork:
      return "FORK Event : ";
      break;
    case procEventExec:
      return "EXEC Event : ";
      break;
    case procEventExit:
      return "EXIT Event : ";
      break;
    case procEventGid :
      return "GID Event : ";
      break;
    case procEventUid :
      return "UID Event : ";
      break;
    case procEventSid:
      return "SID Event : ";
      break;
    case procEventPtrace :
      return "PTRACE Event : ";
      break;
    case procEventComm :
      return "COMM Event : ";
      break;
    case procEventCoreDump:
      return "COREDUMP Event : ";
      break;
    case procEventNone :
      return "EMPTY or NULL Event.";
      break;
    case procEventUnknown:
      return "UNKNOWN Event.";
      break;
    default:
      return "UNKNOWN event.";
      break;
  }
}

/**
 * @brief This function return the category value corresponding to the category enum.
 *
 * @param catEnum
 * @return std::string
 */
std::string TintinReporter::GetCategoryFromEnum(const CategoryEnum &cat)
{
  switch (cat)
  {
    case info:
      return "[ INFO ]";
      break;
    case log:
      return "[ LOG ]";
      break;
    case error:
      return "[ ERROR ]";
      break;
    case procConnector:
      return "[ PROC CONNECTOR ]";
      break;
    default:
      return "[ UNKNOWN ]";
      break;
  }
}

/**
 * @brief This function will clear the log file.
 *
 * @return void
*/
void TintinReporter::ClearLogFile()
{
  std::ofstream logfile;
  logfile.open(LOG_PATH, std::ofstream::out | std::ofstream::trunc);
  if (logfile.is_open())
    logfile.close();
}

void TintinReporter::MakeArchive()
{
  std::string path_name(LOG_DIRECTORY);
  path_name.append("/" + this->CreateTimestampForNameFile() + "-matt_daemon_log.tar.gz");
  std::string full_command("tar -czvf " + path_name + " " + LOG_PATH);
  std::system(full_command.c_str());

  if (std::filesystem::exists(path_name) == true)
    this->MakeNewEvent(this->GetCategoryFromEnum(info), this->GetEventFromEnum(archiveCreated), path_name);
  else
    this->MakeNewEvent(this->GetCategoryFromEnum(error), this->GetEventFromEnum(errorArchive), "");;
}

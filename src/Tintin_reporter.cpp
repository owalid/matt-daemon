#include "Tintin_reporter.hpp"

// Constructors
Tintin_reporter::Tintin_reporter()
{
	this->makeNewEvent(this->getCategoryValueFromEnum(INFO), this->getEventValueFromEnum(PROGRAM_START), "");
	std::cout << "\e[0;33mDefault Constructor called of Tintin_reporter\e[0m" << std::endl;
}

Tintin_reporter::Tintin_reporter(const Tintin_reporter &copy)
{
	_eventList = copy.getEventList();
	std::cout << "\e[0;33mCopy Constructor called of Tintin_reporter\e[0m" << std::endl;
}

// Destructor
Tintin_reporter::~Tintin_reporter()
{
	this->makeNewEvent(this->getCategoryValueFromEnum(INFO), this->getEventValueFromEnum(PROGRAM_QUIT), "");
	std::cout << "\e[0;31mDestructor called of Tintin_reporter\e[0m" << std::endl;
}


// Operators
Tintin_reporter & Tintin_reporter::operator=(const Tintin_reporter &assign)
{
	_eventList = assign.getEventList();
	return *this;
}


// Getters / Setters
std::vector<Event> Tintin_reporter::getEventList() const
{
	return this->_eventList;
}

/**
 * @brief This will create a new event of type Event and add it to the vector and append it to log file.
 *
 * @param cat
 * @param eventValue
 * @param additionalValue
 */
void Tintin_reporter::makeNewEvent(std::string cat, std::string eventValue, std::string additionalValue = "")
{
	Event newEvent(createTimestamp(), cat, eventValue.append(additionalValue));
	this->addEventToList(newEvent);
	this->addEventToLogFile(newEvent);
}


/**
 * @brief This function add the event to the vector.
 *
 * @param ev
 */
void Tintin_reporter::addEventToList(Event &ev)
{
	this->_eventList.push_back(ev);
}

/**
 * @brief This function add the event to the log file.
 *
 * @param ev
 */
void Tintin_reporter::addEventToLogFile(Event &ev)
{
	std::ofstream logfile(LOG_PATH, std::ios_base::app);
	if (logfile.is_open())
	{
		logfile << ev.makeEventString();
		logfile.close();
	}
	else
	{
		std::cerr << "cannot open the file." << std::endl;
	}
}

/**
 * @brief This function create a timestamp.
 *
 * @return std::string
 */
std::string Tintin_reporter::createTimestamp()
{
	auto time = std::chrono::system_clock::now();
	std::time_t timeInSecond = std::chrono::system_clock::to_time_t(time);
	std::tm *timeStruct = std::localtime(&timeInSecond);
	std::string timeString;

	timeString.append("[" \
	+ std::to_string(timeStruct->tm_mday) + "/" \
	+ std::to_string(timeStruct->tm_mon + 1) + "/" \
	+ std::to_string(timeStruct->tm_year + 1900) + "-" \
	+ std::to_string(timeStruct->tm_hour) + ":" \
	+ std::to_string(timeStruct->tm_min) + ":" \
	+ std::to_string(timeStruct->tm_sec) + "]");
	return timeString;
}

/**
 * @brief This function return the string corresponding to the event enum.
 *
 * @param ev
 * @return std::string
 */
std::string Tintin_reporter::getEventValueFromEnum(const EventEnum &ev)
{
	switch (ev)
	{
	case PROGRAM_START:
		return "Start.";
		break;
	case PROGRAM_QUIT:
		return "Quit.";
	case SERVER_CREATED:
		return "Server created";
		break;
	case DEAMON_MODE_STARTED:
		return "Entering deamon mode.";
		break;
	case SERVER_STARTED_PID:
		return "Started. PID : ";
		break;
	case SIGNAL_HANDLER:
		return "Signal handler.";
		break;
	case ERROR_FILE_LOCK:
		return "Error file lock.";
		break;
	case USER_INPUT:
		return "User input : ";
		break;
	case USER_REQUEST:
		return "User request : ";
		break;
	default:
		return "Unknown event.";
		break;
	}
}

/**
 * @brief This function return the category value corresponding to the category enum.
 *
 * @param catEnum
 * @return std::string
 */
std::string Tintin_reporter::getCategoryValueFromEnum(const CategoryEnum &catEnum)
{
	switch (catEnum)
	{
	case INFO:
		return "[ INFO ]";
		break;
	case LOG:
		return "[ LOG ]";
		break;
	case ERROR:
		return "[ ERROR ]";
		break;
	default:
		return "[ UNKNOWN ]";
		break;
	}
}

#ifndef TINTIN_REPORTER_HPP
# define TINTIN_REPORTER_HPP

# include <iostream>
# include <fstream>
# include <filesystem>
# include <string>
# include <vector>
# include <map>
# include <chrono>
# include "Event.hpp"
# include "EventEnum.hpp"
# include "CategoryEnum.hpp"
# define LOG_DIRECTORY "/var/log/matt_daemon"
# define LOG_PATH "/var/log/matt_daemon/matt_daemon.log"

class Tintin_reporter
{
	public:
		// Constructors
		Tintin_reporter();
		Tintin_reporter(const Tintin_reporter &copy);

		// Destructor
		~Tintin_reporter();

		// Operators
		Tintin_reporter & operator=(const Tintin_reporter &assign);

		// Getters / Setters
		std::vector<Event> getEventList() const;

		// fonctions membres
		void makeNewEvent(std::string cat, std::string eventValue, std::string additionalValue);
		std::string getCategoryValueFromEnum(const CategoryEnum &catEnum);
		std::string getEventValueFromEnum(const EventEnum &eventEnum);
	private:
		void addEventToList(Event &ev);
		void addEventToLogFile(Event &ev);
		std::string createTimestamp();

		std::vector<Event> _eventList;
};

#endif

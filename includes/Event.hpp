#ifndef EVENT_HPP
# define EVENT_HPP

# include <iostream>
# include <string>
# include "EventEnum.hpp"
class Event
{
	public:
		// Constructors
		Event();
		Event(const Event &copy);
		Event(std::string eventTimestamp, std::string eventCat, std::string eventValue);

		// Destructor
		~Event();

		// Operators
		Event & operator=(const Event &assign);

		// Getters / Setters
		std::string getEventTimestamp() const;
		std::string getEventCat() const;
		std::string getEventValue() const;

		// fonctions membres
		std::string makeEventString();

	private:
		std::string _eventTimestamp;
		std::string _eventCat;
		std::string _eventValue;

};

#endif

#include "Event.hpp"

// Constructors
Event::Event()
{
	_eventTimestamp = "";
	_eventCat = "";
	_eventValue = "";
	std::cout << "\e[0;33mDefault Constructor called of Event\e[0m" << std::endl;
}

Event::Event(const Event &copy)
{
	_eventTimestamp = copy.getEventTimestamp();
	_eventCat = copy.getEventCat();
	_eventValue = copy.getEventValue();
	std::cout << "\e[0;33mCopy Constructor called of Event\e[0m" << std::endl;
}

Event::Event(std::string eventTimestamp, std::string eventType, std::string eventValue)
{
	this->_eventTimestamp = eventTimestamp;
	this->_eventCat = eventType;
	this->_eventValue = eventValue;
}

// Destructor
Event::~Event()
{
	std::cout << "\e[0;31mDestructor called of Event\e[0m" << std::endl;
}


// Operators
Event & Event::operator=(const Event &assign)
{
	_eventTimestamp = assign.getEventTimestamp();
	_eventCat = assign.getEventCat();
	_eventValue = assign.getEventValue();
	return *this;
}


// Getters / Setters
std::string Event::getEventTimestamp() const
{
	return _eventTimestamp;
}
std::string Event::getEventCat() const
{
	return _eventCat;
}
std::string Event::getEventValue() const
{
	return _eventValue;
}

std::string Event::makeEventString()
{
	return this->getEventTimestamp() + " " + this->getEventCat() + " - Matt_daemon : " + this->getEventValue() + "\n";
}

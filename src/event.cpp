#include "event.hpp"

// Constructors
Event::Event()
{
  event_timestamp_ = "";
  event_category_ = "";
  event_description_ = "";
}

Event::Event(const Event &copy)
{
  event_timestamp_ = copy.GetEventTimestamp();
  event_category_ = copy.GetEventCategory();
  event_description_ = copy.GetEventDescription();
}

Event::Event(std::string event_timestamp, std::string event_category, std::string event_description)
{
  this->event_timestamp_ = event_timestamp;
  this->event_category_ = event_category;
  this->event_description_ = event_description;
}

// Destructor
Event::~Event() {}

// Operators
Event &Event::operator=(const Event &assign)
{
  event_timestamp_ = assign.GetEventTimestamp();
  event_category_ = assign.GetEventCategory();
  event_description_ = assign.GetEventDescription();
  return *this;
}

// Getters / Setters
std::string Event::GetEventTimestamp() const
{
  return event_timestamp_;
}

std::string Event::GetEventCategory() const
{
  return event_category_;
}

std::string Event::GetEventDescription() const
{
  return event_description_;
}

//member functions
std::string Event::MakeEventString()
{
  return this->GetEventTimestamp() + " " + this->GetEventCategory() + " - Matt_daemon : " + this->GetEventDescription() + "\n";
}

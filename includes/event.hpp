#ifndef EVENT_HPP
# define EVENT_HPP

#include <iostream>
#include <string>

#include "event_enum.hpp"

class Event
{
  public:
    // Constructors
    Event();
    Event(const Event &copy);
    Event(std::string event_timestamp, std::string event_category, std::string event_description);

    // Destructor
    ~Event();

    // Operators
    Event &operator=(const Event &assign);

    // Getters / Setters
    std::string GetEventTimestamp() const;
    std::string GetEventCategory() const;
    std::string GetEventDescription() const;

    // fonctions membres
    std::string MakeEventString();

  private:
    std::string event_timestamp_;
    std::string event_category_;
    std::string event_description_;
};

#endif

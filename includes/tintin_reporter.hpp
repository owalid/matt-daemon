#ifndef TINTIN_REPORTER_HPP
# define TINTIN_REPORTER_HPP

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <mutex>

#include "event.hpp"
#include "event_enum.hpp"
#include "category_enum.hpp"
#include "macros.hpp"

class TintinReporter
{
  public:
    // Constructors
    TintinReporter();
    TintinReporter(const TintinReporter &copy);

    // Destructor
    ~TintinReporter();

    // Operators
    TintinReporter &operator=(const TintinReporter &assign);

    // Getters / Setters
    std::vector<Event> getEventList() const;

    // fonctions membres
    void MakeNewEvent(const std::string &cat, const std::string &eventValue, const std::string &additionalValue);
    void ClearLogFile();
    void MakeArchive(bool &make_archive);
    std::mutex GetLogMutex() const;
    std::string GetCategoryFromEnum(const CategoryEnum &catEnum);
    std::string GetEventFromEnum(const EventEnum &eventEnum);
    std::mutex log_mutex_;
  private:
    void AddEventToList(Event &ev);
    void AddEventToLogFile(Event &ev);
    std::string CreateTimestamp();
    std::string CreateTimestampForNameFile();
    std::vector<Event> event_list_;
};

#endif

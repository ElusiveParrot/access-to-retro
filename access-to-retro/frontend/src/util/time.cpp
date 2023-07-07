#include <regex>
#include <ctime>
#include <cmath>
#include "time.hpp"

/****************************************************************************************************
 time_duration
****************************************************************************************************/

ar::util::time_duration::time_duration(unsigned seconds) :
        _seconds(seconds)
{

}

ar::util::time_duration::time_duration(unsigned hours, unsigned minutes, unsigned seconds)
{
    _seconds = seconds;

    _seconds += minutes * 60;
    _seconds += hours * 60 * 60;
}

unsigned ar::util::time_duration::get_total_hours(bool ceil_instead_of_floor) const
{
    double hours = static_cast<double>(_seconds) / 60.0 / 60.0;

    return static_cast<unsigned>(ceil_instead_of_floor ? ceil(hours) : floor(hours));
}

unsigned ar::util::time_duration::get_total_minutes(bool ceil_instead_of_floor) const
{
    double minutes = static_cast<double>(_seconds) / 60.0;

    return static_cast<unsigned>(ceil_instead_of_floor ? ceil(minutes) : floor(minutes));
}

unsigned ar::util::time_duration::get_total_seconds() const
{
    return _seconds;
}

/****************************************************************************************************
 time_and_date
****************************************************************************************************/

ar::util::time_and_date::time_and_date(unsigned year, unsigned month, unsigned day, unsigned hour,
                                       unsigned minute, unsigned second) :
        _year(year),
        _month(month),
        _day(day),
        _hour(hour),
        _minute(minute),
        _second(second)
{
    normalise();
}

ar::util::time_and_date ar::util::time_and_date::from_date(unsigned year, unsigned month, unsigned day)
{
    return { year, month, day, DEFAULT_HOUR, DEFAULT_MINUTE, DEFAULT_SECOND };
}

ar::util::time_and_date ar::util::time_and_date::from_time(unsigned hour, unsigned minute, unsigned second)
{
    return { DEFAULT_YEAR, DEFAULT_MONTH, DEFAULT_DAY, hour, minute, second };
}

ar::util::time_and_date ar::util::time_and_date::now()
{
    time_t now = time(nullptr);

    tm local_time {};

#ifdef WIN32
    localtime_s(&local_time, &now);
#else
    localtime_r(&now, &local_time);
#endif

    // Numbers added due to how local time work in STD Library
    return { 1900 + static_cast<unsigned>(local_time.tm_year), 1 + static_cast<unsigned>(local_time.tm_mon),
             static_cast<unsigned>(local_time.tm_mday), static_cast<unsigned int>(local_time.tm_hour),
             static_cast<unsigned int>(local_time.tm_min), static_cast<unsigned>(local_time.tm_sec) };
}

std::string ar::util::time_and_date::as_string(const std::string& format) const
{
    std::string dt_string = format;

    dt_string = std::regex_replace(dt_string, std::regex("Y"), std::to_string(_year));
    dt_string = std::regex_replace(dt_string, std::regex("M"), std::to_string(_month));
    dt_string = std::regex_replace(dt_string, std::regex("D"), std::to_string(_day));

    dt_string = std::regex_replace(dt_string, std::regex("h"), std::to_string(_hour));
    dt_string = std::regex_replace(dt_string, std::regex("m"), std::to_string(_minute));
    dt_string = std::regex_replace(dt_string, std::regex("s"), std::to_string(_second));

    return dt_string;
}

void ar::util::time_and_date::normalise()
{
    if (_month == 0 || _month > 12)
    {
        _month = DEFAULT_MONTH;
    }

    if (_day == 0)
    {
        _day = DEFAULT_DAY;
    }

    unsigned last_day_of_month = LAST_DAY_OF_MONTH[_month - 1];

    // February edge case
    if (_month == 2)
    {
        // Leap year = february 29 days
        if (_year % 4 == 0)
        {
            last_day_of_month += 1;
        }
    }

    if (_day > last_day_of_month)
    {
        _day = DEFAULT_DAY;
    }

    if (_hour >= 24)
    {
        _hour = DEFAULT_HOUR;
    }

    if (_minute >= 60)
    {
        _minute = DEFAULT_MINUTE;
    }

    if (_second >= 60)
    {
        _second = DEFAULT_SECOND;
    }
}

unsigned ar::util::time_and_date::get_year() const
{
    return _year;
}

unsigned ar::util::time_and_date::get_month() const
{
    return _month;
}

unsigned ar::util::time_and_date::get_day() const
{
    return _day;
}

unsigned ar::util::time_and_date::get_hour() const
{
    return _hour;
}

unsigned ar::util::time_and_date::get_minute() const
{
    return _minute;
}

unsigned ar::util::time_and_date::get_second() const
{
    return _second;
}

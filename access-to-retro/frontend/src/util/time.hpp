/**
 * @file util/time.hpp
 */

#ifndef ACCESS_TO_RETRO_TIME_HPP
#define ACCESS_TO_RETRO_TIME_HPP

#include <string>
#include <array>

namespace ar::util
{
    /// @brief Represents a duration of time, not to be confused with date/time.
    class time_duration
    {
    public:
        explicit time_duration(unsigned seconds);
        time_duration(unsigned hours, unsigned minutes, unsigned seconds);

        /**
         * @brief Returns the time duration in hours
         * @param ceil_instead_of_floor Uses ceil() function instead of floor() on the value
         * @return Duration in hours
         */
        [[nodiscard]] unsigned get_total_hours(bool ceil_instead_of_floor) const;

        /**
         * @brief Returns the time duration in minutes
         * @param ceil_instead_of_floor Uses ceil() function instead of floor() on the value
         * @return Duration in minutes
         */
        [[nodiscard]] unsigned get_total_minutes(bool ceil_instead_of_floor) const;

        [[nodiscard]] unsigned get_total_seconds() const;

    private:
        unsigned _seconds = 0;
    };


    /// @brief Represents both time and date, not to be confused with time duration
    class time_and_date
    {
    public:
        /**
         * @details Values will be normalised, if the value is incorrect the default (from unix epoch) will be chosen
         */
        time_and_date(unsigned year, unsigned month, unsigned day, unsigned hour,
                      unsigned minute, unsigned second);

        /**
         * @brief Constructs time and date object based on values provided, rest is filled in with unix epoch values
         * @details Values will be normalised, if the value is incorrect the default (from unix epoch) will be chosen
         * @return time_and_date object with normalised values
         */
        static time_and_date from_date(unsigned year, unsigned month, unsigned day);

        /**
         * @brief Constructs time and date object based on values provided, rest is filled in with unix epoch values
         * @details Values will be normalised, if the value is incorrect the default (from unix epoch) will be chosen
         * @return time_and_date object with normalised values
         */
        static time_and_date from_time(unsigned hour, unsigned minute, unsigned second);

        /**
         * @brief Constructs time and date object based on current time and date.
         * @return time_and_date object with normalised values
         */
        static time_and_date now();

        /**
         * @brief Returns a string in provided format based on the values in the object
         * @details Format can only contain Y, M, D, h, m, s. Note the upper and lower case letters.
         * @param format Optional string in which the string is to be returned, for example: "Y-M-D h:m:s"
         * @return String with time values filled in
         */
        [[nodiscard]] std::string as_string(const std::string& format = "D/M/Y h:m:s") const;

        [[nodiscard]] unsigned get_year() const;
        [[nodiscard]] unsigned get_month() const;
        [[nodiscard]] unsigned get_day() const;
        [[nodiscard]] unsigned get_hour() const;
        [[nodiscard]] unsigned get_minute() const;
        [[nodiscard]] unsigned get_second() const;

    private:
        // Default = Unix Epoch
        static constexpr unsigned DEFAULT_YEAR  = 1970;
        static constexpr unsigned DEFAULT_MONTH = 1;
        static constexpr unsigned DEFAULT_DAY   = 1;

        static constexpr unsigned DEFAULT_HOUR   = 0;
        static constexpr unsigned DEFAULT_MINUTE = 0;
        static constexpr unsigned DEFAULT_SECOND = 0;

        /// @brief Stores the number of days in given month (index)
        static constexpr std::array<unsigned, 12> LAST_DAY_OF_MONTH = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30,
                                                                        31 };

        unsigned _year;
        unsigned _month;
        unsigned _day;

        unsigned _hour;
        unsigned _minute;
        unsigned _second;

        /// @brief Normalises values
        void normalise();
    };
}

#endif //ACCESS_TO_RETRO_TIME_HPP

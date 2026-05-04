#include "../include/Date.h"


/**********************************************************************************************************/


// formates the date in DD/MM/YYYY format
std::string Date::formate_date(std::time_t time)
{
    std::tm *local_time = std::localtime(&time);

    char date[11];
    std::strftime(date, sizeof(date), "%d/%m/%Y", local_time);

    return std::string(date);
}


/**********************************************************************************************************/


// returns the today's date in DD/MM/YYYY format
std::string Date::get_current_date()
{
    std::time_t now = std::time(nullptr);
    return formate_date(now);
}


/**********************************************************************************************************/


// returns the future date by adding 'n' days to today's date
std::string Date::get_future_date(int days)
{
    std::time_t now = std::time(nullptr);
    now += days * SECONDS_PER_DAY;
    return formate_date(now);
}


/**********************************************************************************************************/


// return the days difference between returned date and due date
int Date::get_days_between(const std::string& due_date, const std::string& return_date)
{
    int d1 = 0, m1 = 0, y1 = 0;
    int d2 = 0, m2 = 0, y2 = 0;

    sscanf(due_date.c_str(), "%d/%d/%d", &d1, &m1, &y1);
    sscanf(return_date.c_str(), "%d/%d/%d", &d2, &m2, &y2);

    std::tm tm1 = {};
    std::tm tm2 = {};

    tm1.tm_mday = d1;
    tm1.tm_mon = m1 - 1;
    tm1.tm_year = y1 - 1900;

    tm2.tm_mday = d2;
    tm2.tm_mon = m2 - 1;
    tm2.tm_year = y2 - 1900;

    std::time_t t1 = std::mktime(&tm1);
    std::time_t t2 = std::mktime(&tm2);

    double seconds = std::difftime(t2, t1);
    return seconds / SECONDS_PER_DAY;
}


/**********************************************************************************************************/
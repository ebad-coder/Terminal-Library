#pragma once

#include <string>
#include <ctime>

class Date {
    private:
        static constexpr int SECONDS_PER_DAY = 86400;
        static std::string formate_date(std::time_t time);
        
    public: 
        static std::string get_current_date();
        static std::string get_future_date(int n);
        static int get_days_between(const std::string& due_date, const std::string& return_date);
};
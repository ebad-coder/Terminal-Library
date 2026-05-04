#pragma once

#include <string>

class Transaction {
    private:
        std::string username;
        std::string resource_id;
        std::string issue_date;
        std::string due_date;
        std::string returned_date;
        bool active;

    public:
        Transaction(const std::string& u, const std::string& r, int d);
        Transaction(const std::string& u, const std::string& r, const std::string& i, const std::string& d, const std::string& rd, bool a);

        std::string get_issue_date() const;
        std::string get_due_date() const;
        std::string get_username() const;
        std::string get_resource_id() const;
        std::string get_returned_date() const;
        
        std::string get_transaction_details() const;
        std::string get_transaction_header() const;
        void set_username(const std::string& u);
        std::string convert_to_string() const;

        void mark_complete();
        bool is_active() const;

        void set_returned_date(const std::string& date);

        friend std::ostream& operator<<(std::ostream& os, const Transaction& transaction);
};
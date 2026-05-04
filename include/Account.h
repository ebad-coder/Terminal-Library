#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Notification.h"

enum class AccountType { Member, Admin };

class Account {
    protected:
        std::string username;
        std::string password;
        std::string full_name;
        std::string address;
        std::vector<std::unique_ptr<Notification>> inbox;

    public:
        Account() = default;

        virtual ~Account() = default;

        Account(const std::string& u, const std::string& p, const std::string& f, const std::string& a);

        std::string get_username() const;

        std::string get_password() const;

        std::string get_fullname() const;

        std::string get_address() const;

        int get_unread_count() const;

        int get_inbox_size() const;

        Notification* get_notification_at(int index) const;

        virtual AccountType get_account_type() const = 0;

        virtual std::string convert_to_string() const = 0;

        void handle_notification(std::unique_ptr<Notification> n);
        
        void remove_notification(int index);

        friend std::ostream& operator<<(std::ostream& os, const Account& account);
};
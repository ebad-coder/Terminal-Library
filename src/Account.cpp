#include "../include/Account.h"

Account::Account(const std::string &u, const std::string &p, const std::string &f, const std::string &a)
            : username(u), password(p), full_name(f), address(a) {}

std::string Account::get_username() const { return username; }

std::string Account::get_password() const { return password; }

std::string Account::get_fullname() const { return full_name; }

std::string Account::get_address()  const { return address; }

int Account::get_unread_count() const
{
    int unread_count = 0;

    for (const std::unique_ptr<Notification>& notification : inbox)
    {
        if (!notification->is_read())
        {
            ++unread_count;
        }
    }

    return unread_count;
}

int Account::get_inbox_size() const { return inbox.size(); }

Notification* Account::get_notification_at(int index) const { return inbox[index].get(); }

void Account::handle_notification(std::unique_ptr<Notification> n) { inbox.push_back(std::move(n)); }

void Account::remove_notification(int index) { inbox.erase(inbox.begin() + index); }

std::ostream& operator<<(std::ostream& os, const Account& account) {
    os << account.convert_to_string();
    return os;
}
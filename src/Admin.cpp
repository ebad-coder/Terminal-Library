#include "../include/Admin.h"

Admin::Admin(const std::string& u, const std::string& p, const std::string& f, const std::string& a)
        : Account(u, p, f, a) {}

AccountType Admin::get_account_type() const { return AccountType::Admin; }

std::string Admin::convert_to_string() const
{
    return "Admin|" + username + "|" + password + "|" + full_name + "|" + address + "\n";
}

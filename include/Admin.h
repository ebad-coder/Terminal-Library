#pragma once

#include "Account.h"

class Admin : public Account {
    public:
        Admin() = default;

        Admin(const std::string&, const std::string&, const std::string&, const std::string&);
        
        AccountType get_account_type() const override;

        std::string convert_to_string() const override;
};
#pragma once

#include <unordered_map>
#include "Resource.h"
#include "Account.h"

class FileHandler {
    private:
        static void load_transactions(std::unordered_map<std::string, std::unique_ptr<Account>>& accounts_database);
        static void save_transactions(const std::unordered_map<std::string, std::unique_ptr<Account>>& accounts_database);
        static void load_notifications(std::unordered_map<std::string, std::unique_ptr<Account>>& accounts_database);
        static void save_notifications(const std::unordered_map<std::string, std::unique_ptr<Account>>& accounts_database);

    public:
        static void load_resources(std::unordered_map<std::string, std::unique_ptr<Resource>>& resources_database, const std::string& file_name);
        static void save_resources(const std::unordered_map<std::string, std::unique_ptr<Resource>>& resources_database, const std::string& file_name);
        static void load_accounts(std::unordered_map<std::string, std::unique_ptr<Account>>& accounts_database);
        static void save_accounts(const std::unordered_map<std::string, std::unique_ptr<Account>>& accounts_database);
};
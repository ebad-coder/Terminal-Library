#include "../include/FileHandler.h"
#include "../include/Article.h"
#include "../include/Book.h"
#include "../include/Journal.h"
#include "../include/Admin.h"
#include "../include/Member.h"
#include <fstream>
#include <sstream>
#include <stdexcept>


/************************************************************************************************************************/


void FileHandler::load_resources(std::unordered_map<std::string, std::unique_ptr<Resource>>& resources_database, const std::string& file_name)
{
    std::ifstream file(file_name);

    if (!file.is_open()) {
        throw std::runtime_error("ERROR: Unable to open data file: " + file_name);
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        std::string id, title, category, publication_date, available_str, fine_str;

        std::stringstream ss(line);

        std::getline(ss, id, '|');
        std::getline(ss, title, '|');
        std::getline(ss, category, '|');
        std::getline(ss, publication_date, '|');
        std::getline(ss, available_str, '|');
        std::getline(ss, fine_str, '|');

        int fine_per_day = std::stoi(fine_str);

        bool availability_status = (available_str == "1");

        std::unique_ptr<Resource> resource;

        if (id[0] == 'B')
        {
            std::string isbn, author, publisher, edition, language;
            
            std::getline(ss, isbn, '|');
            std::getline(ss, author, '|');
            std::getline(ss, publisher, '|');
            std::getline(ss, edition, '|');
            std::getline(ss, language, '|');

            resource = std::make_unique<Book>(
                id, title, category, publication_date, availability_status, fine_per_day,
                isbn, author, publisher, edition, language
            );
        }

        else if (id[0] == 'A')
        {
            std::string journal_name, author, issue_number;
            
            std::getline(ss, journal_name, '|');
            std::getline(ss, author, '|');
            std::getline(ss, issue_number, '|');

            resource = std::make_unique<Article>(
                id, title, category, publication_date, availability_status, fine_per_day,
                journal_name, author, issue_number
            );
        }

        else if (id[0] == 'J')
        {
            std::string issn, editor, publisher, volume_str;
            
            std::getline(ss, issn, '|');
            std::getline(ss, editor, '|');
            std::getline(ss, publisher, '|');
            std::getline(ss, volume_str, '|');

            int volume_number = std::stoi(volume_str);

            resource = std::make_unique<Journal>(
                id, title, category, publication_date, availability_status, fine_per_day,
                issn, editor, publisher, volume_number
            );
        }

        std::string username;

        while (std::getline(ss, username, '|'))
        {
            resource->add_member_to_reservation(username);
        }

        resources_database.emplace(resource->get_resource_id(), std::move(resource));
    }

    file.close();
}


/************************************************************************************************************************/


void FileHandler::save_resources(const std::unordered_map<std::string, std::unique_ptr<Resource>>& resources_database, const std::string& file_name)
{
    std::ofstream file(file_name);

    if (!file.is_open()) {
        throw std::runtime_error("ERROR: Unable to open data file: " + file_name);
    }

    for (const auto& resource_entry : resources_database)
    {
        file << *resource_entry.second;
    }

    file.close();
}


/************************************************************************************************************************/


void FileHandler::save_accounts(const std::unordered_map<std::string, std::unique_ptr<Account>>& accounts_database)
{
    save_notifications(accounts_database);
    save_transactions(accounts_database);

    std::ofstream file("../data/accounts.txt");
    
    if (!file.is_open()) {
        throw std::runtime_error("ERROR: Unable to open data file: accounts.txt");
    }

    for (const auto& account_entry : accounts_database)
    {
        file << *account_entry.second;
    }

    file.close();
}


/************************************************************************************************************************/


void FileHandler::load_accounts(std::unordered_map<std::string, std::unique_ptr<Account>>& accounts_database)
{
    std::ifstream file("../data/accounts.txt");

    if (!file.is_open()) {
        throw std::runtime_error("ERROR: Unable to open data file: accounts.txt");
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty()) continue;
        
        std::stringstream ss(line);

        std::string type, username, password, full_name, address;

        std::getline(ss, type, '|');
        std::getline(ss, username, '|');
        std::getline(ss, password, '|');
        std::getline(ss, full_name, '|');
        std::getline(ss, address, '|');

        if (type == "Admin")
        {
            accounts_database.emplace(username, std::make_unique<Admin>(username, password, full_name, address));
        }

        else if (type == "Member")
        {
            std::string balance_str, membership, resource_donated_str;

            std::getline(ss, balance_str, '|');
            int balance = std::stoi(balance_str);

            std::getline(ss, membership, '|');
            MemberShipType membership_type = membership == "Standard" ? MemberShipType::Standard : MemberShipType::Premium;

            std::getline(ss, resource_donated_str, '|');
            int resources_donated = std::stoi(resource_donated_str);
            
            std::unique_ptr<Member> member = std::make_unique<Member>(username, password, full_name, address, balance, membership_type, resources_donated);

            std::string resource_id;

            while (std::getline(ss, resource_id, '|'))
            {
                member->handle_borrowing(resource_id);
            }

            accounts_database.emplace(username, std::move(member));
        }
    }

    file.close();

    load_notifications(accounts_database);
    load_transactions(accounts_database);
}


/************************************************************************************************************************/


void FileHandler::load_transactions(std::unordered_map<std::string, std::unique_ptr<Account>>& accounts_database)
{
    std::ifstream file("../data/transactions.txt");

    if (!file.is_open()) {
        throw std::runtime_error("ERROR: Unable to open data file: transactions.txt");
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        std::stringstream ss(line);

        std::string username, resource_id, issue_date, due_date, returned_date, active_str;

        std::getline(ss, username, '|');
        std::getline(ss, resource_id, '|');
        std::getline(ss, issue_date, '|');
        std::getline(ss, due_date, '|');
        std::getline(ss, returned_date, '|');
        std::getline(ss, active_str, '|');

        bool active = (active_str == "1");

        auto it = accounts_database.find(username);

        if (it == accounts_database.end()) continue;

        if (it->second->get_account_type() != AccountType::Member) continue;

        Member* member = static_cast<Member*>(it->second.get());

        member->handle_transaction(std::make_unique<Transaction>(
            username,
            resource_id,
            issue_date,
            due_date,
            returned_date,
            active
        ));
    }

    file.close();
}


/************************************************************************************************************************/


void FileHandler::save_transactions(const std::unordered_map<std::string, std::unique_ptr<Account>>& accounts_database)
{
    std::ofstream file("../data/transactions.txt");

    if (!file.is_open()) {
        throw std::runtime_error("ERROR: Unable to open data file: transactions.txt");
    }

    for(const auto& account_entry : accounts_database)
    {
        if(account_entry.second->get_account_type() == AccountType::Member)
        {
            Member* member = static_cast<Member*>(account_entry.second.get());

            for(int i = 0; i < member->get_borrowing_history_size(); ++i)
            {
                file << *(member->get_transaction_at(i));
            }
        }
    }

    file.close();
}


/************************************************************************************************************************/


void FileHandler::load_notifications(std::unordered_map<std::string, std::unique_ptr<Account>>& accounts_database)
{
    std::ifstream file("../data/notifications.txt");

    if (!file.is_open()) {
        throw std::runtime_error("ERROR: Unable to open data file: notifications.txt");
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        std::stringstream ss(line);

        std::string sender, receiver, title, body, date, read_status_str, resource_id;
        
        std::getline(ss, sender, '|');
        std::getline(ss, receiver, '|');
        std::getline(ss, title, '|');
        std::getline(ss, body, '|');

        size_t pos = 0;
        while ((pos = body.find("[NL]", pos)) != std::string::npos)
        {
            body.replace(pos, 4, "\n");
            pos += 1;
        }
        
        std::getline(ss, date, '|');
        std::getline(ss, read_status_str, '|');
        std::getline(ss, resource_id, '|');

        bool read_status = (read_status_str == "1");

        auto it = accounts_database.find(receiver);

        if (it == accounts_database.end()) continue;

        it->second->handle_notification(std::make_unique<Notification>(
            sender,
            receiver,
            title,
            body,
            date,
            read_status,
            resource_id
        ));
    }

    file.close();
}


/************************************************************************************************************************/


void FileHandler::save_notifications(const std::unordered_map<std::string, std::unique_ptr<Account>>& accounts_database)
{
    std::ofstream file("../data/notifications.txt");

    if (!file.is_open()) {
        throw std::runtime_error("ERROR: Unable to open data file: notifications.txt");
    }

    for (const auto& account_entry : accounts_database)
    {
        Account* current_account = account_entry.second.get();

        for (int i = 0; i < current_account->get_inbox_size(); ++i)
        {
            file << *(current_account->get_notification_at(i));
        }
    }

    file.close();
}


/************************************************************************************************************************/
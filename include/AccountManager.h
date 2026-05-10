#pragma once

#include <unordered_map>
#include <memory>
#include "Account.h"
#include "Validator.h"

class Member;
class Resource;

class AccountManager {
    private:
        std::unordered_map<std::string, std::unique_ptr<Account>> accounts_database;

        void generate_reservation_failed_notification(Resource* resource, Member* borrower, BorrowingEligibility eligibility);
    
    public:
        AccountManager();

        ~AccountManager();

        Account* authenticate(const std::string& username, const std::string& password, AccountType expected_type);
        Account* register_member(const std::string& username, const std::string& password, const std::string& fullname, const std::string& address, int deposit);

        Account* get_account_with_username(const std::string& username);
        Member* get_next_borrower(Resource* resource);
        int broadcast_notification_to_members(const std::string& s, const std::string& t, const std::string& b, const std::string& d, const std::string& r_id);
        bool send_donation_request(Member* member, Resource *resource);
        std::vector<Member*> get_all_members();
        bool is_unique_username(const std::string& username);
        void close_account(Member* member);
        void update_inboxes(const std::string& old_username, const std::string& new_username);
        void update_username_key(const std::string& old_username, const std::string& new_username);
};
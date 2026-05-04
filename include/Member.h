#pragma once

#include "Account.h"
#include "Transaction.h"

inline constexpr int SECURITY_DEPOSIT = 500;
inline constexpr int MAX_SECURITY_DEPOSIT = 5000;
inline constexpr int STANDARD_TRANSACTIONS = 2;
inline constexpr int PREMIUM_TRANSACTIONS = 5;
inline constexpr int STANDARD_BORROWS = 2;
inline constexpr int PREMIUM_BORROWS = 5;
inline constexpr int STANDARD_BORROWING_DAYS = 7;
inline constexpr int PREMIUM_BORROWING_DAYS = 14;

enum class MemberShipType { Standard, Premium };

class Member : public Account {
    private:
        std::vector<std::string> borrowed_resources_id;
        std::vector<std::unique_ptr<Transaction>> borrowing_history;
        int balance;
        MemberShipType membership_type;
        int resources_donated;

    public:
        Member() = default;
        
        Member(const std::string& u, const std::string& p, const std::string& f, const std::string& a, int b);

        Member(const std::string& u, const std::string& p, const std::string& f, const std::string& a, int b, MemberShipType t, int r);

        AccountType get_account_type() const override;
        std::string convert_to_string() const override;

        void handle_borrowing(const std::string& resource_id);
        void handle_transaction(std::unique_ptr<Transaction>);
        void handle_return(const std::string& id);

        int get_borrowing_days() const;
        int get_max_borrow_count() const;
        int get_max_transaction_count() const;
        int get_balance() const;
        MemberShipType get_membership_type() const;
        int get_borrowing_history_size() const;
        Transaction* get_transaction_at(int index) const;
        int get_borrowed_resources_count() const;
        std::string get_borrowed_resource_at(int index) const;
        int get_resources_donated() const;

        void withdraw(int money);
        void deposit(int money);

        void increment_donations();

        void upgrade_membership();
        void remove_transaction(int index);

        void update_profile(const std::string& f, const std::string& u, const std::string& p, const std::string& a);

        std::string get_account_details() const;
};
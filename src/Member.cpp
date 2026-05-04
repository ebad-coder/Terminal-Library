#include "../include/Member.h"
#include <algorithm>



Member::Member(const std::string& u, const std::string& p, const std::string& f, const std::string& a, int b) 
            : Account(u, p, f, a), balance(b), membership_type(MemberShipType::Standard), resources_donated(0) {}



Member::Member(const std::string &u, const std::string &p, const std::string &f, const std::string &a, int b, MemberShipType t, int r) 
            : Account(u, p, f, a), balance(b), membership_type(t), resources_donated(r) {} 



AccountType Member::get_account_type() const { return AccountType::Member; }



std::string Member::convert_to_string() const
{
    std::string membership = membership_type == MemberShipType::Standard ? "Standard" : "Premium";

    std::string result =  "Member|" + username + "|" + password + "|" + full_name + "|" + address + "|" + std::to_string(balance) + "|" + membership + "|" + std::to_string(resources_donated);

    for (const std::string& id : borrowed_resources_id)
    {
        result += "|" + id;
    }

    return result + "\n";
}



int Member::get_borrowing_days() const
{
    if (membership_type == MemberShipType::Standard) return STANDARD_BORROWING_DAYS;
    else return PREMIUM_BORROWING_DAYS;
}



int Member::get_max_borrow_count() const
{
    if (membership_type == MemberShipType::Standard) return STANDARD_BORROWS;
    else return PREMIUM_BORROWS;
}



int Member::get_max_transaction_count() const
{
    if (membership_type == MemberShipType::Standard) return STANDARD_TRANSACTIONS;
    else return PREMIUM_TRANSACTIONS;
}



int Member::get_balance() const { return balance; }



MemberShipType Member::get_membership_type() const { return membership_type; }



Transaction* Member::get_transaction_at(int index) const { return borrowing_history[index].get(); }



int Member::get_borrowed_resources_count() const { return borrowed_resources_id.size(); }



std::string Member::get_borrowed_resource_at(int index) const { return borrowed_resources_id[index]; }



int Member::get_resources_donated() const { return resources_donated; }



int Member::get_borrowing_history_size() const { return borrowing_history.size(); }



void Member::handle_return(const std::string &id)
{
    borrowed_resources_id.erase(std::remove(borrowed_resources_id.begin(), borrowed_resources_id.end(), id), borrowed_resources_id.end());
}



void Member::withdraw(int money) { balance -= money; }



void Member::deposit(int money) { balance += money; }



void Member::increment_donations() { ++resources_donated; }



void Member::upgrade_membership()
{
    if (membership_type == MemberShipType::Standard) membership_type = MemberShipType::Premium;
}



void Member::remove_transaction(int index)
{
    borrowing_history.erase(borrowing_history.begin() + index);
}



void Member::update_profile(const std::string &f, const std::string &u, const std::string &p, const std::string &a)
{
    if (username != u)
    {
        for (const std::unique_ptr<Transaction>& transaction : borrowing_history)
        {
            if (transaction->get_username() == username)
            {
                transaction->set_username(u);
            }
        }    
    }
    
    full_name = f;
    username = u;
    password = p;
    address = a;
}



void Member::handle_borrowing(const std::string &resource_id)
{
    borrowed_resources_id.push_back(resource_id);
}



void Member::handle_transaction(std::unique_ptr<Transaction> transaction)
{
    borrowing_history.push_back(std::move(transaction));
}



std::string Member::get_account_details() const
{
    std::string protected_password = "";

    for (int i = 0; i < password.length(); ++i)
    {
        if (i % 5 == 0 || i == password.length() - 1) {
            protected_password += password[i];
        } else
        {
            protected_password += "*";
        }
    }

    std::string membership = get_membership_type() == MemberShipType::Standard ? "Standard" : "Premium";

    std::string details = "\nMember Details\n";
    details += "Username          : " + username + '\n';
    details += "Password          : " + protected_password + '\n';
    details += "Full Name         : " + full_name + '\n';
    details += "Address           : " + address + '\n';
    details += "Balance           : Rs." + std::to_string(balance) + '\n';
    details += "Membership Tier   : " + membership + '\n';
    details += "Resources Donated : " + std::to_string(resources_donated) + '\n';

    return details;
}
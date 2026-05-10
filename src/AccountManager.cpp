#include "../include/AccountManager.h"
#include "../include/Member.h"
#include "../include/FileHandler.h"
#include "../include/Admin.h"
#include "../include/Date.h"


/*****************************************************************************************************************/


AccountManager::AccountManager()
{
    FileHandler::load_accounts(accounts_database);
}


/*****************************************************************************************************************/


AccountManager::~AccountManager()
{
    FileHandler::save_accounts(accounts_database);
}


/*****************************************************************************************************************/


// first finds the account with the input username. if account is not found, then returns nullptr
// if found, then checks that input password and account type (admin/member) matches, if yes then returns pointer to that account
// if not matches, then returns nullptr
Account* AccountManager::authenticate(const std::string& username, const std::string& password, AccountType expected_type)
{
    Account* account = get_account_with_username(username);

    if (account)
    {
        if (account->get_password() == password && account->get_account_type() == expected_type)
        {
            return account;
        }
    }

    return nullptr;
}

/*****************************************************************************************************************/


// creates a new member object through unique_ptr with the input details
Account* AccountManager::register_member(const std::string& username, const std::string& password, const std::string& fullname, const std::string& address, int deposit)
{
    std::unique_ptr<Account> new_member = std::make_unique<Member>(
        username,
        password,
        fullname,
        address,
        deposit
    );

    // Store pointer before moving ownership into database
    Account* member_ptr = new_member.get();

    // Add the new member into the accounts list
    accounts_database.emplace(username, std::move(new_member));

    // Return the pointer to the newly created member
    return member_ptr;
}

/*****************************************************************************************************************/


// returns pointer to the account found with the input username, or nullptr otherwise
Account* AccountManager::get_account_with_username(const std::string& username)
{
    auto it = accounts_database.find(username);

    if (it != accounts_database.end())
    {
        return it->second.get();
    }

    return nullptr;
}


/*****************************************************************************************************************/


// finds the next eligible borrower of the resource in its reservation queue
Member* AccountManager::get_next_borrower(Resource* resource)
{
    while (!resource->is_reservation_queue_empty())
    {
        Member* next_borrower = static_cast<Member*>(get_account_with_username(resource->get_member_at_front()));
        resource->remove_member_from_front();

        // check if member is eligible for borrowing
        BorrowingEligibility eligibility = Validator::is_eligible_for_borrowing(next_borrower, resource);

        if (eligibility == BorrowingEligibility::Eligible || eligibility == BorrowingEligibility::TransactionLimit || eligibility == BorrowingEligibility::ReturnedToday)
        {
            return next_borrower;
        }
        else
        {
            generate_reservation_failed_notification(resource, next_borrower, eligibility);
        }
    }

    return nullptr;
}

/*****************************************************************************************************************/


// sends notification to all members and returns the count
int AccountManager::broadcast_notification_to_members(const std::string& s, const std::string& t, const std::string& b, const std::string& d, const std::string& r_id)
{
    int count = 0;

    for(const auto& account_entry : accounts_database)
    {
        if(account_entry.second->get_account_type() == AccountType::Member)
        {
            account_entry.second->handle_notification(std::make_unique<Notification>(s, account_entry.second->get_username(), t, b, d, false, r_id));
            ++count;
        }
    }
    return count;
}


/*****************************************************************************************************************/


// sends donation request to library admin
bool AccountManager::send_donation_request(Member* member, Resource* resource)
{
    Admin* admin = nullptr;

    for (const auto& account_entry : accounts_database)
    {
        if (account_entry.second->get_account_type() == AccountType::Admin)
        {
            admin = static_cast<Admin*>(account_entry.second.get());
            break;
        }
    }
    
    if (!admin) return false;

    admin->handle_notification(std::make_unique<Notification>(
        member->get_username(),
        admin->get_username(),
        "Donation Request",
        "A member has sent a request for donating a resource",
        Date::get_current_date(),
        false,
        resource->get_resource_id()
    ));

    return true;
}


/*****************************************************************************************************************/


// store all members in a vector and returns it
std::vector<Member*> AccountManager::get_all_members()
{
    std::vector<Member*> members;
    
    for (const auto& account_entry : accounts_database)
    {
        if (account_entry.second->get_account_type() == AccountType::Member)
        {
            Member* member = static_cast<Member*>(account_entry.second.get());
            if (member) members.push_back(member);
        }
    }

    return members;
}


/*****************************************************************************************************************/


// checks if input username is unique in the database
bool AccountManager::is_unique_username(const std::string &username)
{
    return accounts_database.find(username) == accounts_database.end();
}


/*****************************************************************************************************************/


// first deletes all the notification related to the member to remove, then removes the member from the database
void AccountManager::close_account(Member* member)
{
    for (const auto& account_entry : accounts_database)
    {
        Account* current_account = account_entry.second.get();

        for (int i = current_account->get_inbox_size() - 1; i >= 0; --i)
        {
            if (current_account == member) continue;

            Notification* current_notification = current_account->get_notification_at(i);

            if (current_notification->get_sender() == member->get_username())
            {
                current_account->remove_notification(i);
            }
        }
    }
   
    accounts_database.erase(accounts_database.find(member->get_username()));
}


/*****************************************************************************************************************/


// updates all notification with the new username
void AccountManager::update_inboxes(const std::string& old_username, const std::string& new_username)
{
    for (const auto& account_entry : accounts_database)
    {
        Account* current_account = account_entry.second.get();

        for (int i = 0; i < current_account->get_inbox_size(); ++i)
        {
            Notification* current_notification = current_account->get_notification_at(i);

            if (current_notification->get_sender() == old_username)
            {
                current_notification->set_sender(new_username);
            }
            else if (current_notification->get_receiver() == old_username)
            {
                current_notification->set_receiver(new_username);
            }
        }
    }    
}


/*****************************************************************************************************************/


// removes the old username key and creates a new one with the new username
void AccountManager::update_username_key(const std::string &old_username, const std::string &new_username)
{
    auto it = accounts_database.find(old_username);

    if (it != accounts_database.end())
    {
        accounts_database[new_username] = std::move(it->second);
        accounts_database.erase(it);
    }
}


/*****************************************************************************************************************/


// generates and send reservation failed notification to member, indicating that the resource they reserved can't be added to their borrowed resources
void AccountManager::generate_reservation_failed_notification(Resource *resource, Member *borrower, BorrowingEligibility eligibility)
{
    std::string message = "The resource you reserved was not added to your borrowed resources.\n";

    if (eligibility == BorrowingEligibility::InsufficientBalance)
    {
        message += "You must have a minimum of Rs.500 in your account to borrow a resource. ";
    }
    else if (eligibility == BorrowingEligibility::BorrowingLimit)
    {
        message += "You already had " + std::to_string(borrower->get_max_borrow_count()) + " resources borrowed at that time. ";
    }

    message += "\nConsider reserving that resource again.";

    std::unique_ptr<Notification> notification = std::make_unique<Notification>(
        "System",
        borrower->get_username(),
        "Reservation Failed",
        message,
        Date::get_current_date(),
        false,
        resource->get_resource_id()
    );

    borrower->handle_notification(std::move(notification));
}


/*****************************************************************************************************************/
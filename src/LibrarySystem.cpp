#include "../include/LibrarySystem.h"
#include "../include/UserInterface.h"
#include "../include/Validator.h"
#include "../include/Notification.h"
#include "../include/Article.h"
#include "../include/Book.h"
#include "../include/Journal.h"
#include "../include/Date.h"
#include <iomanip>
#include "LibrarySystem.h"


/*****************************************************************************************************************/


LibrarySystem::LibrarySystem()
{
    current_user = nullptr;
}


/*****************************************************************************************************************/


// runs the member version of the program
// first shows authentication menu (login/register) and asks user how to authenticate themselves
// then shows the member dashboard through which the member can select the operation they want to perform
void LibrarySystem::run_member_mode()
{
    while (true)
    {
        int login_choice = UserInterface::show_member_authentication_menu();
    
        switch (login_choice)
        {
            case 1:
                handle_user_registration();
                break;
            case 2:
                handle_user_authentication(AccountType::Member);
                break;
            case 3:
                return;
        }
    
        while (current_user)
        {
            int operation_choice = UserInterface::show_member_dashboard(current_user);
            Member* member = static_cast<Member*>(current_user);
    
            switch (operation_choice)
            {
                case 1:
                {
                    Resource* found_resource = handle_searching_resource();
                    if (found_resource) 
                    {
                        UserInterface::add_text_to_buffer(found_resource->get_resource_details());
                        UserInterface::show_buffered_message();
                    }
                    break;
                }
                case 2:
                    handle_view_available_resources();
                    break;
                case 3:
                    handle_borrowing_resource(member);
                    break;
                case 4:
                    handle_return_resource(member);
                    break;
                case 5:
                    handle_reserving_resource(member);
                    break;
                case 6:
                    handle_view_inbox();
                    break;
                case 7:
                    handle_view_borrowing_history(member);
                    break;
                case 8:
                    handle_view_balance(member);
                    break;
                case 9:
                    handle_view_membership(member);
                    break;
                case 10:
                    handle_view_profile(member);
                    break;
                case 11:
                    handle_donate_resource(member);
                    break;
                case 12:
                    current_user = logout();
                    break;
            }    
        }
    }
}


/*****************************************************************************************************************/


// runs the admin version of the program
// first shows authentication menu through which admin can login to their account
// then shows the admin dashboard through which the admin can select the operation they want to perform
void LibrarySystem::run_admin_mode()
{
    while (true)
    {
        int login_choice = UserInterface::show_admin_authentication_menu();
    
        switch (login_choice)
        {
            case 1:
                handle_user_authentication(AccountType::Admin);
                break;
            case 2:
                return;
        }
    
        while (current_user)
        {
            int operation_choice = UserInterface::show_admin_dashboard(current_user);
        
            switch (operation_choice)
            {
                case 1:
                    handle_add_resource();
                    break;
                case 2:
                    handle_update_resource();
                    break;
                case 3:
                    handle_remove_resource();
                    break;
                case 4:
                    handle_view_all_members();
                    break;
                case 5:
                    handle_send_notification();
                    break;
                case 6:
                    handle_view_donation_request();
                    break;
                case 7:
                    handle_generate_report();
                    break;
                case 8:
                    current_user = logout();
                    break;
            }
        }
    }
}


/*****************************************************************************************************************/


// allow a user to register themselves as a member of the library
// calls UserInterface to show registration form through which member input details such as fullname, username, password, address, and deposit.
// then calls AccountManager to open a new member account with the details provided
void LibrarySystem::handle_user_registration()
{
    std::string fullname, username, password, address, deposit_str;
    std::string error_message = "";

    while (true)
    {
        // show the registration form in a loop
        bool submit = UserInterface::show_registration_form(fullname, username, password, address, deposit_str, error_message);

        // If user clicked cancel, then abort the registration process
        if (!submit) return;

        if (fullname.length() < 4 || fullname.find(' ') == std::string::npos)
        {
            error_message = "Error: Please enter your full name.";
            continue;
        }
        
        // validate username entered by the user
        UsernameStatus username_status = Validator::check_username(username);
        if (!account_manager.is_unique_username(username))
        {
            error_message = "Error: This username is already taken.";
            continue;
        }
        else if (username_status == UsernameStatus::TooShort)
        {
            error_message = "Error: Username must be at least 8 characters long.";
            continue;
        }
        else if (username_status == UsernameStatus::InvalidCharacters)
        {
            error_message = "Error: Username must only contain [a-z] and [0-9].";
            continue;
        }

        // check password strength.
        PasswordStrength password_status = Validator::check_password_strength(password);
        if (password_status == PasswordStrength::TooShort) {
            error_message = "Error: Password must be at least 8 characters long.";
            continue;
        }
        else if (password_status == PasswordStrength::MissingComplexity) {
            error_message = "Error: Password must contain at least 1 uppercase, 1 lowercase and 1 digit.";
            continue;
        }

        // check address is detailed enough
        if (address.length() < 12 || address.find(' ') == std::string::npos)
        {
            error_message = "Error: The address entered is too short. Please enter a detailed one.";
            continue;
        }

        // check deposit entered is an integer and is between 500-5000 range
        int deposit = 0;
        try
        {
            deposit = std::stoi(deposit_str);
            if (deposit < 500 || deposit > 5000) {
                error_message = "Error: Account balance must be between Rs.500 and Rs.5000.";
                continue;
            }
        }
        catch (...)
        {
            error_message = "Error: Please enter a valid number for the deposit.";
            continue;
        }

        // if all validation passed, then register the new member
        current_user = account_manager.register_member(username, password, fullname, address, deposit);
        
        UserInterface::add_text_to_buffer("Account successfully created! You are now logged in.\n");
        UserInterface::show_buffered_message();

        break; // exit the form loop
    }
}


/*****************************************************************************************************************/


// allow user to login to their account
// first calls UserInterface to show login form through which user input username and password of their account
// then calls AccountManager to authenticate the details entered
void LibrarySystem::handle_user_authentication(AccountType expected_type)
{
    std::string username = "";
    std::string password = "";
    std::string error_message = "";

    while (true)
    {
        // show the login form in a loop
        bool submit = UserInterface::show_login_form(username, password, error_message);

        if (!submit) return; // User cancelled

        // check username and password is correct, and also account type matches (admin/member)
        current_user = account_manager.authenticate(username, password, expected_type);

        // exit the loop when the user is successfully logged-in to their account
        if (current_user)
        {
            UserInterface::add_text_to_buffer("You are successfully logged in.\n");
            UserInterface::show_buffered_message();
            break;
        }
        else
        {
            // Login failed, set error message and clear password field so they can try again
            error_message = "Login failed. Username or password may be incorrect.";
            password = ""; 
        }
    }
}


/*****************************************************************************************************************/


// calls resource manager to obtain all the available resources in the library
// then displays them in a list in a loop, and asks user to which resource to view, until user presses "Go back"
void LibrarySystem::handle_view_available_resources()
{
    std::vector<Resource*> available_resources = resource_manager.get_available_resources();

    // there are no available resources
    if (available_resources.empty())
    {
        UserInterface::add_text_to_buffer("There are currently no available resources...\n");
        UserInterface::show_buffered_message();
        return;
    }
    
    while (true)
    {
        // push available resources header to vector for displaying them in a list.
        std::vector<std::string> items;
        for (Resource* resource : available_resources)
        {
            items.push_back(resource->get_resource_header());
        }

        int choice = UserInterface::show_selection_list("AVAILABLE RESOURCES", items);
    
        if (choice == 0) return; // User pressed "Go Back"
    
        // Show the details of the selected resource
        UserInterface::add_text_to_buffer(available_resources[choice-1]->get_resource_details());
        UserInterface::show_buffered_message();
    }
}


/*****************************************************************************************************************/


// first asks user the search criteria (title/category/creator) and input the search term through UserInterface
// then calls resource manager to search and find all the resources that matched
// then displays the matched resources in a list, and asks from user which resource they were looking for.
// then return a pointer to the selected resource
Resource *LibrarySystem::handle_searching_resource()
{
    int criteria_index = 0; // Defaults to title
    std::string search_term = "";

    bool submit = UserInterface::show_search_form(criteria_index, search_term);
    
    if (!submit) return nullptr; // user pressed cancel

    SearchCriteria criteria;
    if      (criteria_index == 0) criteria = SearchCriteria::Title;
    else if (criteria_index == 1) criteria = SearchCriteria::Category;
    else if (criteria_index == 2) criteria = SearchCriteria::Creator;

    std::vector<Resource*> matches = resource_manager.search_resource(search_term, criteria);

    // no resource matched the search term
    if (matches.empty())
    {
        UserInterface::add_text_to_buffer("Sorry, no resources matched your search.\n");
        UserInterface::show_buffered_message();
        return nullptr;
    }

    // push matched resources header into a vector for displaying them in a list.
    std::vector<std::string> items;
    for (Resource* resource : matches)
    {
        items.push_back(resource->get_resource_header());
    }

    int choice = UserInterface::show_selection_list("SEARCH RESULTS", items);

    if (choice == 0) return nullptr;

    // map the choice to the correct resource
    Resource* selected_resource = matches[choice-1];

    // auto-select an available copy if the chosen one is unavailable
    if (!selected_resource->is_available())
    {
        for (Resource* resource : matches)
        {
            if ((selected_resource->get_resource_header() == resource->get_resource_header()) && resource->is_available())
            {
                selected_resource = resource;
                break;
            }
        }
    }

    return selected_resource;
}


/*****************************************************************************************************************/


// allow user to borrow an available resource from our library
// first user searches for the resource they want to borrow
// then checks whether the user is eligible through Validator for borrowing that resource or not
// if the resource is unavailable, then user can simply reserve it
// if available, we call resource manager to perform borrowing of this resource
// then displays the transaction details related to borrowing
void LibrarySystem::handle_borrowing_resource(Member *member)
{  
    Resource *resource_to_borrow = handle_searching_resource();
    if (resource_to_borrow == nullptr) return;

    // Buffer the details so they inject directly into the confirmation modal
    UserInterface::add_text_to_buffer(resource_to_borrow->get_resource_details());
    bool wish = UserInterface::get_confirmation("Are you sure you want to borrow this resource?");

    // user cancelled the borrowing
    if (!wish) return;

    BorrowingEligibility eligibility = Validator::is_eligible_for_borrowing(member, resource_to_borrow);

    if (eligibility == BorrowingEligibility::NotAvailable) {
        UserInterface::add_text_to_buffer("Borrowing failed: The resource is not available at the moment.\n");
        UserInterface::add_text_to_buffer("You can reserve this resource. It will be added to your borrowed resources as soon as it becomes available.\n");
        
        bool reserve_choice = UserInterface::get_confirmation("Do you wish to reserve this resource instead?");

        if (reserve_choice) {
            ReservationResult result = resource_manager.attempt_reservation(member, resource_to_borrow);
            if (result == ReservationResult::Success) {
                UserInterface::add_text_to_buffer("You are successfully added to the reservation queue for this resource.\n");
            }
            else {
                UserInterface::add_text_to_buffer("Reservation failed. You may already have reserved this resource.\n");
            }
            UserInterface::show_buffered_message();
        }
        return;
    }
    else if (eligibility == BorrowingEligibility::AlreadyBorrowed) {
        UserInterface::add_text_to_buffer("Borrowing failed: You already have this resource borrowed.\n");
        UserInterface::show_buffered_message();
        return;
    }
    else if (eligibility == BorrowingEligibility::BorrowingLimit) {
        UserInterface::add_text_to_buffer("Borrowing failed: You can not borrow more than " + std::to_string(member->get_max_borrow_count()) + " resources at once.\n");
        if (member->get_membership_type() == MemberShipType::Standard) {
            UserInterface::add_text_to_buffer("Consider upgrading your account to premium. A premium member can borrow upto " + std::to_string(PREMIUM_BORROWS) + " resources at once.\n");
        }
        UserInterface::show_buffered_message();
        return;
    }
    else if (eligibility == BorrowingEligibility::InsufficientBalance) {
        UserInterface::add_text_to_buffer("Borrowing failed: You must have a minimum of Rs." + std::to_string(SECURITY_DEPOSIT) + " in your account to borrow a resource.\n");
        UserInterface::show_buffered_message();
        return;
    }
    else if (eligibility == BorrowingEligibility::TransactionLimit) {
        UserInterface::add_text_to_buffer("Borrowing failed: You cannot perform more than " + std::to_string(member->get_max_transaction_count()) + " transactions in a day. Try again tomorrow.\n");
        if (member->get_membership_type() == MemberShipType::Standard) {
            UserInterface::add_text_to_buffer("Consider upgrading your account to premium. A premium member can perform upto " + std::to_string(PREMIUM_TRANSACTIONS) + " transactions in a day.\n");
        }
        UserInterface::show_buffered_message();
        return;
    }
    else if (eligibility == BorrowingEligibility::ReturnedToday) {
        UserInterface::add_text_to_buffer("Borrowing failed: You cannot borrow a resource you just returned today.\n");
        UserInterface::show_buffered_message();
        return;
    }

    Transaction* transaction = resource_manager.perform_borrowing(member, resource_to_borrow, true);

    UserInterface::add_text_to_buffer("Borrowing Successful.\n");
    UserInterface::add_text_to_buffer(transaction->get_transaction_details());
    UserInterface::add_text_to_buffer("\nPlease return the resource within the due date. A fine of Rs." + std::to_string(resource_to_borrow->get_fine()) + " applies daily after the due date.\n");

    UserInterface::show_buffered_message();
}


/*****************************************************************************************************************/


// allow user to return a resource they have borrowed
// if they haven't borrowed a resource, then simply asks them if they want to borrow one
// otherwise display all the resources header user has currently borrowed in a list, and ask user which one to return through UserInterface in a loop
// then calls the resource manager to help returning the resource, and also calculate fine if user is returning late
// then calls account manager to find next eligible borrower from the reservation queue of the resource just returned
// then calls resource manager to attempt borrowing for the same resource for next borrower (if any)
void LibrarySystem::handle_return_resource(Member* member)
{    
    if(member->get_borrowed_resources_count() == 0)
    {
        UserInterface::add_text_to_buffer("You currently have no borrowed resources...\n");

        bool borrow_wish = UserInterface::get_confirmation("Do you wish to borrow a resource now?");

        if (borrow_wish)
        {
            handle_borrowing_resource(member);
        }
        return;
    }
    
    while (true)
    {
        // store the updated borrowed resources count when user returns a resource
        int num_borrowed_resources = member->get_borrowed_resources_count();

        if (num_borrowed_resources <= 0)
        {
            UserInterface::add_text_to_buffer("You have returned all your borrowed resources.\n");
            UserInterface::show_buffered_message();
            return;
        }

        std::vector<std::string> items;
        
        for(int i = 0; i < num_borrowed_resources; ++i)
        {
            Resource* resource = resource_manager.get_resource_with_id(member->get_borrowed_resource_at(i));	
            items.push_back(resource->get_resource_header());
        }
    
        int choice = UserInterface::show_selection_list("RETURN A RESOURCE", items);
    
        if(choice == 0) return; // User pressed "Cancel / Go Back"
        
        // Map the choice to the correct resource
        Resource* resource_to_return = resource_manager.get_resource_with_id(member->get_borrowed_resource_at(choice-1));
        
        // Buffer the resource details for the confirmation modal
        UserInterface::add_text_to_buffer(resource_to_return->get_resource_details());

        bool return_wish = UserInterface::get_confirmation("Are you sure you want to return this resource?");

        if (!return_wish) continue;
    
        int fine = resource_manager.help_returning_resource(member, resource_to_return);
    
        UserInterface::add_text_to_buffer("Thank you for returning the resource. Keep exploring more!\n");
    
        if (fine > 0)
        {
            UserInterface::add_text_to_buffer("You returned this resource late. View your inbox for more details regarding the fine.\n");
        }
    
        Member* next_borrower = account_manager.get_next_borrower(resource_to_return);
    
        if (next_borrower)
        {
            resource_manager.perform_borrowing(next_borrower, resource_to_return, false);
        }

        UserInterface::show_buffered_message();
    }
}


/*****************************************************************************************************************/


// allow user to reserve a resource which is currently unavailable
// first user search the resource they want to reserve
// then calls resource manager to attempt reservation for the desired resource
// if the resource is already available, then member can borrow it, otherwise member is added to reservation queue of the resource
void LibrarySystem::handle_reserving_resource(Member *member)
{
    Resource *resource_to_reserve = handle_searching_resource();
    if (resource_to_reserve == nullptr) return;

    // Buffer details for the modal
    UserInterface::add_text_to_buffer(resource_to_reserve->get_resource_details());
    bool wish = UserInterface::get_confirmation("Are you sure you want to reserve this resource?");

    if (!wish) return;

    ReservationResult result = resource_manager.attempt_reservation(member, resource_to_reserve);

    if (result == ReservationResult::NotEligible)
    {
        UserInterface::add_text_to_buffer("You already have this resource borrowed or reserved.\n");
        UserInterface::show_buffered_message();
    }
    else if (result == ReservationResult::AlreadyAvailable)
    {
        UserInterface::add_text_to_buffer("The resource you want to reserve is actually available right now.\n");
        bool borrow_choice = UserInterface::get_confirmation("Do you wish to borrow it immediately?");

        if (borrow_choice) {
            // first check the eligibility for borrowing
            BorrowingEligibility eligibility = Validator::is_eligible_for_borrowing(member, resource_to_reserve);

            if (eligibility == BorrowingEligibility::Eligible) {
                Transaction* transaction = resource_manager.perform_borrowing(member, resource_to_reserve, true);
                UserInterface::add_text_to_buffer("Borrowing successful.\n");
                UserInterface::add_text_to_buffer(transaction->get_transaction_details());
            }
            else {
                UserInterface::add_text_to_buffer("Borrowing failed due to borrowing/transaction limits or insufficient balance.\n");
            }
            UserInterface::show_buffered_message();
        }
    }
    else
    {
        UserInterface::add_text_to_buffer("You have successfully been added to the reservation queue.\n");
        UserInterface::show_buffered_message();
    }
}


/*****************************************************************************************************************/


// allow user to view the notifications in their inbox.
// first calls UserInterface to display all the notifications in a list, and asks which notification to view
// then displays the selected notification and the resource associated with it.
void LibrarySystem::handle_view_inbox()
{
    int size = current_user->get_inbox_size();

    // inbox is empty
    if (size == 0)
    {
        UserInterface::add_text_to_buffer("It looks like your inbox is empty...\n");
        UserInterface::show_buffered_message();
        return;
    }

    while (true)
    {
        std::vector<std::string> items;
        
        // Populate the list backwards so newest messages are at the top
        for (int i = size - 1; i >= 0; --i)
        {
            items.push_back(current_user->get_notification_at(i)->get_notification_header());
        }
    
        int choice = UserInterface::show_selection_list("VIEW YOUR INBOX", items);
    
        if (choice == 0) return; // User pressed "Go Back"
    
        // Map the choice to the correct underlying notification
        Notification* selected_notification = current_user->get_notification_at(size - choice);
    
        // mark as read
        selected_notification->mark_as_read();
    
        // Buffer the notification details to be shown later
        UserInterface::add_text_to_buffer(selected_notification->get_notification_details());
    
        // get the resource associated with the notification
        Resource* resource = resource_manager.get_resource_with_id(selected_notification->get_resource_id());
    
        // Buffer the resource details as well.
        if (resource)
        {
            UserInterface::add_text_to_buffer(resource->get_resource_details());
        }
        else
        {
            UserInterface::add_text_to_buffer("\nThere was an error displaying the associated resource.\nThe resource may have been removed.");
        }

        UserInterface::show_buffered_message(); // Show the accumulated message modal
    }
}


/*****************************************************************************************************************/


// displays the borrowing history of a member
// first calls UserInterface to show all the transaction in a list, and asks user which one to view
// then shows the selected transaction details and resource associated with it
void LibrarySystem::handle_view_borrowing_history(Member *member)
{
    int history_size = member->get_borrowing_history_size();

    // no borrowing history.
    if (history_size == 0)
    {
        UserInterface::add_text_to_buffer("Looks like the borrowing history is empty...\n");
        UserInterface::show_buffered_message();
        return;
    }

    while (true)
    {
        std::vector<std::string> items;
        
        // Populate the list backwards so newest transactions are at the top
        for (int i = history_size - 1; i >= 0; --i)
        {
            Transaction* current_transaction = member->get_transaction_at(i);
            items.push_back(current_transaction->get_transaction_header());
        }
    
        std::string title = "@" + member->get_username() + " - BORROWING HISTORY";
        int choice = UserInterface::show_selection_list(title, items);
    
        if (choice == 0) return; // User pressed "Go Back"
    
        // Map the choice to the correct underlying transaction
        Transaction* selected_transaction = member->get_transaction_at(history_size - choice);

        // Buffer the transaction details to display later
        UserInterface::add_text_to_buffer(selected_transaction->get_transaction_details());
        
        // get the resource associated with the transaction
        Resource* resource = resource_manager.get_resource_with_id(selected_transaction->get_resource_id());
    
        // Buffer the resource detail as well
        if (resource)
        {
            UserInterface::add_text_to_buffer(resource->get_resource_details());
        }
        else
        {
            UserInterface::add_text_to_buffer("\nThere was an error displaying the associated resource.\nIt may have been removed.");
        }

        UserInterface::show_buffered_message(); // Show the accumulated message modal
    }
}


/*****************************************************************************************************************/


// allow member to view their balance, and also deposit money to their balance
// asks user if they want to deposit money, if yes then shows the deposit form through UserInterface and input amount to deposit.
// then add the new amount to member current balance
void LibrarySystem::handle_view_balance(Member *member)
{
    int balance = member->get_balance();

    // Buffer the balance details for later
    UserInterface::add_text_to_buffer("Your current balance is: Rs." + std::to_string(balance) + "\n");

    // member has reached the maximum deposit limit (Rs.5000)
    if (balance >= MAX_SECURITY_DEPOSIT)
    {
        UserInterface::add_text_to_buffer("\nYou have reached the maximum balance limit.\n");
        UserInterface::show_buffered_message();
        return;
    }

    // member balance is less than minimum security deposit (Rs.500)
    if (balance < SECURITY_DEPOSIT)
    {
        UserInterface::add_text_to_buffer("\nYou must deposit a minimum of Rs." + std::to_string(SECURITY_DEPOSIT - balance) + " to continue borrowing resources.\n");
    }

    bool wish = UserInterface::get_confirmation("Do you wish to deposit money into your account?");

    if (!wish) return;

    std::string deposit_str = "";
    std::string error_message = "";
    int deposit_amount = 0;

    while (true)
    {
        bool submit = UserInterface::show_deposit_form(balance, deposit_str, error_message);

        // user cancelled
        if (!submit)
        {
            return;
        }

        // Validate the deposit input
        try {
            deposit_amount = std::stoi(deposit_str);
            
            if (deposit_amount <= 0) {
                error_message = "Error: Please enter a positive amount.";
                continue;
            }
            if (balance + deposit_amount > MAX_SECURITY_DEPOSIT) {
                error_message = "Error: Cannot deposit more than Rs." + std::to_string(MAX_SECURITY_DEPOSIT - balance) + ".";
                continue;
            }
            
            break;
        } catch (...) {
            error_message = "Error: Please enter a valid numerical amount.";
        }
    }

    member->deposit(deposit_amount);

    UserInterface::add_text_to_buffer("Rs." + std::to_string(deposit_amount) + " has been successfully deposited to your account.\n");
    UserInterface::add_text_to_buffer("Your new balance is: Rs." + std::to_string(member->get_balance()) + "\n");

    UserInterface::show_buffered_message();
}


/*****************************************************************************************************************/


// allows member to view their membership status (Standard or Premium)
// then shows the benefits of each plan
// they can also upgrade their membership status by donating 3 resources or paying one-time fee of Rs.2000
void LibrarySystem::handle_view_membership(Member* member)
{
    std::string membership_status = member->get_membership_type() == MemberShipType::Standard ? "Standard" : "Premium";

    UserInterface::add_text_to_buffer("Your current membership status is: " + membership_status + '\n');

    UserInterface::show_membership_perks();

    if (member->get_membership_type() == MemberShipType::Premium)
    {
        UserInterface::show_buffered_message();
        return;
    }

    int choice = UserInterface::show_membership_upgrade_options();

    if (choice == 1)
    {
        handle_donate_resource(member);
    }
    else if (choice == 2)
    {
        if (member->get_balance() < 2000)
        {
            UserInterface::add_text_to_buffer("\nYou don't have enough balance to perform this operation.");
        }
        else
        {
            member->upgrade_membership();
            member->withdraw(2000);
            resource_manager.generate_membership_upgrade_notification(member);
            UserInterface::add_text_to_buffer("\nYour account has been successfully upgraded to premium.");
        }
        UserInterface::show_buffered_message();
    }
}


/*****************************************************************************************************************/


// allow member to view their profile details
// member can also edit their profile details, or close their account
void LibrarySystem::handle_view_profile(Member* member)
{
    UserInterface::add_text_to_buffer(member->get_account_details());

    // shows the menu asking if user wants to edit or close account
    int choice = UserInterface::show_profile_options();

    if (choice == 1) // Edit Profile
    {
        help_edit_profile(member);
    }
    else if (choice == 2) // Close Account
    {
        help_close_account(member);
    }
}


/*****************************************************************************************************************/


// allow member to donate a resource to the library
// first asks which resource to donate (article, book, journal) through UserInterface
// then input the resource details
// then calls account manager which sends donation request to library admin
// and finally calls resource manager which adds this donated resource to pending resources
void LibrarySystem::handle_donate_resource(Member* member)
{
    int choice = UserInterface::show_resource_options();

    if (choice == 4) return; // user selected "back"

    std::unique_ptr<Resource> resource_to_donate = resource_input(choice);

    // Check if the user cancelled the form
    if (!resource_to_donate) return;

    if (account_manager.send_donation_request(member, resource_to_donate.get()))
    {
        resource_manager.add_resource_to_pending(std::move(resource_to_donate));
        UserInterface::add_text_to_buffer("The resource you donated has been added to the pending list.\n");
        UserInterface::add_text_to_buffer("It will be added to our collection when an admin accept your donated resource.\n");
    }
    else
    {
        UserInterface::add_text_to_buffer("There was an error sending donation request to library admin.\n");
        UserInterface::add_text_to_buffer("Please try again later.");
    }

    UserInterface::show_buffered_message();
}


/*****************************************************************************************************************/


// allow admin to add a resource to the library
// it first asks which resource to add (article, book, journal), and then input its details
// then displays the new added resource and add it to the catalog
void LibrarySystem::handle_add_resource()
{
    int choice = UserInterface::show_resource_options();
    if (choice == 4) return;

    std::unique_ptr<Resource> resource_to_add = resource_input(choice);
    
    // Check if the user cancelled the form!
    if (!resource_to_add) return; 

    UserInterface::add_text_to_buffer("This resource has been added to our collection successfully.\n");
    UserInterface::add_text_to_buffer(resource_to_add->get_resource_details());

    resource_manager.add_new_resource(std::move(resource_to_add));
    UserInterface::show_buffered_message();
}


/*****************************************************************************************************************/


// allow admin to update an existing resource
// first admin search for the resource they want to update
// then asks for updated details of the selected resource through UserInterface
// then updates the resource with the new details
void LibrarySystem::handle_update_resource()
{
    Resource* resource_to_update = handle_searching_resource();
    if (resource_to_update == nullptr) return;

    std::string resource_id = resource_to_update->get_resource_id();
    int choice = (resource_id[0] == 'A') ? 1 : ((resource_id[0] == 'B') ? 2 : 3);

    // Extract current data to pre-fill the form!
    std::string title = resource_to_update->get_title();
    std::string category = resource_to_update->get_category();
    std::string pub_date = resource_to_update->get_publication_date();
    std::string fine = std::to_string(resource_to_update->get_fine());

    std::vector<std::string> extra_vals;

    if (choice == 1)
    {
        Article* article = static_cast<Article*>(resource_to_update);
        extra_vals = {article->get_journal_name(), article->get_author(), article->get_issue_number()};
    }
    else if (choice == 2)
    {
        Book* book = static_cast<Book*>(resource_to_update);
        extra_vals = {book->get_isbn(), book->get_author(), book->get_publisher(), book->get_edition(), book->get_language()};
    }
    else if (choice == 3)
    {
        Journal* journal = static_cast<Journal*>(resource_to_update);
        extra_vals = {journal->get_issn(), journal->get_editor(), journal->get_publisher(), std::to_string(journal->get_volume_number())};
    }

    std::string error_message = "";

    while (true) {
        bool submit = UserInterface::show_resource_form(choice, title, category, pub_date, fine, extra_vals, error_message);
        
        if (!submit) {
            UserInterface::add_text_to_buffer("Update operation cancelled.\n");
            UserInterface::show_buffered_message();
            return;
        }

        // Check for empty core fields
        if (title.empty() || category.empty() || pub_date.empty() || fine.empty()) {
            error_message = "Error: All fields are required. Please fill them out.";
            continue;
        }

        // Check for empty dynamic fields
        bool has_empty_extra = false;
        for (const std::string& val : extra_vals)
        {
            if (val.empty()) {
                has_empty_extra = true;
                break;
            }
        }

        if (has_empty_extra) {
            error_message = "Error: Please fill out all specific resource details.";
            continue;
        }

        if (!Validator::is_valid_date(pub_date)) {
            error_message = "Invalid date format. Please use DD/MM/YYYY.";
            continue;
        }

        int fine_val = 0;
        try {
            fine_val = std::stoi(fine);
            if (fine_val < 50 || fine_val > 100) {
                error_message = "Error: Fine must be between Rs. 50 and Rs. 100.";
                continue;
            }
        }
        catch (...) {
            error_message = "Error: Invalid fine amount. Please enter a valid number.";
            continue;
        }

        // Execute the update
        if (choice == 1)
        {
            static_cast<Article*>(resource_to_update)->update_resource(title, category, pub_date, fine_val, extra_vals[0], extra_vals[1], extra_vals[2]);
        }
        else if (choice == 2)
        {
            static_cast<Book*>(resource_to_update)->update_resource(title, category, pub_date, fine_val, extra_vals[0], extra_vals[1], extra_vals[2], extra_vals[3], extra_vals[4]);
        }
        else if (choice == 3)
        {
            int vol = 0;
            try {
                vol = std::stoi(extra_vals[3]);
            }
            catch (...) {
                error_message = "Invalid volume number.";
                continue;
            }
            static_cast<Journal*>(resource_to_update)->update_resource(title, category, pub_date, fine_val, extra_vals[0], extra_vals[1], extra_vals[2], vol);
        }
        break;
    }

    UserInterface::add_text_to_buffer("The resource has been updated successfully.\n");
    UserInterface::add_text_to_buffer(resource_to_update->get_resource_details());
    UserInterface::show_buffered_message();
}


/*****************************************************************************************************************/


// allows admin to remove a resource from library
// first admin searches for the resource they want to remove, if resource isn't available (currently borrowed) then it can't be removed
// then calls account manager to remove all notifications and transactions that were linked with the resource to remove
// then calls resource manager to remove the resource completely from the catalog
void LibrarySystem::handle_remove_resource()
{
    Resource* resource_to_remove = handle_searching_resource();
    if (resource_to_remove == nullptr) return;

    if (!resource_to_remove->is_available()) 
    {
        UserInterface::add_text_to_buffer("Unable to remove this resource as it is currently borrowed.\n");
        UserInterface::show_buffered_message();
        return;
    }

    // Buffer details for the modal
    UserInterface::add_text_to_buffer(resource_to_remove->get_resource_details());
    bool wish = UserInterface::get_confirmation("Are you sure you want to permanently remove this resource?");

    if (!wish) return;

    resource_manager.remove_resource(resource_to_remove->get_resource_id());

    UserInterface::add_text_to_buffer("The resource has been removed from our collection successfully.\n");
    UserInterface::show_buffered_message();
}


/*****************************************************************************************************************/


// allow admin to view all the members of library
// first calls account manager to retrieve all the members
// then displays all members in a list through UserInterface
// then asks admin which member to view its details and its borrowing history
void LibrarySystem::handle_view_all_members()
{
    std::vector<Member*> members = account_manager.get_all_members();

    if (members.empty())
    {
        UserInterface::add_text_to_buffer("It looks like there are no members in our library...\n");
        UserInterface::show_buffered_message();
        return;
    }

    while (true)
    {
        std::vector<std::string> items;
        
        for (Member* member : members)
        {
            items.push_back(member->get_fullname() + " (@" + member->get_username() + ")");
        }
    
        int choice = UserInterface::show_selection_list("ALL LIBRARY MEMBERS", items);
    
        if (choice == 0) return; // User pressed "Go Back"
    
        Member* selected_member = members[choice - 1];
    
        // Buffer the member details
        UserInterface::add_text_to_buffer(selected_member->get_account_details());
    
        bool wish = UserInterface::get_confirmation("Do you wish to view this member's borrowing history?");
    
        if (wish)
        {
            handle_view_borrowing_history(selected_member);
        }
    }
}


/*****************************************************************************************************************/


// allow admin to send notification to a specific member or all members
// admin writes the notification details (title, body)
// then it is broadcasted to a specific member (asks for username), or all members in the library
void LibrarySystem::handle_send_notification()
{
    int choice = UserInterface::show_notification_options();
    if (choice == 3) return; // Go back to menu

    std::string title, message, resource_id, target_username;
    std::string error_message = "";

    while (true)
    {
        bool submit = UserInterface::show_notification_form(choice, title, message, resource_id, target_username, error_message);

        // If admin cancels the form
        if (!submit)
        {
            return;
        }

        if (title.empty() || message.empty())
        {
            error_message = "Error: Title and Body cannot be empty.";
            continue;
        }

        if (!resource_id.empty() && resource_manager.get_resource_with_id(resource_id) == nullptr)
        {
            error_message = "Error: No resource found with ID " + resource_id;
            continue;
        }

        if (choice == 1) // Direct Message
        {
            if (target_username.empty())
            {
                error_message = "Error: Please enter a target username.";
                continue;
            }

            // Verify if user exists
            Account* target_member = account_manager.get_account_with_username(target_username);

            if (target_member == nullptr) {
                error_message = "Error: No member found with username '" + target_username + "'.";
                continue;
            }

            target_member->handle_notification(std::make_unique<Notification>(current_user->get_username(), target_username, title, message, Date::get_current_date(), false, resource_id));
            UserInterface::add_text_to_buffer("Notification sent to " + target_member->get_fullname() + " successfully!\n");
        }
        else if (choice == 2) // Broadcast
        {
            int count = account_manager.broadcast_notification_to_members(current_user->get_username(), title, message, Date::get_current_date(), resource_id);
            UserInterface::add_text_to_buffer("Notification broadcasted to " + std::to_string(count) + " member(s) successfully!\n");
        }

        UserInterface::show_buffered_message();
        break;
    }
}


/*****************************************************************************************************************/


// allow admin to view all the donation requests sent by members when donating resource
// first asks which request to view
// then shows the request and donated resource details and asks whether to accept it or not
// if admin accepts it, then this donated resource is added to main catalog through resource manager
void LibrarySystem::handle_view_donation_request()
{
    while (true)
    {
        // Re-evaluate size every loop because we are removing request as we process them
        int size = current_user->get_inbox_size();

        if (size == 0)
        {
            UserInterface::add_text_to_buffer("There are currently no pending donation requests.\n");
            UserInterface::show_buffered_message();
            return;
        }

        std::vector<std::string> items;
        
        // Populate the list backwards so newest requests are at the top
        for (int i = size - 1; i >= 0; --i)
        {
            items.push_back(current_user->get_notification_at(i)->get_notification_header());
        }
    
        int choice = UserInterface::show_selection_list("DONATION REQUESTS", items);
    
        if (choice == 0) return; // User pressed "Go Back"
    
        // Map the choice back to the correct notification index
        int target_index = size - choice;
        
        Notification* request = current_user->get_notification_at(target_index);
        Resource* resource_to_donate = resource_manager.get_donated_resource(request->get_resource_id());
    
        if (!resource_to_donate)
        {
            UserInterface::add_text_to_buffer("There was an error when displaying the resource.\nIt may have been removed.");
            UserInterface::show_buffered_message();
            current_user->remove_notification(target_index);
            continue;
        }

        // Buffer the details of the resource
        UserInterface::add_text_to_buffer(resource_to_donate->get_resource_details());
    
        bool accepted = UserInterface::get_confirmation("Do you want to accept this donated resource?");
    
        Member* donor = static_cast<Member*>(account_manager.get_account_with_username(request->get_sender()));
    
        resource_manager.handle_donated_resource(resource_to_donate, accepted, donor);
    
        if (accepted)
        {
            UserInterface::add_text_to_buffer("This resource has been added to our collection.\n");
        }
        else
        {
            UserInterface::add_text_to_buffer("You have rejected this resource.\n");
        }
    
        // remove the exact notification we just processed
        current_user->remove_notification(target_index);
        UserInterface::show_buffered_message();
    }
}


/*****************************************************************************************************************/


// allow admin to generate report showing issued and overdued resources
// first calls account manager to retrieve all the members
// then check every active transaction, if it is overdue then then push it to ovedue array, otherwise push to issued array
// then displays the issued and overdued resources details in a tabular form
void LibrarySystem::handle_generate_report()
{
    std::vector<Member*> all_members = account_manager.get_all_members();

    std::vector<std::string> issued_rows;
    std::vector<std::string> overdued_rows;

    for(Member *member : all_members)
    {
        int history_size = member->get_borrowing_history_size();

        for(int i = 0; i < history_size; ++i)
        {
            Transaction* current_transaction = member->get_transaction_at(i);

            if (!current_transaction->is_active()) continue;

            Resource* resource = resource_manager.get_resource_with_id(current_transaction->get_resource_id());
            std::string title = resource->get_title();

            std::ostringstream row;
            int days_difference = Date::get_days_between(current_transaction->get_due_date(), Date::get_current_date());
            if(days_difference > 0)
            {
                std::string days_str = std::to_string(days_difference) + " days";

                row << "| " << std::left << std::setw(12) << current_transaction->get_username().substr(0,12) << "| " << std::setw(11) << current_transaction->get_resource_id().substr(0,11) << "| " << std::setw(34) << title.substr(0,34) << "| " << std::setw(10) << current_transaction->get_due_date().substr(0,10) << "| " << std::setw(10) << days_str.substr(0,10) << "|";
                overdued_rows.push_back(row.str());
            }
            else
            {
                row << "| " << std::left << std::setw(12) << current_transaction->get_username().substr(0,12) << "| " << std::setw(11) << current_transaction->get_resource_id().substr(0,11) << "| " << std::setw(34) << title.substr(0,34) << "| " << std::setw(10) << current_transaction->get_issue_date().substr(0,10) << "| " << std::setw(10) << current_transaction->get_due_date().substr(0,10) << "|";
                issued_rows.push_back(row.str());
            }
        }
    }

    // Accumulate the entire report into a vector of strings
    std::vector<std::string> report_lines;
    
    std::string border = "======================================================================================";
    std::string divider = "--------------------------------------------------------------------------------------";

    report_lines.push_back(border);
    report_lines.push_back("                       LIBRARY AND RESOURCE MANAGEMENT SYSTEM");
    report_lines.push_back("                              ISSUED & OVERDUED REPORT");
    report_lines.push_back("                              Generated on: " + Date::get_current_date());
    report_lines.push_back(border);
    report_lines.push_back("");
    report_lines.push_back("  Total Issued   : " + std::to_string(issued_rows.size() + overdued_rows.size()));
    report_lines.push_back("  Total Overdue  : " + std::to_string(overdued_rows.size()));
    report_lines.push_back("");

    report_lines.push_back(divider);
    report_lines.push_back("                                   ISSUED RESOURCES");
    report_lines.push_back(divider);

    if(issued_rows.empty())
    {
        report_lines.push_back("                      There are no resources issued currently...");
    }
    else
    {
        std::ostringstream header;
        header << "| " << std::left << std::setw(12) << "Borrower" << "| " << std::setw(11) << "Resource ID" << "| " << std::setw(34) << "Title" << "| " << std::setw(10) << "Issue Date" << "| " << std::setw(10) << "Due Date " << "|";
               
        report_lines.push_back(header.str());
        report_lines.push_back("|" +  std::string(13, '-') + "+" + std::string(12, '-') + "+" + std::string(35, '-') + "+" + std::string(11, '-') + "+" + std::string(11, '-') + "|");

        for(const std::string& row : issued_rows)
        {
            report_lines.push_back(row);
        }
    }

    report_lines.push_back("");
    report_lines.push_back(divider);
    report_lines.push_back("                                  OVERDUED RESOURCES");
    report_lines.push_back(divider);
    
    if(overdued_rows.empty())
    {
        report_lines.push_back("                       Hurray! There are no resources overdue...");
    }
    else
    {
        std::ostringstream header;
        header << "| " << std::left << std::setw(12) << "Borrower" << "| " << std::setw(11) << "Resource ID" << "| " << std::setw(34) << "Title" << "| " << std::setw(10) << "Due Date" << "| " << std::setw(10) << "Days Late" << "|";
               
        report_lines.push_back(header.str());
        report_lines.push_back("|" +  std::string(13, '-') + "+" + std::string(12, '-') + "+" + std::string(35, '-') + "+" + std::string(11, '-') + "+" + std::string(11, '-') + "|");

        for(const std::string& row : overdued_rows)
        {
            report_lines.push_back(row);
        }
    }

    report_lines.push_back(divider);
    report_lines.push_back("");

    // Send the report to UserInterface to show it as a scrollable list
    UserInterface::show_report_view("SYSTEM REPORT", report_lines);
}


/*****************************************************************************************************************/


// helper function for taking resource details as input
// it then creates a new object based on the choice (article, book, journal)
std::unique_ptr<Resource> LibrarySystem::resource_input(int choice)
{
    std::string resource_id = resource_manager.generate_unique_id(choice);
    std::string title, category, publication_date;
    std::string fine = "50"; // Default fine
    
    // storing extra attribute values of each resource
    std::vector<std::string> extra_vals;
    if      (choice == 1) extra_vals.resize(3); // article has 3
    else if (choice == 2) extra_vals.resize(5); // book has 5
    else if (choice == 3) extra_vals.resize(4); // journal has 4

    std::string error_message = "";

    while (true) {
        bool submit = UserInterface::show_resource_form(choice, title, category, publication_date, fine, extra_vals, error_message);
        
        if (!submit) return nullptr; // User cancelled the operation

        // Check for empty fields of common attributes
        if (title.empty() || category.empty() || publication_date.empty() || fine.empty()) {
            error_message = "Error: All fields are required. Please fill them out.";
            continue;
        }

        // Check for empty resource-specific fields
        bool has_empty_extra = false;
        for (const std::string& val : extra_vals) {
            if (val.empty()) {
                has_empty_extra = true;
                break;
            }
        }

        if (has_empty_extra) {
            error_message = "Error: All fields are required. Please fill them out.";
            continue;
        }

        if (!Validator::is_valid_date(publication_date)) {
            error_message = "Invalid date format. Please use DD/MM/YYYY.";
            continue;
        }

        int fine_val = 50;
        try {
            fine_val = std::stoi(fine);
            if (fine_val < 50 || fine_val > 100) {
                error_message = "Error: Fine must be between Rs. 50 and Rs. 100.";
                continue;
            }
        } catch (...) {
            error_message = "Error: Invalid fine amount. Please enter a valid number.";
            continue;
        }

        bool availability = true;

        if (choice == 1) {
            return std::make_unique<Article>(resource_id, title, category, publication_date, availability, fine_val, extra_vals[0], extra_vals[1], extra_vals[2]);
        }
        else if (choice == 2) {
            return std::make_unique<Book>(resource_id, title, category, publication_date, availability, fine_val, extra_vals[0], extra_vals[1], extra_vals[2], extra_vals[3], extra_vals[4]);
        }
        else if (choice == 3) {
            int volume_number = 0;
            try {
                volume_number = std::stoi(extra_vals[3]);
            }
            catch (...) {
                error_message = "Invalid volume number.";
                continue;
            }
            return std::make_unique<Journal>(resource_id, title, category, publication_date, availability, fine_val, extra_vals[0], extra_vals[1], extra_vals[2], volume_number);
        }
    }

    return nullptr;
}


/*****************************************************************************************************************/


// helper function to edit a member profile details
// calls UserInterface to show the profile edit form where member can enter new details
// then resource manager is called to update every resource reservation queue with the new username
// and account manager is called to update every inbox notification with the new username
void LibrarySystem::help_edit_profile(Member* member)
{
    std::string old_username = member->get_username();
    std::string old_password = member->get_password();

    // storing old values to pre-fill the form
    std::string username = old_username;
    std::string password = old_password;
    std::string full_name = member->get_fullname();
    std::string address = member->get_address();
    std::string error_message = "";

    while (true)
    {
        bool submit = UserInterface::show_profile_form(full_name, username, password, address, error_message);

        if (!submit) return; // User cancelled edit

        if (full_name.length() < 4 || full_name.find(' ') == std::string::npos)
        {
            error_message = "Error: Please enter your full name.";
            continue;
        }

        // validate new username
        if (username != old_username)
        {
            if (!account_manager.is_unique_username(username)) {
                error_message = "Error: This username is already taken.";
                continue;
            }
            if (Validator::check_username(username) != UsernameStatus::Valid) {
                error_message = "Error: The username entered is too short or contains invalid characters.";
                continue;
            }
        }

        // validate new password
        if (password != old_password)
        {
            if (Validator::check_password_strength(password) != PasswordStrength::Strong) {
                error_message = "Error: Password must be at least 8 characters long with 1 uppercase, 1 lowercase and 1 digit.";
                continue;
            }
        }

        // validating address
        if (address.length() < 12 || address.find(' ') == std::string::npos) {
            error_message = "Error: The address entered is too short. Please enter a detailed one.";
            continue;
        }

        if (username != old_username) {
            resource_manager.update_reservation_queues(old_username, username);
            account_manager.update_inboxes(old_username, username);
            account_manager.update_username_key(old_username, username);
        }

        member->update_profile(full_name, username, password, address);

        UserInterface::add_text_to_buffer("Your profile has been updated successfully.\n");
        UserInterface::show_buffered_message();
        break;
    }
}


/*****************************************************************************************************************/


// helper function to close a member account
// first checks that if member has an outstanding balance or if they have a borrowed resource, then they can't close their account currently
// calls resource manager to remove this member from every resource reservation queue
// and calls account manager to delete this account from the database
void LibrarySystem::help_close_account(Member* member)
{
    if (member->get_balance() < 0) {
        UserInterface::add_text_to_buffer("Unable to close your account. You have an outstanding balance.\n");
        UserInterface::show_buffered_message();
        return;
    }
    if (member->get_borrowed_resources_count() > 0) {
        UserInterface::add_text_to_buffer("Unable to close your account. Please return your borrowed resources first.\n");
        UserInterface::show_buffered_message();
        return;
    }

    bool wish = UserInterface::get_confirmation("Are you sure you want to close your account?");

    if (!wish) return;

    resource_manager.remove_member_from_reservation_queues(member->get_username());
    account_manager.close_account(member);

    UserInterface::add_text_to_buffer("Your account has been closed successfully.\n");
    UserInterface::show_buffered_message();
    
    current_user = nullptr;
}


/*****************************************************************************************************************/


// asks user whether they are sure about logging out or not
Account* LibrarySystem::logout()
{
    // UserInterface shows confirmation dialogue box
    bool wish = UserInterface::get_confirmation("Are you sure you want to logout?");


    if (!wish) return current_user;
    return nullptr;
}


/*****************************************************************************************************************/


// main loop of the program
// first shows main menu asks how to continue (admin or member)
// then run the mode user selects or exits the program.
void LibrarySystem::run()
{
    while (true)
    {
        int mode_choice = UserInterface::show_main_menu();

        switch (mode_choice)
        {
            case 1:
                run_member_mode();
                break;

            case 2:
                run_admin_mode();
                break;

            case 3:
                return;
        }
    }
}


/*****************************************************************************************************************/
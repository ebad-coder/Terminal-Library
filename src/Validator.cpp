#include "../include/Validator.h"
#include "../include/Date.h"
#include "../include/Resource.h"
#include "../include/Member.h"


/*****************************************************************************************************************/


// check that if username is long enough and only contain characters [a-z, 0-9]
UsernameStatus Validator::check_username(const std::string& username)
{
    if (username.length() < 8)
    {
        return UsernameStatus::TooShort;
    }

    for (char c : username)
    {
        if ((c < 'a' || c > 'z') && (c < '0' || c > '9'))
        {
            return UsernameStatus::InvalidCharacters;
        }
    }

    return UsernameStatus::Valid;
}


/*****************************************************************************************************************/


// check that if password is strong enough, it should be atleast 8 characters long
// and contain atleast 1 uppercase, 1 lowercase and 1 digit
PasswordStrength Validator::check_password_strength(const std::string& password)
{
    if (password.length() > 12)
    {
        return PasswordStrength::Strong;
    }

    if (password.length() < 8)
    {
        return PasswordStrength::TooShort;
    }
    
    bool is_upper = false, is_lower = false, is_digit = false;

    for (char c : password)
    {
        if (c >= 'a' && c <= 'z')      is_lower = true;
        else if (c >= 'A' && c <= 'Z') is_upper = true;
        else if (c >= '0' && c <= '9') is_digit = true;
    }

    if (!is_lower || !is_upper || !is_digit)
    {
        return PasswordStrength::MissingComplexity;
    }

    return PasswordStrength::Strong;
}


/*****************************************************************************************************************/


bool Validator::string_match(const std::string& search_term, const std::string& actual_value)
{
    std::string actual_value_copy = actual_value;

    for (char& c : actual_value_copy)
    {
        c = tolower(c);
    }
        
    std::vector<std::string> search_words = Validator::string_to_array(search_term);

    for (const std::string& word : search_words)
    {
        if (actual_value_copy.find(word) == std::string::npos)
        {
            return false;
        }
    }

    return true;
}


/*****************************************************************************************************************/


bool Validator::is_eligible_for_reserving(Member *current_user, Resource* resource_to_reserve)
{

    for (int i = 0; i < current_user->get_borrowed_resources_count(); ++i)
    {
        if (current_user->get_borrowed_resource_at(i) == resource_to_reserve->get_resource_id())
        {
            return false;
        }
    }
    
    std::queue<std::string> temp;
    bool already_reserved = false;

    while (!resource_to_reserve->is_reservation_queue_empty())
    {
        if (resource_to_reserve->get_member_at_front() == current_user->get_username())
        {
            already_reserved = true;
            break;
        }

        temp.push(resource_to_reserve->get_member_at_front());
        resource_to_reserve->remove_member_from_front();
    }

    while (!resource_to_reserve->is_reservation_queue_empty())
    {
        temp.push(resource_to_reserve->get_member_at_front());
        resource_to_reserve->remove_member_from_front();
    }

    resource_to_reserve->set_new_reservation_queue(temp);

    return !already_reserved;
}


/*****************************************************************************************************************/


BorrowingEligibility Validator::is_eligible_for_borrowing(Member* current_user, Resource* resource_to_borrow)
{
    for (int i = 0; i < current_user->get_borrowed_resources_count(); ++i)
    {
        if (current_user->get_borrowed_resource_at(i) == resource_to_borrow->get_resource_id()) return BorrowingEligibility::AlreadyBorrowed;
    }

    if (!resource_to_borrow->is_available()) return BorrowingEligibility::NotAvailable;

    if (current_user->get_balance() < SECURITY_DEPOSIT) return BorrowingEligibility::InsufficientBalance;

    if (current_user->get_borrowed_resources_count() >= current_user->get_max_borrow_count()) return BorrowingEligibility::BorrowingLimit;

    int today_transactions = 0;

    for (int i = 0; i < current_user->get_borrowing_history_size(); ++i)
    {
        Transaction* current_transaction = current_user->get_transaction_at(i);

        if (current_transaction->get_returned_date() == Date::get_current_date() && current_transaction->get_resource_id() == resource_to_borrow->get_resource_id()) return BorrowingEligibility::ReturnedToday;

        if (current_transaction->get_issue_date() == Date::get_current_date()) ++today_transactions;

        if (today_transactions >= current_user->get_max_transaction_count()) return BorrowingEligibility::TransactionLimit;
    }

    return BorrowingEligibility::Eligible;
}


/*****************************************************************************************************************/


std::vector<std::string> Validator::string_to_array(const std::string& str)
{
    std::vector<std::string> result;
    std::string word = "";

    for (char c : str)
    {
        if (isspace(c))
        {
            if (!word.empty())
            {
                result.push_back(word);
                word = "";
            }
        }
        else
        {
            word += tolower(c);
        }
    }

    if (!word.empty())
    {
        result.push_back(word);
    }
    
    return result;
}


/*****************************************************************************************************************/


bool Validator::is_valid_date(const std::string& date)
{
    if (date.length() != 10) return false;

    if (date[2] != '/' || date[5] != '/') return false;

    for (int i = 0; i < date.length(); ++i)
    {
        if (i == 2 || i == 5) continue;

        if (!std::isdigit(date[i])) return false;
    }

    int day = 0, month = 0, year = 0;
    
    if (std::sscanf(date.c_str(), "%2d/%2d/%4d", &day, &month, &year) != 3)
        return false;

    if (year < 1) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;

    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    bool is_leap_year = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

    if (month == 2 && is_leap_year) days_in_month[1] = 29;

    if (day > days_in_month[month - 1]) return false;

    if (Date::get_days_between(Date::get_current_date(), date) > 0) return false;

    return true;
}


/**********************************************************************************************************/
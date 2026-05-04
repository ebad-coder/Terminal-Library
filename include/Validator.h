#pragma once

#include <string>
#include <vector>

class Resource;
class Member;

enum class PasswordStrength { Strong, TooShort, MissingComplexity };

enum class UsernameStatus { Valid, TooShort, InvalidCharacters };

enum class BorrowingEligibility { Eligible, AlreadyBorrowed, BorrowingLimit, InsufficientBalance, TransactionLimit, ReturnedToday, NotAvailable };

class Validator
{
    private:
        static std::vector<std::string> string_to_array(const std::string& str);

    public:
        static UsernameStatus check_username(const std::string& username);
        static PasswordStrength check_password_strength(const std::string& password);
        static bool string_match(const std::string& search_term, const std::string& actual_value);
        static bool is_eligible_for_reserving(Member* current_user, Resource* resource_to_reserve);
        static BorrowingEligibility is_eligible_for_borrowing(Member* current_user, Resource* resource_to_borrow);
        static bool is_valid_date(const std::string& date);
};
#pragma once

#include <string>
#include "Account.h"

class UserInterface {
    public:
        static int show_main_menu();
        static int show_member_authentication_menu();
        static int show_admin_authentication_menu();
        static int show_member_dashboard(Account* current_user);
        static int show_admin_dashboard(Account* current_user);
        static bool show_registration_form(std::string& fullname, std::string& username, std::string& password, std::string& address, std::string& deposit, const std::string& error_msg);
        static bool show_login_form(std::string& username, std::string& password, const std::string& error_msg);
        static bool show_search_form(int& criteria_index, std::string& search_term);
        static int show_selection_list(const std::string& title, const std::vector<std::string>& items);
        static bool show_deposit_form(int current_balance, std::string& deposit_amount, const std::string& error_msg);
        static void show_membership_perks();
        static int show_membership_upgrade_options();
        static int show_profile_options();
        static bool show_profile_form(std::string& fullname, std::string& username, std::string& password, std::string& address, const std::string& error_msg);
        static int show_resource_options();
        static bool show_resource_form(int type, std::string& title, std::string& category, std::string& pub_date, std::string& fine, std::vector<std::string>& extra_vals, const std::string& error_msg);
        static int show_notification_options();
        static bool show_notification_form(int choice, std::string& title, std::string& message, std::string& resource_id, std::string& target_username, const std::string& error_msg);
        static void show_report_view(const std::string& title, const std::vector<std::string>& lines);
        static void add_text_to_buffer(const std::string& prompt);
        static bool get_confirmation(const std::string& prompt);
        static void show_buffered_message();
};
#pragma once

#include "AccountManager.h"
#include "ResourceManager.h"

class LibrarySystem {
    private:
        ResourceManager resource_manager;
        AccountManager account_manager;
        Account* current_user;

        void run_member_mode();
        void run_admin_mode();

        void handle_user_registration();
        void handle_user_authentication(AccountType expected_type);

        Resource* handle_searching_resource();
        void handle_view_available_resources();
        void handle_borrowing_resource(Member* member);
        void handle_return_resource(Member* member);
        void handle_reserving_resource(Member* current_member);
        void handle_view_inbox();
        void handle_view_borrowing_history(Member* member);
        void handle_view_balance(Member* member);
        void handle_view_membership(Member* member);
        void handle_view_profile(Member* member);
        void handle_donate_resource(Member* member);

        void handle_add_resource();
        void handle_update_resource();
        void handle_remove_resource();
        void handle_view_all_members();
        void handle_send_notification();
        void handle_view_donation_request();
        void handle_generate_report();
        
        std::unique_ptr<Resource> resource_input(int choice);
        Account* logout();
        void help_edit_profile(Member* member);
        void help_close_account(Member* member);

    public:
        LibrarySystem();
        void run();
};

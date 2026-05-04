#pragma once

#include "Member.h"
#include "Resource.h"
#include <unordered_map>

enum class ReservationResult { Success, AlreadyAvailable, NotEligible };

enum class SearchCriteria { Title, Creator, Category };

class ResourceManager {
    private:
        std::unordered_map<std::string, std::unique_ptr<Resource>> resources_database;
        std::unordered_map<std::string, std::unique_ptr<Resource>> pending_resources_database;

        void generate_reservation_notification(Member* current_user, Resource* resource_to_reserve);
        void generate_borrowing_notification(Member* member, Resource* resource_to_borrow);
        void generate_returning_notification(Member* member, Resource* resource_to_return, int total_fine);
        void generate_donation_accepted_notification(Member* donor, const std::string& resource_id);
        void generate_donation_rejected_notification(Member* donor, const std::string& resource_id);
        void generate_reservation_complete_notification(Member* member, Resource* resource);
        
    public:
        ResourceManager();
        ~ResourceManager();
        
        Resource* get_resource_with_id(const std::string& id);
        std::vector<Resource*> search_resource(const std::string& search_term, SearchCriteria criteria);
        std::vector<Resource*> get_available_resources();
        ReservationResult attempt_reservation(Member* current_user, Resource* resource_to_reserve);
        void add_new_resource(std::unique_ptr<Resource> resource);
        Transaction* perform_borrowing(Member* member, Resource* resource_to_borrow, bool is_borrowing);
        int help_returning_resource(Member* member, Resource* resource_to_return);
        std::string generate_unique_id(int choice);
        void add_resource_to_pending(std::unique_ptr<Resource> resource);
        Resource* get_donated_resource(const std::string& id);
        void handle_donated_resource(Resource* resource_to_donate, bool accepted, Member* donor);
        void remove_resource(const std::string& resource_id);
        void update_reservation_queues(const std::string& old_username, const std::string& new_username);
        void remove_member_from_reservation_queues(const std::string& username);
        void generate_membership_upgrade_notification(Member* member);
};
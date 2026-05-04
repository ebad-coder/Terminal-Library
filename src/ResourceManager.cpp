#include "../include/ResourceManager.h"
#include "../include/Validator.h"
#include "../include/FileHandler.h"
#include "../include/Notification.h"
#include "../include/Date.h"
#include "ResourceManager.h"


/********************************************************************************************************/


ResourceManager::ResourceManager()
{
    FileHandler::load_resources(resources_database, "../data/resources.txt");
    FileHandler::load_resources(pending_resources_database, "../data/pending_resources.txt");
}


/********************************************************************************************************/


ResourceManager::~ResourceManager()
{
    FileHandler::save_resources(resources_database, "../data/resources.txt");
    FileHandler::save_resources(pending_resources_database, "../data/pending_resources.txt");
}


/********************************************************************************************************/


// store all available resources in a vector and returns it
std::vector<Resource*> ResourceManager::get_available_resources()
{
    std::vector<Resource*> available_resources;

    for (const auto& resource_entry : resources_database)
    {
        if (resource_entry.second->is_available())
        {
            available_resources.push_back(resource_entry.second.get());
        }
    }

    return available_resources;
}


/********************************************************************************************************/


// returns the resource with the input id
Resource* ResourceManager::get_resource_with_id(const std::string &id)
{
    auto it = resources_database.find(id);

    if (it != resources_database.end()) return it->second.get();

    return nullptr;
}


/********************************************************************************************************/


// store all the resources in a vector which mathched the search term and returns them
std::vector<Resource*> ResourceManager::search_resource(const std::string& search_term, SearchCriteria criteria)
{
    std::vector<Resource*> matches;

    for (const auto& resource_entry : resources_database)
    {
        std::string actual_value;

        if      (criteria == SearchCriteria::Title)    actual_value = resource_entry.second->get_title();
        else if (criteria == SearchCriteria::Creator)  actual_value = resource_entry.second->get_creator();
        else if (criteria == SearchCriteria::Category) actual_value = resource_entry.second->get_category();

        if (Validator::string_match(search_term, actual_value))
        {
            matches.push_back(resource_entry.second.get());
        }
    }

    return matches;
}


/********************************************************************************************************/


// attemps reservation of the resource
// first checks if resource is already available and the member is eligible for reserving
// if yes, then add this member to reservation queue of the resource, and sends notification to the member reserving
ReservationResult ResourceManager::attempt_reservation(Member* current_user, Resource* resource_to_reserve)
{
    if (resource_to_reserve->is_available())
    {
        return ReservationResult::AlreadyAvailable;
    }
    else if (!Validator::is_eligible_for_reserving(current_user, resource_to_reserve))
    {
        return ReservationResult::NotEligible;
    }

    resource_to_reserve->add_member_to_reservation(current_user->get_username());

    generate_reservation_notification(current_user, resource_to_reserve);

    return ReservationResult::Success;
}


/********************************************************************************************************/


// generates and sends reservation notification to member reserving a resourec
void ResourceManager::generate_reservation_notification(Member *current_user, Resource *resource_to_reserve)
{
    std::unique_ptr<Notification> notification = std::make_unique<Notification>(
        "System",
        current_user->get_username(),
        "Reservation Sucessful",
        "You have reserved a resource, it will be automatically added to your borrowed resources as soon as it becomes available.",
        Date::get_current_date(),
        false,
        resource_to_reserve->get_resource_id()
    );

    current_user->handle_notification(std::move(notification));
}


/********************************************************************************************************/


// adds new resource to the resources database
void ResourceManager::add_new_resource(std::unique_ptr<Resource> resource)
{
    if (resource)
    {
        resources_database.emplace(resource->get_resource_id(), std::move(resource));
    }
}


/********************************************************************************************************/


// performs borrowing of a resource
// add the resource to member borrowing list
// creates a transaction object of this borrowing, and adds it to member borrowing history
// send borrowing notification to member borrowing, and returns the borrowing transaction
Transaction* ResourceManager::perform_borrowing(Member* member, Resource* resource_to_borrow, bool is_borrowing)
{
    member->handle_borrowing(resource_to_borrow->get_resource_id());

    resource_to_borrow->set_availability(false);
    
    std::unique_ptr<Transaction> transaction = std::make_unique<Transaction>(
        member->get_username(),
        resource_to_borrow->get_resource_id(),
        member->get_borrowing_days()
    );

    Transaction* current_transaction = transaction.get();

    member->handle_transaction(std::move(transaction));

    if (is_borrowing)
    {
        generate_borrowing_notification(member, resource_to_borrow);
    }
    else
    {
        generate_reservation_complete_notification(member, resource_to_borrow);
    }

    return current_transaction;
}


/********************************************************************************************************/


// generates and send borrowing successful notification to member
void ResourceManager::generate_borrowing_notification(Member* member, Resource* resource_to_borrow)
{
    std::unique_ptr<Notification> notification = std::make_unique<Notification>(
        "System",
        member->get_username(),
        "Borrowing Sucessful",
        "You have borrowed a resource. Please return the resource within the due date.\nAfter the due date, a fine of Rs. " + std::to_string(resource_to_borrow->get_fine()) + " will be applied each day until the resource is not returned.",
        Date::get_current_date(),
        false,
        resource_to_borrow->get_resource_id()
    );

    member->handle_notification(std::move(notification));
}


/********************************************************************************************************/


// removes the resource from member borrowing list.
// calculates fine if member is returning late, and apply fines if any
// then sends notification to member returning the resource.
int ResourceManager::help_returning_resource(Member* member, Resource* resource_to_return)
{
    member->handle_return(resource_to_return->get_resource_id());

    resource_to_return->set_availability(true);

    int total_fine = 0;

    for (int i = 0; i < member->get_borrowing_history_size(); ++i)
    {
        Transaction* current_transaction = member->get_transaction_at(i);

        if (current_transaction->get_resource_id() == resource_to_return->get_resource_id() && current_transaction->is_active())
        {
            int days = Date::get_days_between(current_transaction->get_due_date(), Date::get_current_date());

            // positive days represents member is returning late
            if (days > 0)
            {
                total_fine = days * resource_to_return->get_fine();
            }

            current_transaction->mark_complete();
            current_transaction->set_returned_date(Date::get_current_date());
            break;
        }
    }

    member->withdraw(total_fine);

    generate_returning_notification(member, resource_to_return, total_fine);

    return total_fine;
}


/********************************************************************************************************/


// generates a unique id for new resource
std::string ResourceManager::generate_unique_id(int choice)
{
    std::string resource_id = "";
    bool exist = true;

    while (exist)
    {
        int random_id = (std::rand() % 9000) + 1000;

        if      (choice == 1) resource_id = "A" + std::to_string(random_id);
        else if (choice == 2) resource_id = "B" + std::to_string(random_id);
        else if (choice == 3) resource_id = "J" + std::to_string(random_id);

        exist = resources_database.find(resource_id) != resources_database.end();
    }

    return resource_id;
}


/********************************************************************************************************/


// adds donated resource to pending list
void ResourceManager::add_resource_to_pending(std::unique_ptr<Resource> resource)
{
    if (resource)
    {
        pending_resources_database.emplace(resource->get_resource_id(), std::move(resource));
    }
}


/********************************************************************************************************/


// returns the resource in pending database with given id
Resource* ResourceManager::get_donated_resource(const std::string &id)
{
    auto it = pending_resources_database.find(id);

    if (it != pending_resources_database.end()) return it->second.get();

    return nullptr;
}


/********************************************************************************************************/


// if admin accepts the donation, then the donated resource is added to main database
// also upgrades the donor membership status to Premium if their donation count reaches 3
void ResourceManager::handle_donated_resource(Resource* resource_to_donate, bool accepted, Member* donor)
{
    std::string resource_id = resource_to_donate->get_resource_id();

    auto it = pending_resources_database.find(resource_id);

    if (it == pending_resources_database.end()) return;

    if (accepted)
    {
        resources_database.emplace(resource_id, std::move(it->second));
        generate_donation_accepted_notification(donor, resource_id);
        donor->increment_donations();
    }
    else
    {
        generate_donation_rejected_notification(donor, resource_id);
    }

    pending_resources_database.erase(it);

    if (donor->get_resources_donated() >= 3 && donor->get_membership_type() == MemberShipType::Standard)
    {
        donor->upgrade_membership();
        generate_membership_upgrade_notification(donor);
    }
}


/********************************************************************************************************/


// removes a resource from database
void ResourceManager::remove_resource(const std::string& resource_id)
{
    resources_database.erase(resource_id);
}


/********************************************************************************************************/


// updates all resources reservation queue with the new username
void ResourceManager::update_reservation_queues(const std::string &old_username, const std::string &new_username)
{
    for (const auto& resource_entry : resources_database)
    {
        Resource* current_resource = resource_entry.second.get();

        std::queue<std::string> temp;

        while (!current_resource->is_reservation_queue_empty())
        {
            if (current_resource->get_member_at_front() == old_username)
            {
                temp.push(new_username);
            }
            else
            {
                temp.push(current_resource->get_member_at_front());
            }

            current_resource->remove_member_from_front();
        }

        current_resource->set_new_reservation_queue(temp);
    }
}


/********************************************************************************************************/


// removes a member from all resources reservation queues
void ResourceManager::remove_member_from_reservation_queues(const std::string& username)
{
    for (const auto& resource_entry : resources_database)
    {
        Resource* current_resource = resource_entry.second.get();

        std::queue<std::string> temp;

        while (!current_resource->is_reservation_queue_empty())
        {
            if (current_resource->get_member_at_front() != username)
            {
                temp.push(current_resource->get_member_at_front());
            }

            current_resource->remove_member_from_front();
        }

        current_resource->set_new_reservation_queue(temp);
    }
}


/********************************************************************************************************/


// generates and send notification to member returning a resource.
void ResourceManager::generate_returning_notification(Member* member, Resource* resource_to_return, int total_fine)
{
    std::string late_message = "";

    if (total_fine > 0)
    {
        int days_late = total_fine / resource_to_return->get_fine();
        late_message = "\nYou returned " + std::to_string(days_late) + " day(s) late. A fine of Rs. " + std::to_string(total_fine) + " has been applied to your account.\n";
    }

    std::unique_ptr<Notification> notification = std::make_unique<Notification>(
        "System",
        member->get_username(),
        "Returned Sucessful",
        "Thankyou for returning the resource. " + late_message + "Keep exploring more.",
        Date::get_current_date(),
        false,
        resource_to_return->get_resource_id()
    );

    member->handle_notification(std::move(notification));
}


/********************************************************************************************************/


// generates and send notification to member whose donated resource is accepted
void ResourceManager::generate_donation_accepted_notification(Member *donor, const std::string &resource_id)
{
    std::unique_ptr<Notification> notification = std::make_unique<Notification>(
        "System",
        donor->get_username(),
        "Donation Sucessful",
        "Your resource donation request has been accepted. Thank you for your generous contribution.\nIt means a lot to our library and the community we serve.",
        Date::get_current_date(),
        false,
        resource_id
    );

    donor->handle_notification(std::move(notification));
}


/********************************************************************************************************/


// generates and send notification to member whose donated resource is rejected
void ResourceManager::generate_donation_rejected_notification(Member *donor, const std::string &resource_id)
{
    std::unique_ptr<Notification> notification = std::make_unique<Notification>(
        "System",
        donor->get_username(),
        "Donation Request Rejected",
        "Unfortunately, the resource you donated has not been accepted this time.\nThank you for your interest in supporting our library.",
        Date::get_current_date(),
        false,
        resource_id
    );

    donor->handle_notification(std::move(notification));
}


/********************************************************************************************************/


void ResourceManager::generate_reservation_complete_notification(Member *member, Resource *resource)
{
    std::unique_ptr<Notification> notification = std::make_unique<Notification>(
        "System",
        member->get_username(),
        "Reservation Completed",
        "The resource you reserved is now added to your borrowed resources.",
        Date::get_current_date(),
        false,
        resource->get_resource_id()
    );

    member->handle_notification(std::move(notification));
}


/********************************************************************************************************/


// generates and send notification to member whose membership has been upgraded
void ResourceManager::generate_membership_upgrade_notification(Member *member)
{
    std::unique_ptr<Notification> notification = std::make_unique<Notification>(
        "System",
        member->get_username(),
        "Membership Upgraded",
        "Your account has been upgraded to premium membership.\nYou can now enjoy all the benefits of Premium account.",
        Date::get_current_date(),
        false
    );

    member->handle_notification(std::move(notification));
}


/********************************************************************************************************/
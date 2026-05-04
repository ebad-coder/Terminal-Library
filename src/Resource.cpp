#include "../include/Resource.h"

Resource::Resource(const std::string &id, const std::string &t, const std::string &c, std::string y, bool a, int f)
            : resource_id(id), title(t), category(c), publication_date(y), availability_status(a), fine_per_day(f) {}

std::string Resource::get_resource_details() const
{
    std::string details = "ID               : " + resource_id + '\n';
    details += "Title            : " + title + '\n';
    details += "Category         : " + category + '\n';
    details += "Publication Date : " + publication_date + '\n';
    details += "Fine per day     : Rs." + std::to_string(fine_per_day) + '\n';
    return details;
}

bool Resource::is_available() const { return availability_status; }

std::string Resource::get_title() const { return title; }

std::string Resource::get_category() const { return category; }

int Resource::get_fine() const { return fine_per_day; }

std::string Resource::get_resource_id() const { return resource_id; }

std::string Resource::get_publication_date() const { return publication_date; }

void Resource::set_availability(bool a) { availability_status = a; }

std::string Resource::get_member_at_front() const { return reservation_queue.front(); }

void Resource::remove_member_from_front() { reservation_queue.pop(); }

void Resource::add_member_to_reservation(const std::string& username) { reservation_queue.push(username); }

bool Resource::is_reservation_queue_empty() const { return reservation_queue.empty(); }

void Resource::set_new_reservation_queue(const std::queue<std::string>& q) { reservation_queue = q; }

void Resource::update_resource(const std::string &t, const std::string &c, const std::string &d, int f)
{
    title = t;
    category = c;
    publication_date = d;
    fine_per_day = f;
}

std::ostream &operator<<(std::ostream &os, const Resource &resource)
{
    os << resource.convert_to_string();
    return os;
}

#pragma once

#include <string>
#include <queue>

class Member;

class Resource {
    protected:  
        std::string resource_id;
        std::string title;
        std::string category;
        std::string publication_date;
        bool availability_status;
        int fine_per_day;
        std::queue<std::string> reservation_queue;

    public:
        Resource() = default;

        virtual ~Resource() = default;
        
        Resource(const std::string& id, const std::string& t, const std::string& c, std::string y, bool a, int f);

        virtual std::string get_resource_details() const;

        virtual std::string get_resource_header() const = 0;

        virtual std::string convert_to_string() const = 0;

        virtual std::string get_creator() const = 0;

        bool is_available() const;

        std::string get_title() const;
        
        std::string get_category() const;

        int get_fine() const;

        std::string get_resource_id() const;

        std::string get_publication_date() const;

        void set_availability(bool a);

        std::string get_member_at_front() const;

        void add_member_to_reservation(const std::string& username);

        void remove_member_from_front();

        bool is_reservation_queue_empty() const;

        void set_new_reservation_queue(const std::queue<std::string>& q);

        friend std::ostream& operator<<(std::ostream& os, const Resource& resource);

        void update_resource(const std::string& t, const std::string& c, const std::string& d, int f);
};
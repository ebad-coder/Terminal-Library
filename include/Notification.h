#pragma once

#include <string>

class Notification {
    private:
        std::string sender;
        std::string receiver;
        std::string title;
        std::string body;
        std::string date;
        bool read_status;
        std::string resource_id;

    public:
        Notification(const std::string& s, const std::string& r, const std::string& t, const std::string& b, const std::string& d, bool status, const std::string& resource_id = "");

        std::string get_notification_details();
        const std::string get_notification_header();

        std::string get_sender() const;
        std::string get_receiver() const;
        std::string get_title() const;
        std::string get_body() const;
        std::string get_date() const;
        std::string get_resource_id() const;

        void set_sender(const std::string& s);
        void set_receiver(const std::string& r);

        bool is_read();
        void mark_as_read();

        const std::string convert_to_string();

        friend std::ostream& operator<<(std::ostream& os, Notification& notification);
};
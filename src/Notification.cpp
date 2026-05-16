#include "../include/Notification.h"


Notification::Notification(const std::string& s, const std::string& r, const std::string& t, const std::string& b, const std::string& d, bool status, const std::string& r_id)
{
    sender = s;
    receiver = r;
    title = t;
    body = b;
    date = d;
    read_status = status;
    resource_id = r_id;
}


std::string Notification::get_notification_details()
{
    std::string details = "\nFrom  : " + sender + "\n";
    details += "Date  : " + date + "\n";
    details += "Title : " + title + "\n\n";
    details += body + "\n";
    return details;
}


const std::string Notification::get_notification_header()
{
    std::string status = is_read() ? "[Read]" : "[Unread]";
    return status + ' ' + title + " from " + sender + " on " + date;
}


std::string Notification::get_sender() const { return sender; }


std::string Notification::get_receiver() const { return receiver; }


std::string Notification::get_title() const { return title; }


std::string Notification::get_body() const { return body; }


std::string Notification::get_date() const { return date; }


bool Notification::is_read() { return read_status; }


void Notification::mark_as_read() { read_status = true; }


const std::string Notification::convert_to_string()
{
    size_t pos = 0;
    while ((pos = body.find("\n", pos)) != std::string::npos)
    {
        body.replace(pos, 1, "[NL]");
        pos += 4;
    }

    return sender + "|" + receiver + "|" + title + "|" + body + "|" + date + "|" + std::to_string(read_status) + "|" + resource_id + "\n";
}


std::string Notification::get_resource_id() const { return resource_id; }


void Notification::set_sender(const std::string &s)
{
    sender = s;
}


void Notification::set_receiver(const std::string &r)
{
    receiver = r;
}


std::ostream &operator<<(std::ostream &os, Notification &notification)
{
    os << notification.convert_to_string();
    return os;
}
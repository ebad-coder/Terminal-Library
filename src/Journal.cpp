#include "../include/Journal.h"

Journal::Journal(const std::string &id, const std::string &t, const std::string &c, const std::string &d, bool s, int f, const std::string &i, const std::string &e, const std::string &p, int v) 
        : Resource(id, t, c, d, s, f), issn(i), editor(e), publisher(p), volume_number(v) {}

std::string Journal::get_resource_details() const
{
    std::string details = "\nResource Details\n";
    details += "Type             : Journal\n";
    details += Resource::get_resource_details();
    details += "ISSN             : " + issn + '\n';
    details += "Editor           : " + editor + '\n';
    details += "Publisher        : " + publisher + '\n';
    details += "Volume Number    : " + std::to_string(volume_number) + '\n';
    return details;
}

std::string Journal::get_resource_header() const
{
    return "(Journal - " + resource_id + ") " + title + " edited by " + editor;
}

std::string Journal::convert_to_string() const
{
    std::string result = resource_id + "|" + title + "|" + category + "|" + publication_date + "|" + std::to_string(availability_status) + "|" + std::to_string(fine_per_day) + "|" + issn + "|" + editor + "|" + publisher + "|" + std::to_string(volume_number);

    std::queue<std::string> temp = reservation_queue;

    while (!temp.empty())
    {
        result += '|' + temp.front();
        temp.pop();
    }

    return result + '\n';
}

std::string Journal::get_creator() const
{
    return editor;
}

std::string Journal::get_issn() const
{
    return issn;
}

std::string Journal::get_editor() const
{
    return editor;
}

std::string Journal::get_publisher() const
{
    return publisher;
}

int Journal::get_volume_number() const
{
    return volume_number;
}

void Journal::update_resource(const std::string &t, const std::string &c, const std::string &d,int f, const std::string &i, const std::string &e, const std::string &p, int v)
{
    Resource::update_resource(t, c, d, f);
    issn = i;
    editor = e;
    publisher = p;
    volume_number = v;
}

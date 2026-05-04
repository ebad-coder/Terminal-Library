#include "../include/Book.h"

Book::Book(const std::string &id, const std::string &t, const std::string &c, const std::string &d, bool s, int f, const std::string &i, const std::string &a, const std::string &p, const std::string &e, const std::string &l) 
        : Resource(id, t, c, d, s, f), isbn(i), author(a), publisher(p), edition(e), language(l) {}

std::string Book::get_resource_details() const
{
    std::string details = "\nResource Details\n";
    details += "Type             : Book\n";
    details += Resource::get_resource_details();
    details += "ISBN             : " + isbn + '\n';
    details += "Author           : " + author + '\n';
    details += "Publisher        : " + publisher + '\n';
    details += "Edition          : " + edition + '\n';
    details += "Language         : " + language + '\n';
    return details;
}

std::string Book::get_resource_header() const
{
    return "(Book - " + resource_id + ") " + title + " by " + author;
}

std::string Book::convert_to_string() const
{
    std::string result =  resource_id + "|" + title + "|" + category + "|" + publication_date + "|" + std::to_string(availability_status) + "|" + std::to_string(fine_per_day) + "|" + isbn + "|" + author + "|" + publisher + "|" + edition + "|" + language;

    std::queue<std::string> temp = reservation_queue;

    while (!temp.empty())
    {
        result += '|' + temp.front();
        temp.pop();
    }

    return result + '\n';
}

std::string Book::get_creator() const
{
    return author;
}

std::string Book::get_isbn() const
{
    return isbn;
}

std::string Book::get_author() const
{
    return author;
}

std::string Book::get_publisher() const {
    return publisher;
}

std::string Book::get_edition() const
{
    return edition;
}

std::string Book::get_language() const
{
    return language;
}

void Book::update_resource(const std::string &t, const std::string &c, const std::string &d, int f, const std::string &i, const std::string &a, const std::string &p, const std::string &e, const std::string &l)
{
    Resource::update_resource(t, c, d, f);
    isbn = i;
    author = a;
    publisher = p;
    edition = e;
    language = l;
}

#include "../include/Article.h"

Article::Article(const std::string &id, const std::string &t, const std::string &c, const std::string &d, bool s, int f, const std::string &j, const std::string &a, const std::string &i) 
            : Resource(id, t, c, d, s, f), journal_name(j), author(a), issue_number(i) {}

std::string Article::get_resource_details() const
{
   std::string details = "\nResource Details\n";
   details += "Type             : Article\n";
   details += Resource::get_resource_details();
   details += "Journal Name     : " + journal_name + '\n';
   details += "Author           : " + author + '\n';
   details += "Issue Number     : " + issue_number + '\n';
   return details;
}

std::string Article::get_resource_header() const
{
    return "(Article - " + resource_id + ") " + title + " by " + author;
}

std::string Article::convert_to_string() const
{
    std::string result = resource_id + "|" + title + "|" + category + "|" + publication_date + "|" + std::to_string(availability_status) + "|" + std::to_string(fine_per_day) + "|" + journal_name + "|" + author + "|" + issue_number;

    std::queue<std::string> temp = reservation_queue;

    while (!temp.empty())
    {
        result += '|' + temp.front();
        temp.pop();
    }

    return result + '\n';
}

std::string Article::get_creator() const
{
    return author;
}

std::string Article::get_journal_name() const
{
    return journal_name;
}

std::string Article::get_author() const
{
    return author;
}

std::string Article::get_issue_number() const
{
    return issue_number;
}

void Article::update_resource(const std::string &t, const std::string &c, const std::string &d, int f, const std::string &j, const std::string &a, const std::string &i)
{
    Resource::update_resource(t, c, d, f);
    journal_name = j;
    author = a;
    issue_number = i;
}
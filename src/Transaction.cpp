#include "../include/Transaction.h"
#include "../include/Date.h"


Transaction::Transaction(const std::string& u, const std::string& r, int d)
{
    username = u;
    resource_id = r;
    issue_date = Date::get_current_date();
    due_date = Date::get_future_date(d);
    returned_date = "";
    active = true;
}


Transaction::Transaction(const std::string &u, const std::string &r, const std::string &i, const std::string &d, const std::string &rd, bool a)
{
    username = u;
    resource_id = r;
    issue_date = i;
    due_date = d;
    returned_date = rd;
    active = a;
}


std::string Transaction::get_transaction_details() const
{
    std::string details = "\nTransaction Details\n";
    details += "Borrower    : " + username + '\n';
    details += "Resource ID : " + resource_id + '\n';
    details += "Issue Date  : " + issue_date + '\n';
    details += "Due Date    : " + due_date + '\n';

    if (!active)
    {
        details += "Returned On : " + returned_date + '\n';
    }

    return details;
}


std::string Transaction::get_issue_date() const
{
    return issue_date;
}


std::string Transaction::get_due_date() const
{
    return due_date;
}


std::string Transaction::get_username() const
{
    return username;
}


std::string Transaction::get_resource_id() const
{
    return resource_id;
}


std::string Transaction::get_returned_date() const
{
    return returned_date;
}


void Transaction::mark_complete() { active = false; }


bool Transaction::is_active() const { return active; }


void Transaction::set_returned_date(const std::string &date) { returned_date = date; }


std::string Transaction::get_transaction_header() const {
    std::string status = is_active() ? "[Active]" : "[Complete]";
    return status + " Borrowed a resource on " + issue_date;
}


void Transaction::set_username(const std::string &u)
{
    username = u;
}


std::string Transaction::convert_to_string() const
{
    return username + "|" + resource_id + "|" + issue_date + "|" + due_date + "|" + returned_date + "|" + std::to_string(active) + '\n'; 
}


std::ostream &operator<<(std::ostream &os, const Transaction &transaction)
{
    os << transaction.convert_to_string();
    return os;
}
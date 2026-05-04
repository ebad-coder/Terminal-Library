#pragma once

#include "Resource.h"

class Article : public Resource {
    private:
        std::string journal_name;
        std::string author;
        std::string issue_number;
        
    public:
        Article(
            const std::string &id, const std::string &t, const std::string &c, const std::string& d, bool s, int f,
            const std::string &j, const std::string &a, const std::string &i
        );
        
        std::string get_resource_details() const override;
        std::string get_resource_header() const override;
        std::string convert_to_string() const override;
        std::string get_creator() const override;

        std::string get_journal_name() const;
        std::string get_author() const;
        std::string get_issue_number() const;

        void update_resource(
            const std::string& t, const std::string& c, const std::string& d, int f,
            const std::string& j, const std::string& a, const std::string& i
        );
};
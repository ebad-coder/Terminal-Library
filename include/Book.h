#pragma once

#include "Resource.h"

class Book : public Resource {
    private:
        std::string isbn;
        std::string author;
        std::string publisher;
        std::string edition;
        std::string language;

    public:
        Book(
            const std::string& id, const std::string& t, const std::string& c, const std::string& d, bool status, int f,
            const std::string& i, const std::string& a, const std::string& p, const std::string& e, const std::string& l
        );

        std::string get_resource_details() const override;
        std::string get_resource_header() const override;
        std::string convert_to_string() const override;
        std::string get_creator() const override;

        std::string get_isbn() const;
        std::string get_author() const;
        std::string get_publisher() const;
        std::string get_edition() const;
        std::string get_language() const;

        void update_resource(
            const std::string& t, const std::string& c, const std::string& d, int f,
            const std::string& i, const std::string& a, const std::string& p, const std::string& e, const std::string& l
        );
};
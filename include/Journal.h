#pragma once

#include "Resource.h"

class Journal : public Resource {
    private:
        std::string issn;
        std::string editor;
        std::string publisher;
        int volume_number;

    public:
        Journal(
            const std::string &id, const std::string &t, const std::string &c, const std::string& d, bool s, int f,
            const std::string &i, const std::string &e, const std::string &p, int v
        );

        std::string get_resource_details() const override;
        std::string get_resource_header() const override;
        std::string convert_to_string() const override;
        std::string get_creator() const override;

        std::string get_issn() const;
        std::string get_editor() const;
        std::string get_publisher() const;
        int get_volume_number() const;

        void update_resource(
            const std::string& t, const std::string& c, const std::string& d, int f,
            const std::string& i, const std::string& e, const std::string& p, int v
        );
};
#pragma once
#include <string>
#include <map>

namespace serialization
{
    struct json_value_t
    {
        /*
         * json value can be a string
         * or a mapping of (string key => json_value_t).
         */
        std::string value_;
        std::map<std::string, json_value_t> mapping_;
    };
    // TODO
} // serialization

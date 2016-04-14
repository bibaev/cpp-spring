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

    struct json_writer_t {
        json_writer_t(json_value_t& json)
            : json_(json) {
        }

        template<class field_type>
        void operator()(field_type& value, const char* key) {
            json_value_t inner;
            write(inner, value);
            json_.mapping_.insert({ std::string(key), inner });
        }

    private:
        json_value_t& json_;
    };

    struct json_reader_t {
        json_reader_t(json_value_t& json)
            : json_(json) {
        }

        template<class field_type>
        void operator()(field_type& value, const char* key) {
            // TODO:
        }

    private:
        json_value_t& json_;
    };

    template<class type>
    typename std::enable_if<std::is_arithmetic<type>::value>::type write(json_value_t& jvalue, type& obj) {
        std::stringstream ss;
        ss << obj;
        jvalue.value_ = ss.str();
    }

    template<class type>
    typename std::enable_if<!std::is_arithmetic<type>::value>::type write(json_value_t& jvalue, type& obj) {
        json_writer_t proc(jvalue);
        reflect_type(proc, obj);
    }

    template<class type>
    typename std::enable_if<std::is_arithmetic<type>::value>::type read(json_value_t& jvalue, type& obj) {

    }

    template<class type>
    typename std::enable_if<!std::is_arithmetic<type>::value>::type read(json_value_t& jvalue, type& obj) {
        
    }
    // TODO
} // serialization

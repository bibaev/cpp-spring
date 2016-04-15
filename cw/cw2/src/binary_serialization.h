#pragma once
#include <type_traits>
namespace serialization
{
    template<class type>
    typename std::enable_if<std::is_pod<type>::value>::type write(std::ostream& os, type& obj);

    template<class type>
    typename std::enable_if<std::is_pod<type>::value>::type read(std::istream& is, type& obj);

    template<class type>
    typename std::enable_if<!std::is_pod<type>::value>::type read(std::istream& is, type& obj); 

    template<class type>
    typename std::enable_if<!std::is_pod<type>::value>::type write(std::ostream& os, type& obj);

    // Task 1.
    struct iserializer_t {
        iserializer_t(std::istream& is)
            : stream(is) {
        }

        template<class field_type>
        void operator()(field_type& value, const char* key) {
            read(stream, value);
        }

    private:
        std::istream& stream;
    };

    struct oserializer_t {
        oserializer_t(std::ostream& is)
            : stream(is) {
        }

        template<class field_type>
        void operator()(field_type& value, const char* key) {
            write(stream, value);
        }

    private:
        std::ostream& stream;
    };

    template<typename T>
    void serialize(...) {
        static_assert(std::is_pod<T>::value, "Type is not serializable");
    }

    template<class type>
    typename std::enable_if<std::is_pod<type>::value>::type write(std::ostream& os, type& obj) {
        os.write(reinterpret_cast<const char*>(&obj), sizeof(type));
    }

    template<class type>
    typename std::enable_if<std::is_pod<type>::value>::type read(std::istream& is, type& obj) {
        is.read(reinterpret_cast<char*>(&obj), sizeof(type));
    }

    template<class type>
    typename std::enable_if<!std::is_pod<type>::value>::type read(std::istream& is, type& obj) {
        iserializer_t proc(is);
        reflect_type(proc, obj);
    }

    template<class type>
    typename std::enable_if<!std::is_pod<type>::value>::type write(std::ostream& os, type& obj) {
        oserializer_t proc(os);
        reflect_type(proc, obj);
    }

} // serialization

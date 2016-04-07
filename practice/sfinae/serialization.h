#include <iostream>

namespace serialization {

    // Task 1
    template<typename T>
    typename std::enable_if<std::is_pod<T>::value>::type serialize(std::ostream& out, T& obj) {
        out.write(reinterpret_cast<const char*>(&obj), sizeof(T));
    }

    template<typename T>
    typename std::enable_if<!std::is_pod<T>::value>::type serialize(std::ostream& out, T& obj) {
        static_assert(std::is_pod<T>::value, "Type is not serializable");
    }

    template<typename T>
    typename std::enable_if<std::is_pod<T>::value>::type deserialize(std::istream& in, T& obj) {
        in.read(reinterpret_cast<char*>(&obj), sizeof(T));
    }

    template<typename T>
    typename std::enable_if<!std::is_pod<T>::value>::type deserialize(std::istream& in, T& obj) {
        static_assert(std::is_pod<T>::value, "Type is not serializable");
    }
}

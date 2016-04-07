#include <iostream>

namespace serialization {

    // Task 1
    template<typename T>
    typename std::enable_if<std::is_pod<T>::value>::type serialize(std::ostream& out, T const& obj) {
        out.write(reinterpret_cast<const char*>(&obj), sizeof(T));
    }

    template<typename T>
    void serialize(...) {
        static_assert(std::is_pod<T>::value, "Type is not serializable");
    }

    template<typename T>
    typename std::enable_if<std::is_pod<T>::value>::type deserialize(std::istream& in, T& obj) {
        in.read(reinterpret_cast<char*>(&obj), sizeof(T));
    }

    template<typename T>
    void deserialize(...) {
        static_assert(std::is_pod<T>::value, "Type is not serializable");
    }

    // Task 2
    template<typename T>
    class is_iterator_contains {
        
        template<typename U>
        static std::true_type  test(typename U::iterator*);

        template<typename>
        static std::false_type test(...);

    public:
        static const bool value = decltype(test<T>(nullptr))::value;
    };

    template<typename T>
    typename std::enable_if<is_iterator_contains<T>::value>::type serialize(std::ostream& out, T const& obj) {
        size_t size = obj.size();
        serialize(out, size);
        for (auto i = obj.begin(); i != obj.end(); ++i) {
            serialize(out, *i);
        }
    }

    template<typename T>
    typename std::enable_if<is_iterator_contains<T>::value>::type deserialize(std::istream& in, T& obj) {
        using value_type = typename T::value_type;
        size_t size;
        deserialize(in, size);
        for (size_t i = 0; i < size; ++i) {
            value_type val;
            deserialize(in, val);
            obj.insert(obj.end(), val);
        }
    }
}

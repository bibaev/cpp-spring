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

    // Task 2, 3
    template<typename T>
    class is_iterator_contains {
        
        template<typename U>
        static std::true_type  test(typename U::iterator*) { return{}; };

        template<typename>
        static std::false_type test(...) { return{}; };

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
        obj.clear();
        for (size_t i = 0; i < size; ++i) {
            value_type val;
            deserialize(in, val);
            obj.insert(obj.end(), std::move(val));
        }
    }

    // Task 4
    template<typename T1, typename T2>
    void serialize(std::ostream& out, std::pair<T1, T2> const& p) {
        typedef typename std::remove_const<T1>::type key_type;

        serialize(out, const_cast<key_type&>(p.first));
        serialize(out, p.second);
    }

    template<typename T1, typename T2>
    void deserialize(std::istream& in, std::pair<T1, T2> &p) {
        typedef typename std::remove_const<T1>::type key_type;
        typedef T2 value_type;

        key_type key;
        value_type value;
        deserialize(in, key);
        deserialize(in, value);
        const_cast<key_type&>(p.first) = key;
        p.second = value;
    }
}

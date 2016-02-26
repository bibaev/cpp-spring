#ifndef LAZY_BASIC_STRING
#define LAZY_BASIC_STRING
#include <iosfwd>
#include <algorithm>
#include <cctype>

namespace std_utils {
    template<class CharT, class Traits = std::char_traits<CharT>>
    class lazy_basic_string {
    public:
        typedef Traits traits_type;
        typedef typename Traits::char_type value_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef value_type& reference;
        typedef value_type const& const_reference;
        typedef value_type* pointer;
        typedef value_type const* const_pointer;
    private:
        class proxy {
            lazy_basic_string& ls_;
            size_type index_;
        public:
            proxy(lazy_basic_string& ls, size_t const ind) : ls_(ls), index_(ind) {}
            char operator=(char x) const {
                ls_.set_at(x, index_);
                delete this;
                return x;
            }

            operator char() const {
                return ls_.get_at(index_);
            }
        }; // proxy
    public:
        lazy_basic_string();
        lazy_basic_string(lazy_basic_string const& other);
        lazy_basic_string(const_pointer c_str);
        lazy_basic_string(value_type ch, size_t count);
        explicit lazy_basic_string(value_type ch);

        ~lazy_basic_string();

        lazy_basic_string& operator=(lazy_basic_string const& other);
        lazy_basic_string& operator=(lazy_basic_string && other);

        lazy_basic_string& operator+(lazy_basic_string const& other);
        lazy_basic_string& operator+(const_pointer other);
        lazy_basic_string& operator+(value_type ch);

        lazy_basic_string& operator+=(lazy_basic_string const& other);
        lazy_basic_string& operator+=(const_pointer other);
        lazy_basic_string& operator+=(value_type ch);

        const_reference operator[](size_type index) const;
        proxy& operator[](size_type index);

        void swap(lazy_basic_string& other);
        void clear();
        size_type size() const;
        bool empty() const;
        const_pointer c_str() const;
        friend class proxy;
    private:
        void set_at(value_type value, size_type index);
        const_reference get_at(size_type index);

        CharT* buffer_;
        size_t* use_count_;
    }; // lazy_basic_string

    template<typename CharT, class Traits>
    void swap(lazy_basic_string<CharT, Traits>& left, lazy_basic_string<CharT, Traits>& right) {
        left.swap(right);
    }

    template<typename CharT, class Traits>
    lazy_basic_string<CharT, Traits>& operator+(typename lazy_basic_string<CharT, Traits>::const_reference left, lazy_basic_string<CharT, Traits>& right) {
        left += right;
        return left;
    }

    template<typename CharT, class Traits>
    lazy_basic_string<CharT, Traits>& operator+(typename lazy_basic_string<CharT, Traits>::const_pointer left, lazy_basic_string<CharT, Traits>& right) {
        left += right;
        return left;
    }

    template<typename CharT, class Traits>
    bool operator==(lazy_basic_string<CharT, Traits> const& left, lazy_basic_string<CharT, Traits> const& right) {
        return Traits::compare(left.c_str(), right.c_str(), std::min(left.size(), right.size())) == 0;
    }

    template<typename CharT, class Traits>
    bool operator!=(lazy_basic_string<CharT, Traits> const& left, lazy_basic_string<CharT, Traits> const& right) {
        return !(left == right);
    }

    template<typename CharT, class Traits>
    bool operator<(lazy_basic_string<CharT, Traits> const& left, lazy_basic_string<CharT, Traits> const& right) {
        return Traits::compare(left.c_str(), right.c_str(), std::min(left.size(), right.size())) < 0;
    }

    template<typename CharT, class Traits>
    bool operator<=(lazy_basic_string<CharT, Traits> const& left, lazy_basic_string<CharT, Traits> const& right) {
        return !(right < left);
    }

    template<typename CharT, class Traits>
    bool operator>(lazy_basic_string<CharT, Traits> const& left, lazy_basic_string<CharT, Traits> const& right) {
        return right < left;
    }

    template<typename CharT, class Traits>
    bool operator>=(lazy_basic_string<CharT, Traits> const& left, lazy_basic_string<CharT, Traits> const& right) {
        return !(left < right);
    }

    template<typename CharT, class Traits>
    bool operator==(typename lazy_basic_string<CharT, Traits>::const_pointer left, lazy_basic_string<CharT, Traits> const& right) {
        return Traits::compare(left, right.c_str(), std::min(Traits::length(left), right.size())) == 0;
    }

    template<typename CharT, class Traits>
    bool operator==(lazy_basic_string<CharT, Traits>const& left, typename lazy_basic_string<CharT, Traits>::const_pointer right) {
        return Traits::compare(left.c_str(), right, std::min(left.size(), Traits::length(right))) == 0;
    }

    template<typename CharT, class Traits>
    bool operator!=(typename lazy_basic_string<CharT, Traits>::const_pointer left, lazy_basic_string<CharT, Traits> const& right) {
        return !(left == right);
    }

    template<typename CharT, class Traits>
    bool operator!=(lazy_basic_string<CharT, Traits>const& left, typename lazy_basic_string<CharT, Traits>::const_pointer right) {
        return !(left == right);
    }

    template<typename CharT, class Traits>
    bool operator<(typename lazy_basic_string<CharT, Traits>::const_pointer left, lazy_basic_string<CharT, Traits> const& right) {
        return Traits::compare(left, right.c_str(), std::min(Traits::length(left), right.size())) < 0;
    }

    template<typename CharT, class Traits>
    bool operator<(lazy_basic_string<CharT, Traits>const& left, typename lazy_basic_string<CharT, Traits>::const_pointer right) {
        return Traits::compare(left.c_str(), right, std::min(left.size(), Traits::length(right))) < 0;
    }

    template<typename CharT, class Traits>
    bool operator<=(typename lazy_basic_string<CharT, Traits>::const_pointer left, lazy_basic_string<CharT, Traits> const& right) {
        return !(right < left);
    }

    template<typename CharT, class Traits>
    bool operator<=(lazy_basic_string<CharT, Traits>const& left, typename lazy_basic_string<CharT, Traits>::const_pointer right) {
        return !(right < left);
    }

    template<typename CharT, class Traits>
    bool operator>(typename lazy_basic_string<CharT, Traits>::const_pointer left, lazy_basic_string<CharT, Traits> const& right) {
        return right < left;
    }

    template<typename CharT, class Traits>
    bool operator>(lazy_basic_string<CharT, Traits>const& left, typename lazy_basic_string<CharT, Traits>::const_pointer right) {
        return right < left;
    }

    template<typename CharT, class Traits>
    bool operator>=(typename lazy_basic_string<CharT, Traits>::const_pointer left, lazy_basic_string<CharT, Traits> const& right) {
        return !(left < right);
    }

    template<typename CharT, class Traits>
    bool operator>=(lazy_basic_string<CharT, Traits>const& left, typename lazy_basic_string<CharT, Traits>::const_pointer right) {
        return !(left < right);
    }

    struct ichar_traits: std::char_traits<char> {
        static bool eq(char left, char right) {
            return tolower(left) == tolower(right);
        }
        
        static bool lt(char left, char right) {
            return tolower(left) == tolower(right);
        }
    };

    typedef lazy_basic_string<char> lazy_string;
    typedef lazy_basic_string<wchar_t> lazy_wstring;
    typedef lazy_basic_string<char, ichar_traits> lazy_istring;
} // std_utils

#endif // LAZY_BASIC_STRING

#ifndef LAZY_STRING
#define LAZY_STRING
#include <iosfwd>
#include <algorithm>
#include <cctype>
#include <memory>

namespace std_utils {
    template<class CharT, class Traits = std::char_traits<CharT>>
    class lazy_basic_string {
    public:
        typedef Traits traits_type;
        typedef typename Traits::char_type value_type;
        typedef size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef value_type& reference;
        typedef value_type const& const_reference;
        typedef value_type* pointer;
        typedef value_type const* const_pointer;
    private:
        class proxy {
            lazy_basic_string& ls_;
            size_type index_;
        public:
            proxy(proxy const& other);
            proxy(lazy_basic_string& ls, size_t index) : ls_(ls), index_(index) {}
            value_type operator=(value_type x) const {
                ls_.set_at(x, index_);
                return x;
            }

            operator value_type() const {
                return ls_.get_at(index_);
            }
        }; // proxy

        class buffer {
        public:
            buffer();
            explicit buffer(size_type size);
            explicit buffer(const_pointer c_str);
            buffer(const_pointer prefix, const_pointer suffix);
            explicit buffer(value_type, size_type);
            buffer(buffer const& other) = default;
            ~buffer();

            pointer get_data();
            const_pointer get_data() const;

            size_type get_size() const;
        private:
            pointer data_;
            size_type size_;
        }; // buffer  
    public:
        lazy_basic_string();
        lazy_basic_string(lazy_basic_string const& other);
        lazy_basic_string(const_pointer c_str);
        lazy_basic_string(value_type ch, size_t count);

        ~lazy_basic_string() = default;

        lazy_basic_string& operator=(lazy_basic_string const& other);
        lazy_basic_string& operator=(lazy_basic_string && other);

        lazy_basic_string operator+(lazy_basic_string const& other);
        lazy_basic_string operator+(const_pointer other);
        lazy_basic_string operator+(value_type ch);

        lazy_basic_string& operator+=(lazy_basic_string const& other);
        lazy_basic_string& operator+=(const_pointer other);
        lazy_basic_string& operator+=(value_type ch);

        const_reference operator[](size_type index) const;
        proxy operator[](size_type index);

        void swap(lazy_basic_string& other);
        void clear();
        size_type size() const;
        bool empty() const;
        const_pointer c_str() const;

#ifndef NDEBUG
        size_t use_count() const {
            return shared_buffer_.use_count();
        }
#endif
    private:
        void set_at(value_type value, size_type index);
        const_reference get_at(size_type index);

        std::shared_ptr<buffer> shared_buffer_;
    }; // lazy_basic_string

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits>::proxy::proxy(proxy const& other)
        : ls_(other.ls_)
        , index_(other.index_)
    {}

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits>::buffer::buffer()
        : data_(new value_type[1] { '\0' } )
        , size_(0)
    {}

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits>::buffer::buffer(size_type size)
        : data_(new value_type[size])
        , size_(size)
    {}

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits>::buffer::buffer(const_pointer c_str)
        : data_(nullptr)
        , size_(Traits::length(c_str)) {
        data_ = new value_type[size_ + 1];
        Traits::copy(data_, c_str, size_);
        data_[size_] = '\0';
    }

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits>::buffer::buffer(const_pointer prefix, const_pointer suffix) {
        auto prefix_size = Traits::length(prefix);
        auto suffix_size = Traits::length(suffix);
        size_ = prefix_size + suffix_size;
        data_ = new value_type[size_ + 1];
        Traits::copy(data_, prefix, prefix_size);
        Traits::copy(data_ + prefix_size, suffix, suffix_size);
        data_[size_] = '\0';
    }

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits>::buffer::buffer(value_type character, size_type count)
        : data_(new value_type[count + 1])
        , size_(count) {
        Traits::assign(data_, count, character);
        data_[size_] = '\0';
    }

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits>::buffer::~buffer() {
        delete[] data_;
    }

    template <class CharT, class Traits>
    typename lazy_basic_string<CharT, Traits>::pointer lazy_basic_string<CharT, Traits>::buffer::get_data() {
        return data_;
    }

    template <class CharT, class Traits>
    typename lazy_basic_string<CharT, Traits>::const_pointer lazy_basic_string<CharT, Traits>::buffer::get_data() const {
        return data_;
    }

    template <class CharT, class Traits>
    typename lazy_basic_string<CharT, Traits>::size_type lazy_basic_string<CharT, Traits>::buffer::get_size() const {
        return size_;
    }

    // lazy_basic_string
    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits>::lazy_basic_string() 
        : shared_buffer_(std::make_shared<buffer>()) 
    {}

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits>::lazy_basic_string(lazy_basic_string const& other)
        : shared_buffer_(other.shared_buffer_)
    {}

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits>::lazy_basic_string(const_pointer c_str)
        : shared_buffer_(std::make_shared<buffer>(c_str))
    {}

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits>::lazy_basic_string(value_type ch, size_t count)
        : shared_buffer_(std::make_shared<buffer>(ch, count)) 
    {}

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits>& lazy_basic_string<CharT, Traits>::operator=(lazy_basic_string const& other) {
        shared_buffer_ = other.shared_buffer_;
        return *this;
    }

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits>& lazy_basic_string<CharT, Traits>::operator=(lazy_basic_string&& other) {
        clear();
        swap(other);
        return *this;
    }

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits> lazy_basic_string<CharT, Traits>::operator+(lazy_basic_string const& other) {
        return lazy_basic_string<CharT, Traits>(*this) += other;
    }

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits> lazy_basic_string<CharT, Traits>::operator+(const_pointer other) {
        return lazy_basic_string<CharT, Traits>(*this) += other;
    }

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits> lazy_basic_string<CharT, Traits>::operator+(value_type ch) {
        return lazy_basic_string<CharT, Traits>(*this) += lazy_basic_string<CharT, Traits>(ch, 1);
    }

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits>& lazy_basic_string<CharT, Traits>::operator+=(lazy_basic_string const& other) {
        return this->operator+=(other.c_str());
    }

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits>& lazy_basic_string<CharT, Traits>::operator+=(const_pointer other) {
        auto other_size = Traits::length(other);
        if(other_size == 0) {
            return *this;
        }

        shared_buffer_ = std::make_shared<buffer>(shared_buffer_->get_data(), other);

        return *this;
    }

    template <class CharT, class Traits>
    lazy_basic_string<CharT, Traits>& lazy_basic_string<CharT, Traits>::operator+=(value_type ch) {
        lazy_basic_string<CharT, Traits> other(ch, 1);
        this->operator+=(other);
        return *this;
    }

    template <class CharT, class Traits>
    typename lazy_basic_string<CharT, Traits>::const_reference lazy_basic_string<CharT, Traits>::operator[](size_type index) const {
        return shared_buffer_->get_data()[index];
    }

    template <class CharT, class Traits>
    typename lazy_basic_string<CharT, Traits>::proxy lazy_basic_string<CharT, Traits>::operator[](size_type index) {
        return proxy(*this, index);
    }

    template <class CharT, class Traits>
    void lazy_basic_string<CharT, Traits>::swap(lazy_basic_string& other) {
        std::swap(shared_buffer_, other.shared_buffer_);
    }

    template <class CharT, class Traits>
    void lazy_basic_string<CharT, Traits>::clear() {
        shared_buffer_ = std::make_shared<buffer>();
    }

    template <class CharT, class Traits>
    typename lazy_basic_string<CharT, Traits>::size_type lazy_basic_string<CharT, Traits>::size() const {
        return shared_buffer_->get_size();
    }

    template <class CharT, class Traits>
    bool lazy_basic_string<CharT, Traits>::empty() const {
        return shared_buffer_->get_size() == 0;
    }

    template <class CharT, class Traits>
    typename lazy_basic_string<CharT, Traits>::const_pointer lazy_basic_string<CharT, Traits>::c_str() const {
        return shared_buffer_->get_data();
    }

    template <class CharT, class Traits>
    void lazy_basic_string<CharT, Traits>::set_at(value_type value, size_type index) {
        if(value != shared_buffer_->get_data()[index]) {
            if(shared_buffer_.use_count() > 1) {
                shared_buffer_ = std::make_shared<buffer>(shared_buffer_->get_data());
            }

            shared_buffer_->get_data()[index] = value;
        }
    }

    template <class CharT, class Traits>
    typename lazy_basic_string<CharT, Traits>::const_reference lazy_basic_string<CharT, Traits>::get_at(size_type index) {
        return shared_buffer_->get_data()[index];
    }

    template<typename CharT, class Traits>
    void swap(lazy_basic_string<CharT, Traits>& left, lazy_basic_string<CharT, Traits>& right) {
        left.swap(right);
    }

    template<typename CharT, class Traits>
    lazy_basic_string<CharT, Traits> operator+(typename lazy_basic_string<CharT, Traits>::const_reference left, lazy_basic_string<CharT, Traits>& right) {
        return lazy_basic_string<CharT, Traits>(left, 1) += right;
    }

    template<typename CharT, class Traits>
    lazy_basic_string<CharT, Traits> operator+(typename lazy_basic_string<CharT, Traits>::const_pointer left, lazy_basic_string<CharT, Traits>& right) {
        return lazy_basic_string<CharT, Traits>(left) += right;
    }

    template<typename CharT, class Traits>
    bool operator==(lazy_basic_string<CharT, Traits> const& left, lazy_basic_string<CharT, Traits> const& right) {
        size_t left_size = left.size();
        size_t right_size = right.size();
        return left_size == right_size &&
            Traits::compare(left.c_str(), right.c_str(), std::min(left_size, right_size)) == 0;
    }

    template<typename CharT, class Traits>
    bool operator!=(lazy_basic_string<CharT, Traits> const& left, lazy_basic_string<CharT, Traits> const& right) {
        return !(left == right);
    }

    template<typename CharT, class Traits>
    bool operator<(lazy_basic_string<CharT, Traits> const& left, lazy_basic_string<CharT, Traits> const& right) {
        size_t left_size = left.size();
        size_t right_size = right.size();
        auto cmp_result = Traits::compare(left.c_str(), right.c_str(), std::min(left_size, right_size));
        return cmp_result < 0 || (cmp_result == 0 && left_size < right_size);
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
        auto left_size = Traits::length(left);
        return left_size == right.size() && Traits::compare(left, right.c_str(), left_size) == 0;
    }

    template<typename CharT, class Traits>
    bool operator==(lazy_basic_string<CharT, Traits>const& left, typename lazy_basic_string<CharT, Traits>::const_pointer right) {
        auto right_size = Traits::length(right);
        return right_size == left.size() && Traits::compare(left.c_str(), right, right_size) == 0;
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
        size_t left_size = Traits::length(left);
        size_t right_size = right.size();
        auto cmp_result = Traits::compare(left, right.c_str(), std::min(left_size, right_size));
        return cmp_result < 0 || (cmp_result == 0 && left_size < right_size);
    }

    template<typename CharT, class Traits>
    bool operator<(lazy_basic_string<CharT, Traits>const& left, typename lazy_basic_string<CharT, Traits>::const_pointer right) {
        size_t left_size = left.size();
        size_t right_size = Traits::length(right);
        auto cmp_result = Traits::compare(left.c_str(), right, std::min(left_size, right_size));
        return cmp_result < 0 || (cmp_result == 0 && left_size < right_size);
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
        static int compare(const char* left, const char* right, size_t count) {
            for (size_t i = 0; i < count; ++i) {
                char l = tolower(left[i]);
                char r = tolower(right[i]);
                if(l != r) {
                    return l < r ? -1 : 1;
                }
            }

            return 0;
        }
    };

    typedef lazy_basic_string<char> lazy_string;
    typedef lazy_basic_string<wchar_t> lazy_wstring;
    typedef lazy_basic_string<char, ichar_traits> lazy_istring;
} // std_utils

#endif // LAZY_STRING

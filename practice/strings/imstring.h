#pragma once
#include <string>
#include <memory>
#include <cstdint>

struct imstring;

struct imstring {
public:
    imstring();
    imstring(const char*);
    imstring(imstring const& other);
    imstring(imstring && other);
    imstring(imstring const& left, imstring const& right);
    imstring& operator=(imstring const& other) = delete;

    size_t size() const;
    const char* c_str() const;

    imstring& operator+=(imstring const& other);
    std::ostream& operator<<(std::ostream & stream) const;
    const char operator[](int ix) const;

    std::string::iterator begin();
    std::string::iterator end();


private:
    std::shared_ptr<std::string> shared_string_;

    bool is_concat;
    imstring left_;
    imstring right_;
};

imstring::imstring()
    : imstring("")
{}

imstring::imstring(const char * c_str)
    : shared_string_(std::make_shared<std::string>(c_str))
    , is_concat(false)
{}

imstring::imstring(const imstring &other)
    : shared_string_(other.shared_string_)
    , is_concat(other.is_concat)
    , left_(other.left_)
    , right_(other.right_)
{}

imstring::imstring(imstring &&other)
{
    shared_string_ = std::move(other.shared_string_);
    is_concat = other.is_concat;
    if (other.is_concat) {
        left_(std::move(other.left_));
        right_(std::move(other.right_));
    }

    other.is_concat = false;
}

imstring::imstring(const imstring &left, const imstring &right)
    : left_(left)
    , right_(right)
    , is_concat(true)
    , shared_string_(nullptr)
{}

size_t imstring::size() const
{
    return shared_string_->size();
}

const char *imstring::c_str() const
{
    return shared_string_->c_str();
}

imstring& imstring::operator+=(const imstring &other)
{
    return *this;
}

std::ostream &imstring::operator<<(std::ostream &stream) const
{
    stream << *shared_string_;
    return stream;
}

const char imstring::operator[](int ix) const
{
    return shared_string_->operator [](ix);
}

std::string::iterator imstring::begin()
{
    return shared_string_->begin();
}

std::string::iterator imstring::end()
{
    return shared_string_->end();
}

imstring& operator+(imstring left, imstring const& right) {
    return left += right;
}

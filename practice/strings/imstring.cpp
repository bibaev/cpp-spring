#include "imstring.h"
#include <cassert>

imstring::buffer::buffer(std::string const& value)
    : is_concat(false)
    , value_(value)
    , left_(nullptr)
    , right_(nullptr)
{}

imstring::buffer::buffer(std::shared_ptr<buffer> left, std::shared_ptr<buffer> right)
    : is_concat(true)
    , value_("")
    , left_(left)
    , right_(right)
{}

size_t imstring::buffer::size() const {
    if(is_concat) {
        return  (left_ ? left_->size() : 0) + 
                (right_ ? right_->size() : 0);
    }
    
    return value_.size();
}

const char* imstring::buffer::c_str() {
    if(is_concat) {
        collapse();
    }

    assert(!is_concat);
    return value_.c_str();
}

char imstring::buffer::get_at(size_t ix) {
    if(is_concat) {
        auto left_size = left_->size();
        if(left_->size() < ix) {
            return left_->get_at(ix);
        }

        return right_->get_at(ix - left_size);
    }
    
    return value_[ix];
}

std::string::iterator imstring::buffer::begin() {
    collapse();
    return value_.begin();
}

std::string::iterator imstring::buffer::end() {
    collapse();
    return value_.end();
}

void imstring::buffer::collapse() {
    if (!is_concat) return;
    std::vector<std::shared_ptr<buffer>> primitive_buffers;
    auto total_size = add_primitive_buffers(primitive_buffers);

    value_.reserve(total_size);
    value_ = "";
    for (auto buffer_ptr : primitive_buffers) {
        value_ += buffer_ptr->value_;
    }

    right_ = left_ = nullptr;
    is_concat = false;
}

size_t imstring::buffer::add_primitive_buffers(std::vector<std::shared_ptr<buffer>>& buffers) const {
    size_t total_size = 0;

    if (left_->is_concat) {
        total_size += left_->add_primitive_buffers(buffers);
    }
    else {
        total_size += left_->size();
        buffers.push_back(left_);
    }

    if(right_->is_concat) {
        total_size += right_->add_primitive_buffers(buffers);
    }
    else {
        total_size += right_->size();
        buffers.push_back(right_);
    }

    return total_size;
}

imstring::imstring()
    : shared_buffer_(std::make_shared<buffer>(""))
{}

imstring::imstring(const char* c_str)
    : shared_buffer_(std::make_shared<buffer>(c_str))
{}

imstring::imstring(imstring const& other)
    : shared_buffer_(other.shared_buffer_)
{}

imstring::imstring(imstring&& other)
    : shared_buffer_(std::move(other.shared_buffer_))
{}

size_t imstring::size() const {
    return shared_buffer_->size();
}

const char* imstring::c_str() const {
    return shared_buffer_->c_str();
}

imstring& imstring::operator+=(imstring const& other) {
    shared_buffer_ = std::make_shared<buffer>(shared_buffer_, other.shared_buffer_);
    return *this;
}

char imstring::operator[](int ix) const {
    return shared_buffer_->get_at(ix);
}

std::string::iterator imstring::begin() const {
    return shared_buffer_->begin();
}

std::string::iterator imstring::end() const {
    return shared_buffer_->end();
}

imstring operator+(imstring left, imstring const& right) {
    return left += right;
}

std::ostream& operator<<(std::ostream& stream, imstring const& imstring) {
    // TODO: make more lazy
    stream << imstring;
    return stream;
}

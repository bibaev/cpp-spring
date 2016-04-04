#pragma once
#include <string>
#include <memory>
#include <vector>

struct imstring;

struct imstring {
public:
    struct iterator : std::iterator<std::bidirectional_iterator_tag, char>{
        // all categories
        iterator(iterator const& other);
        iterator& operator=(iterator const& other);

        iterator& operator++();
        iterator  operator++(int);

        // input iterator
        // TODO: move out-scope
        bool operator==(iterator const& other);
        bool operator!=(iterator const& other);

        char operator*();
        char* operator->();

        // forward iterator
        iterator();

        // bidirectional iterator
        iterator& operator--();
        iterator  operator--(int);
    };

private:
    struct buffer {
        buffer(std::string const& value);

        buffer(std::shared_ptr<buffer> left, std::shared_ptr<buffer> right);

        size_t size() const;
        const char* c_str();
        char get_at(size_t ix);

        std::string::iterator begin();
        std::string::iterator end();

    private:
        void collapse();
        size_t add_primitive_buffers(std::vector<std::shared_ptr<buffer>>& buffers) const;
        
        bool is_concat;
        std::string value_;
        std::shared_ptr<buffer> left_;
        std::shared_ptr<buffer> right_;
    }; // buffer
public:
    imstring();
    imstring(const char*);
    imstring(imstring const& other);
    imstring(imstring && other);
    imstring& operator=(imstring const& other) = delete;

    size_t size() const;
    const char* c_str() const;

    imstring& operator+=(imstring const& other);
    char operator[](int ix) const;

    std::string::iterator begin() const;
    std::string::iterator end() const;


private:
    std::shared_ptr<buffer> shared_buffer_;
};

imstring operator+(imstring left, imstring const& right);
std::ostream& operator<<(std::ostream& ostream, imstring const& imstring);

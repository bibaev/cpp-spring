#pragma once
#include <string>
#include <memory>
#include <vector>
#include <stack>

struct imstring;

struct imstring {
private:
    struct iterator;
    struct buffer {
        buffer(std::string const& value);

        buffer(std::shared_ptr<buffer> left, std::shared_ptr<buffer> right);

        size_t size() const;
        const char* c_str();
        char get_at(size_t ix);

        std::string::iterator begin();
        std::string::iterator end();

        friend struct iterator;
    private:
        void collapse();
        size_t add_primitive_buffers(std::vector<std::shared_ptr<buffer>>& buffers) const;
        
        bool is_concat_;
        std::string value_;
        std::shared_ptr<buffer> left_;
        std::shared_ptr<buffer> right_;
    }; // buffer

public:
    struct iterator : std::iterator<std::bidirectional_iterator_tag, char> {
        explicit iterator(std::shared_ptr<buffer> buffer);

        // all categories
        iterator(iterator const& other) = default;
        iterator& operator=(iterator const& other) = default;

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

    private:
        bool to_next_value();
        void eval_end();
        std::shared_ptr<buffer> root_;

        std::shared_ptr<buffer> current_node_;
        std::string::iterator current_pos_;
        std::string::iterator end_pos_;

        std::stack<std::shared_ptr<buffer>> stack_;
    }; // iterator

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

    iterator _begin() const;
    iterator _end() const;

private:
    std::shared_ptr<buffer> shared_buffer_;
};

imstring operator+(imstring left, imstring const& right);
std::ostream& operator<<(std::ostream& ostream, imstring const& imstring);

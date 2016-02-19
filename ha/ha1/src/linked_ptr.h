#ifndef LINKED_PTR_h
#define LINKED_PTR_h
#include <cstddef>
#include <memory>

namespace smart_ptr {

    template<typename T>
    class linked_ptr {
    private:
        class node {
        public:
            node();

            bool unique() const;
            void insert_after_this(node & other);
            void extract();
            void swap(node & other);
            ~node();
        private:
            node * left_;
            node * right_;
        };

    public:
        linked_ptr();
        explicit linked_ptr(T*);
        explicit linked_ptr(linked_ptr const &);
        template<typename U>
        explicit linked_ptr(U*);
        template<typename U>
        explicit linked_ptr(linked_ptr<U>);

        ~linked_ptr();

        linked_ptr & operator=(linked_ptr const&);
        template<typename U>
        linked_ptr & operator=(linked_ptr<U> const&);

        void reset();
        void reset(T*);
        template<typename U>
        void reset(U*);

        bool unique() const;
        T* get() const;
        void swap(linked_ptr& other);

        T& operator*() const;
        T* operator->() const;

        operator bool() const;

    private:
        mutable node node_;
        T* ptr_;
    };

    template <typename T>
    linked_ptr<T>::node::node()
        : left_(this)
        , right_(this)
    {}

    template <typename T>
    bool linked_ptr<T>::node::unique() const {
        return left_ == this && right_ == this;
    }

    template <typename T>
    void linked_ptr<T>::node::insert_after_this(node& other) {
        other.right_ = right_;
        other.left_ = this;

        right_->left_ = &other;
        right_ = &other;
    }

    template <typename T>
    void linked_ptr<T>::node::extract() {
        left_->right_ = right_;
        right_->left_ = left_;

        left_ = right_ = this;
    }

    template <typename T>
    void linked_ptr<T>::node::swap(node& other) {
        auto left = left_;
        auto right = right_;

        auto other_left = other.left_;
        auto other_right = other.right_;

        left->right_ = &other;
        right->left_ = &other;

        other_right->left_ = this;
        other_left->right_ = this;

        other.left_ = left == this ? &other : left;
        other.right_ = right == this ? &other : right;

        left_ = other_left == &other ? this : other_left;
        right_ = other_right == &other ? this : other_right;
    }

    template <typename T>
    linked_ptr<T>::node::~node() {
        extract();
    }

    template <typename T>
    linked_ptr<T>::linked_ptr()
        : node_()
        , ptr_(nullptr)
    {}

    template <typename T>
    linked_ptr<T>::linked_ptr(T* ptr)
        : node_()
        , ptr_(ptr)
    {}

    template <typename T>
    linked_ptr<T>::linked_ptr(linked_ptr const& other)
        : node_()
        , ptr_(other.get()) {
        other.node_.insert_after_this(node_);
    }

    template <typename T>
    template <typename U>
    linked_ptr<T>::linked_ptr(U* ptr) 
        : node_()
        , ptr_(ptr)
    {}

    template <typename T>
    template <typename U>
    linked_ptr<T>::linked_ptr(linked_ptr<U>) {}

    template <typename T>
    linked_ptr<T>::~linked_ptr() {
        void(sizeof(T));

        if (unique()) {
            delete ptr_;
        }
    }

    template <typename T>
    linked_ptr<T>& linked_ptr<T>::operator=(linked_ptr const& other) {
        linked_ptr<T> tmp(other);
        swap(tmp);
        return *this;
    }

    template <typename T>
    template <typename U>
    linked_ptr<T>& linked_ptr<T>::operator=(linked_ptr<U> const& other) {
        linked_ptr<T> tmp(other);
        swap(tmp);
        return *this;
    }

    template <typename T>
    void linked_ptr<T>::reset() {
        linked_ptr<T> tmp;
        swap(tmp);
    }

    template <typename T>
    void linked_ptr<T>::reset(T* ptr) {
        linked_ptr<T> tmp(ptr);
        swap(tmp);
    }

    template <typename T>
    template <typename U>
    void linked_ptr<T>::reset(U* ptr) {
        linked_ptr<T> tmp(ptr);
        swap(tmp);
    }

    template <typename T>
    bool linked_ptr<T>::unique() const {
        return ptr_ != nullptr && node_.unique();
    }

    template <typename T>
    T* linked_ptr<T>::get() const {
        return ptr_;
    }

    template <typename T>
    void linked_ptr<T>::swap(linked_ptr& other) {
        std::swap(ptr_, other.ptr_);

        node_.swap(other.node_);
    }

    template <typename T>
    T& linked_ptr<T>::operator*() const {
        return *ptr_;
    }

    template <typename T>
    T* linked_ptr<T>::operator->() const {
        return ptr_;
    }

    template <typename T>
    linked_ptr<T>::operator bool() const {
        return ptr_ != nullptr;
    }

    template<typename T, typename U>
    bool operator==(linked_ptr<T> const& left, linked_ptr<U> const& right) {
        return left.get() == right.get();
    }

    template<typename T, typename U>
    bool operator!=(linked_ptr<T> const& left, linked_ptr<U> const& right) {
        return !(left == right);
    }

    template<typename T, typename U>
    bool operator<(linked_ptr<T> const& left, linked_ptr<U> const& right) {
        return std::less<T>()(left.get(), right.get());
    }

    template<typename T, typename U>
    bool operator<=(linked_ptr<T> const& left, linked_ptr<U> const& right) {
        return !(right < left);
    }

    template<typename T, typename U>
    bool operator>(linked_ptr<T> const& left, linked_ptr<U> const& right) {
        return right < left;
    }

    template<typename T, typename U>
    bool operator>=(linked_ptr<T> const& left, linked_ptr<U> const& right) { 
        return !(left < right);
    }

    template< class T >
    bool operator==(linked_ptr<T> const& left, std::nullptr_t right) {
        return !left;
    }

    template< class T >
    bool operator==(nullptr_t left, linked_ptr<T> const& right) {
        return !right;
    }

    template< class T >
    bool operator!=(linked_ptr<T> const& left, std::nullptr_t right) {
        return static_cast<bool>(left);
    }

    template< class T >
    bool operator!=(nullptr_t left, linked_ptr<T> const& right) { 
        return static_cast<bool>(right);
    }

    template< class T >
    bool operator<(linked_ptr<T> const& left, std::nullptr_t right) { 
        return std::less<T>()(left.get(), nullptr);
    }

    template< class T >
    bool operator<(nullptr_t left, linked_ptr<T> const& right) { 
        return std::less<T>()(nullptr, right.get());
    }

    template< class T >
    bool operator>(linked_ptr<T> const& left, std::nullptr_t right) {
        return nullptr < left;
    }

    template< class T >
    bool operator>(nullptr_t left, linked_ptr<T> const& right) { 
        return right < nullptr;
    }

    template< class T >
    bool operator<=(linked_ptr<T> const& left, std::nullptr_t right) { 
        return !(nullptr < left);
    }

    template< class T >
    bool operator<=(nullptr_t left, linked_ptr<T> const& right) { 
        return !(right < nullptr);
    }

    template< class T >
    bool operator>=(linked_ptr<T> const& left, std::nullptr_t right) { 
        return !(left < nullptr);
    }

    template< class T >
    bool operator>=(nullptr_t left, linked_ptr<T> const& right) {
        return !(nullptr < right);
    }

    template<typename T>
    void swap(linked_ptr<T>& left, linked_ptr<T>& right) {
        left.swap(right);
    }
} // smart_ptr

#endif

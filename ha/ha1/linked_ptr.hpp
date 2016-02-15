#ifndef LINKED_PTR_h
#define LINKED_PTR_h

namespace smart_ptr {

    template<typename T>
    class linked_ptr {
    public:
        linked_ptr();
        explicit linked_ptr(T*);
        template<typename U>
        explicit linked_ptr(U*);
        template<typename U>
        explicit linked_ptr(linked_ptr<U>);

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
        T* ptr_;
    }; // linked_ptr

    template<typename T>
    bool operator==(linked_ptr<T> const& left, linked_ptr<T> const& right);
    
    template<typename T>
    bool operator!=(linked_ptr<T> const& left, linked_ptr<T> const& right);

    template<typename T>
    bool operator<(linked_ptr<T> const& left, linked_ptr<T> const& right);

    template<typename T>
    bool operator<=(linked_ptr<T> const& left, linked_ptr<T> const& right);

    template<typename T>
    bool operator>(linked_ptr<T> const& left, linked_ptr<T> const& right);

    template<typename T>
    bool operator>=(linked_ptr<T> const& left, linked_ptr<T> const& right);

    template<typename T>
    void swap(linked_ptr<T>& left, linked_ptr<T>& right);
} // smart_ptr

#endif

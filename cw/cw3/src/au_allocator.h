#include <vector>
#include <list>
#include <stdexcept>
#include <cassert>
const size_t OS_ALLOC_SIZE = 4096;

struct au_allocator {
    explicit au_allocator(size_t max_order = 7);
    ~au_allocator();
    void* allocate(size_t size);
    void deallocate(void* ptr, size_t size);

    template<typename T, typename... ARGS>
    T* allocate(ARGS&&... args);

    template<typename T>
    void deallocate(T* const ptr);

#ifndef NDEBUG
    static void get_order_test() {
        assert(get_order(1) == 0);
        assert(get_order(3) == 2);
        assert(get_order(2) == 1);
        assert(get_order(4) == 2);
        assert(get_order(5) == 3);
    }
#endif

private:

    static size_t get_order(size_t size) {
        auto res = static_cast<size_t>(log2(size));
        if(size != pow(2, res)) {
            ++res;
        }

        return res;
    }

    void allocate_new_block(size_t order) {
        auto block = new char[OS_ALLOC_SIZE];
        buffers_.push_back(block);
        size_t size = 1 << order;
        size_t offset = 0;
        while(offset < OS_ALLOC_SIZE) {
            free_[order].push_back(block + offset);
            offset += size;
        }
    }

    size_t order_;
    std::vector<void*> buffers_;
    std::vector<std::list<void*>> free_;
};

template <typename T, typename ... ARGS>
T* au_allocator::allocate(ARGS&&... args) {
    T* ptr = reinterpret_cast<T*>(allocate(sizeof(T)));
    new (ptr) T(std::forward<ARGS>(args)...);
    return ptr;
}

template <typename T>
void au_allocator::deallocate(T* const ptr) {
    ptr->~T();
    deallocate(ptr, sizeof(T));
}

inline au_allocator::au_allocator(size_t max_order) 
    : order_(max_order)
    , free_(max_order) {
    if(pow(2, max_order) > OS_ALLOC_SIZE) {
        throw std::logic_error("2^(max_order-1) > N");
    }
}

inline au_allocator::~au_allocator() {
    for(auto buffer : buffers_) {
        delete[] reinterpret_cast<char*>(buffer);
    }
}

inline void* au_allocator::allocate(size_t size) {
    if(size > pow(2, order_)) {
        return new char[size];
    }

    auto order = get_order(size);

    if(free_[order].empty()) {
        allocate_new_block(order);
    }

    auto res = free_[order].front();
    free_[order].pop_front();
    return res;
}

inline void au_allocator::deallocate(void* ptr, size_t size) {
    if(size > pow(2, order_)) {
        delete[] reinterpret_cast<char*>(ptr);
    }
    else {
        auto order = get_order(size);
        free_[order].push_back(ptr);
    }
}

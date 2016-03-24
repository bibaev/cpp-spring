#include <cstddef>
#include <cstdlib>
#include <new>
#include <iostream>
#include <ctime>

struct fault_distribution
{
    fault_distribution()
    {
        srand(time(NULL));
        //srand(0); // makes faults reproduction stable
    }

    bool should_fault()
    {
        return rand() < (RAND_MAX / 60); // 1/60 chance to fail
        //return false; // always fail
    }
};

void* operator new(std::size_t count)
{
    static fault_distribution fdistr;

    if (fdistr.should_fault())
    {
        //std::cerr << "injecting allocation fault" << std::endl;
        //throw std::bad_alloc();
    }

    return malloc(count);
}

void operator delete(void* ptr)
{
    free(ptr);
}

#include "fn.h"
#include <iostream>
#define ENABLE

#if defined(ENABLE)

int my_testt_start() {
    std::cout << "my tests - Ok!" << std::endl;
    return 0;
}

#else

int my_testt_start() {
    std::cout << "my tests disabled!" << std::endl;
    return 0;
}
#endif
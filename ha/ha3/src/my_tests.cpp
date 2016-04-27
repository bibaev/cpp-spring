#include "fn.h"
#include <iostream>
#include <cassert>
#define ENABLE

#if defined(ENABLE)

using namespace fn;
int sum(int a, int b, int c) {
    return a + b;
}

void bind_1_test() {
    //assert(fn::bind(sum, 1, 4, 3)(54) == 5);
}

void ignore_args_tests() {
    assert(fn::bind(sum, 1, _2, 3)(1, 2) == 3);
}

void functor_tests() {
    struct my_functor {
        long operator()(int a, long b) const {
            return a + b;
        }
    };

    assert(5 == fn::bind(my_functor(), fn::_1, 2)(3));
}

void lambda_tests() {
    //assert(6 == bind([](int a, int b, int c) ->int { return a + b + c; }, 1, _1, _2)(1, 4));
}

void succ1(int& a, int val) {
    a += val;
}


void fun_ref_change_tests() {
    auto a = 0;
    auto succ = [](int & i, int val) -> void { i += val; };
    auto f = function<void(int&, int)>(succ);
    f(a, 2);
    assert(2 == a);
}

int my_testt_start() {
    // bind tests
    bind_1_test();
    functor_tests();
    lambda_tests();

    // function tests
    fun_ref_change_tests();
    std::cout << "my tests - Ok!" << std::endl;
    return 0;
}

#else

int my_testt_start() {
    std::cout << "my tests disabled!" << std::endl;
    return 0;
}
#endif
#include "fn.h"
#include <iostream>
#include <vector>
#include <cassert>
#define ENABLE

#if defined(ENABLE)

using namespace fn;
int sum(int a, int b, int c) {
    return a + b;
}

void bind_1_test() {
    assert(fn::bind(sum, 1, 4, 3)(54) == 5);
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

void succ(int& a) {
    ++a;
}

void ref_bind_tests() {
    int a = 0;
    auto binder = fn::bind(succ, _1);
    binder(a);
    assert(1 == a);
}

void extra_args_tests() {
    int a = 0;
    auto binder = fn::bind(succ, _1);
    binder(a, 3, 5);
    assert(1 == a);
}

int mean(int a, int b, int c) {
    return (a + b + c) / 3;
}

void one_two_arg_tests() {
    auto binder = fn::bind(mean, _1, _1, _1);
    assert(5 == binder(5));
    assert(5 == binder(5, 10));
    auto binder2 = fn::bind(mean, _2, _3, _2);
    assert(8 == binder2(1000, 12, 0));
}

void cp_binder() {
    int a = 0;
    auto b1 = fn::bind(succ, _1);
    b1(a);
    assert(1 == a);
    auto b2(b1);
    b2(a);
    assert(2 == a);
}

void assign_binder() {
    int a = 0;
    auto b1 = fn::bind(succ, _1);
    b1(a);
    assert(1 == a);
    auto b2(fn::bind(succ, _1));
    b2(a, a);
    assert(2 == a);
    b2 = b1;
    b2(a);
    assert(3 == a);
}

int vector_sum(std::vector<int> const& v) {
    int res = 0;
    for (int val : v) {
        res += val;
    }

    return res;
}

void move_binder() {
    std::vector<int> v = { 1, 2, 3, 4 };
    auto b1(fn::bind(vector_sum, _1));
    assert(10 == b1(v));
    auto const v2(v);
    assert(10 == b1(v2));

    auto b2(std::move(b1));
    assert(10 == b1(v2));

    auto b3(fn::bind(vector_sum, v2));
    auto b4(std::move(b3));
    assert(10 == b4());
    // assert(10 != b3());
}

void lambda_tests() {
    assert(6 == bind([](int a, int b, int c) ->int { return a + b + c; }, 1, _1, _2)(1, 4));
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
    ref_bind_tests();
    extra_args_tests();
    one_two_arg_tests();
    cp_binder();
    assign_binder();
    move_binder();

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
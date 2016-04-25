#pragma once
#include <tuple>
#include <utility>
namespace {
    template<typename FUN, typename ARG, typename... ARGS>
    struct binder {
        binder(FUN function, ARGS... arg);
    private:
        FUN function_;
    };

    template<typename FUN, typename ARG, typename ... ARGS>
    binder<FUN, ARG, ARGS...>::binder(FUN function, ARGS... arg) {}
}

namespace fn {
    template<int N>
    struct placeholder{};

    const placeholder<1> _1{};
    const placeholder<2> _2{};

    template<typename FUN, typename... ARGS>
    binder<FUN, ARGS ...> bind(FUN func, ARGS... args) {
        return binder<FUN, ARGS...>(func, args...);
    }
}

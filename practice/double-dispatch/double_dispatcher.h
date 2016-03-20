#include <functional>
#include <utility>
#include <map>
#include <typeinfo>
#include <typeindex>

template<typename BASE_CLASS, typename TAG = void>
class double_dispatcher {
public:
    template<typename ARG1, typename ARG2>
    static void reg(std::function<void(ARG1&, ARG2&)>);

    template<typename ARG1, typename ARG2>
    static void reg(void (*func)(ARG1&, ARG2&));

    static void call(BASE_CLASS&, BASE_CLASS&);

private:
    typedef std::pair<std::type_index, std::type_index> type_ix_pair_type;
    typedef std::function<void(BASE_CLASS&, BASE_CLASS&)> stored_func_type;
    typedef std::map<type_ix_pair_type, stored_func_type> dd_map_type;

    static dd_map_type dd_map;
};

template<typename BASE_CLASS, typename TAG>
template<typename ARG1, typename ARG2>
void double_dispatcher<BASE_CLASS, TAG>::reg(std::function<void(ARG1&, ARG2&)> func) {
    type_ix_pair_type tp(typeid(ARG1), typeid(ARG2));
    dd_map[tp] = [func](BASE_CLASS &arg1, BASE_CLASS &arg2) {
        func(dynamic_cast<ARG1&>(arg1), dynamic_cast<ARG2&>(arg2));
    };

    type_ix_pair_type tp_rev(typeid(ARG2), typeid(ARG1));
    dd_map[tp_rev] = [func](BASE_CLASS &arg2, BASE_CLASS &arg1) {
        func(dynamic_cast<ARG1&>(arg1), dynamic_cast<ARG2&>(arg2));
    };
}

template<typename BASE_CLASS, typename TAG>
template<typename ARG1, typename ARG2>
void double_dispatcher<BASE_CLASS, TAG>::reg(void(* func)(ARG1&, ARG2&)) {
    reg<ARG1, ARG2>(std::function<void(ARG1&, ARG2&)>(func));
}

template<typename BASE_CLASS, typename TAG>
void double_dispatcher<BASE_CLASS, TAG>::call(BASE_CLASS& arg1, BASE_CLASS& arg2) {
    type_ix_pair_type tp(typeid(arg1), typeid(arg2));
    typename dd_map_type::iterator value = dd_map.find(tp);
    if(value == dd_map.end()) {
        throw std::bad_cast();
    }

    value->second(arg1, arg2);
}

template<typename BASE_TYPE, typename TAG>
typename double_dispatcher<BASE_TYPE, TAG>::dd_map_type
double_dispatcher<BASE_TYPE, TAG>::dd_map;

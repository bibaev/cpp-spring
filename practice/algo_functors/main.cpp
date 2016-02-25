#include <type_traits>
#include <algorithm>
#include <cassert>
#include <vector>
#include <iostream>

template<typename it>
using ivt = typename std::iterator_traits<it>::value_type;

template<typename container>
using cvt = typename container::value_type;

template<typename fwd_it, typename cmp>
size_t elem_num_in_sorted(fwd_it begin, fwd_it end, 
        typename std::iterator_traits<fwd_it>::value_type value, 
        cmp comparer = std::less<typename std::iterator_traits<fwd_it>::value_type>()) {
    auto it_lower = std::lower_bound(begin, end, value, comparer);
    auto it_upper = std::upper_bound(begin, end, value, comparer);
    return std::distance(it_lower, it_upper);
}

template<typename container, typename cmp = std::less<typename container::value_type>>
typename container::iterator set_add(container & set, typename container::value_type value, cmp comparer = cmp()) {
    auto pos = std::lower_bound(set.begin(), set.end(), value, comparer);
    if (pos != set.end() && *pos != value) {
        return set.insert(pos, value);
    }

    return set.end();
}

template<typename container, typename pred>
void erase_if(container & set, pred predicate) {
    auto after_move = std::remove_if(set.begin(), set.end(), predicate);
    set.erase(after_move, set.end());
}

template<typename it>
void merge_sort(it begin, it end) {
    
}

template<typename it>
void heap_sort(it begin, it end) {
    std::make_heap(begin, end);
}
template<typename Arg>
struct inputed_filter : std::unary_function<Arg, bool> {
    Res operator()(Arg const & arg) {
        if (values_.size() != 5) {
            std::cout << "Enter 5 values:" << std::endl;
            while(values_.size() != 5) {
                
            }
            return true;
        }
        return {};
    }

private:
    std::vector<int> values_;
};

void tests() {
    int sorted[] = { 1, 2, 3, 3, 4, 5, 6, 7, 7, 7, 7 };
    assert(elem_num_in_sorted(sorted, sorted + 11, 2, std::less<int>()) == 1);
    assert(elem_num_in_sorted(sorted, sorted + 11, 3, std::less<int>()) == 2);
    assert(elem_num_in_sorted(sorted, sorted + 11, 7, std::less<int>()) == 4);

    std::vector<int> vect = { 1, 2, 3, 4, 5, 6, 9 };
    assert(*set_add(vect, 7) == 7);
    assert(set_add(vect, 7) == vect.end());
    assert(set_add(vect, 1) == vect.end());
    assert(*set_add(vect, 8) == 8);
    assert(vect[6] == 7 && vect[7] == 8);

    std::vector<int> vect2 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    erase_if(vect2, [](int val) { return val > 3; });
    assert(vect2.size() == 3);
    assert(vect2[2] == 3);
}

int main() {
    tests();
}

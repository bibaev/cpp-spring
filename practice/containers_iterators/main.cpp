#include <vector>
#include <iostream>
#include <set>
#include <list>
#include <unordered_set>
#include <cassert>

template<typename T>
void print(T begin, T end) {
	while (begin != end) {
		std::cout << *begin << " ";
		++begin;
	}

	std::cout << std::endl;
}

template<typename T, class cmp = std::less<typename T::value_type>>
void tree_sort(T begin, T end) {
	typedef typename std::iterator_traits<T>::value_type it_type;
	std::multiset<it_type, cmp> set;
	auto it = begin;
	while(it != end) {
		set.insert(*it);
		++it;
	}
	it = begin;
	for (auto iter = set.begin(); iter != set.end(); ++iter) {
		*it = *iter;
		++it;
	}
}

template<typename T>
void remove_duplicates(std::list<T> & lst) {
	std::unordered_set<T> set;
	auto end = lst.end();
	auto it = lst.begin();
	while (it != end) {
		if(set.insert(*it).second == false)
			lst.erase(it++);
		else
			++it;
	}
}

template<typename T>
bool is_palindrome(T const begin, T const end) {
	auto reverse = std::reverse_iterator<T>(end);
	auto it = begin;
	while (it != end) {
		if (*it != *reverse)
			return false;
		++it;
		++reverse;
	}

	return true;
}

void print_tests() {
	std::vector<int> vector = { 1, 2, 4, 6 };
	print(vector.begin(), vector.end());
}

void tree_sort_tests() {
	std::vector<int> vector = { 5, 7, 4, 5, 8 };
	tree_sort(vector.begin(), vector.end());
	assert(vector[0] == 4);
	print(vector.begin(), vector.end());
}

void remove_duplicates_tests() {
	std::list<int> lst = { 1,1,1,1,3,4,5,5 };
	remove_duplicates(lst);
	print(lst.begin(), lst.end());
	assert(lst.size() == 4);
}

void is_palindrome_tests() {
	std::string str1 = "abbbbbba";
	std::string str2 = "abbbbbbb";
	assert(is_palindrome(str1.cbegin(), str1.cend()));
	assert(!is_palindrome(str2.cbegin(), str2.cend()));
}

int main() {
	print_tests();
	tree_sort_tests();
	remove_duplicates_tests();
	is_palindrome_tests();
}

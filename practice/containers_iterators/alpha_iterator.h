#pragma once

template<typename FWD_IT>
struct alpha_iterator : std::iterator<std::forward_iterator_tag, 
		typename std::iterator_traits<FWD_IT>::value_type> {
	
	using base = std::iterator < std::forward_iterator_tag,
		typename std::iterator_traits<FWD_IT>::value_type >;

	alpha_iterator(FWD_IT begin, FWD_IT end)
		: current_(begin)
		, end_(end)
	{
		if (!isalpha(*begin))
			to_next_letter();
	}

	alpha_iterator(FWD_IT end)
		: current_(end)
		, end_(end) {
	}

	alpha_iterator() = delete;
	alpha_iterator(alpha_iterator const & other) = default;
	alpha_iterator & operator=(alpha_iterator const & other) = default;

	alpha_iterator operator++();
	alpha_iterator operator++(int);

	typename base::reference operator*() { return *current_; }
	typename base::pointer   operator->() { return &(*current_); }

	bool operator==(alpha_iterator const & other);
	bool operator!=(alpha_iterator const & other);
private:
	FWD_IT current_;
	FWD_IT end_;

	void to_next_letter();
};

template <typename FWD_IT>
alpha_iterator<FWD_IT> alpha_iterator<FWD_IT>::operator++() {
	to_next_letter();
	return current_;
}

template <typename FWD_IT>
alpha_iterator<FWD_IT> alpha_iterator<FWD_IT>::operator++(int) {
	auto cp = current_;
	to_next_letter();
	return cp;
}

template <typename FWD_IT>
bool alpha_iterator<FWD_IT>::operator==(alpha_iterator const & other)
{
	return other.current_ == current_;
}

template <typename FWD_IT>
bool alpha_iterator<FWD_IT>::operator!=(alpha_iterator const& other)
{
	return !this->operator==(other);
}

template <typename FWD_IT>
void alpha_iterator<FWD_IT>::to_next_letter()
{
	while (++current_ != end_) {
		if (isalpha(*current_))
			break;
	}
}

template<typename T>
alpha_iterator<T> make_alpha_iterator(T begin, T end){
	return alpha_iterator<T>(begin, end);
}

template<typename T>
alpha_iterator<T> make_alpha_iterator(T end) {
	return alpha_iterator<T>(end);
}

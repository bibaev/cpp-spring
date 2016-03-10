#pragma once
#include <stdlib.h>
#include <queue>
#include <functional>
#include <iostream>

template<typename PRIOR=size_t, typename COMP=std::less<PRIOR>>
struct priority_task_queue
{
private:
	typedef std::function<void()> function;
	typedef std::pair<std::function<void()>, PRIOR> prior_pair;
	class comparer {
	public:
		comparer(COMP const & comp) 
			: comparator_(comp){
				
			}
	
		bool operator()(prior_pair const& left, prior_pair const& right) {
			return comparator_(left.second, right.second);
		}
	private:
		COMP comparator_;
	};
public:
	
	priority_task_queue(COMP const& cmp = COMP()) 
		: queue_(std::priority_queue<std::pair<function, PRIOR>, std::vector<std::pair<function, PRIOR>>, comparer>(comparer(cmp)))	
	{
		
	}
	priority_task_queue(ordered_task_queue && other) {
		std::swap(other.queue_, queue_);
	}
	priority_task_queue(priority_task_queue const& other) = delete;
	priority_task_queue& operator=(priority_task_queue const& other) = delete;
	void push(function const& task, PRIOR priority){
		queue_.push(std::make_pair(task, priority));
	}
	size_t run_one() {
		size_t result = 0;
		if(!queue_.empty()) {
			queue_.top().first();
			queue_.pop();
			++result;
		}

		return result;
	}
	size_t run() {
		size_t result = 0;
		while(run_one() != 0) {
			++result;
		}

		return result;
	}
	bool empty() const {
		return queue_.empty();
	}

private:
	std::priority_queue<std::pair<function, PRIOR>, std::vector<std::pair<function, PRIOR>>, comparer> queue_;
};

template<class fwd_it>
void sleep_sort(fwd_it begin, fwd_it end) {
	auto it = begin;
	priority_task_queue<typename fwd_it::value_type> queue{};
	while(begin != end) {
		auto value = *begin;
		queue.push([&it, value]() { *it = value; ++it; }, *begin); ++begin;
	}
	
	queue.run();
}
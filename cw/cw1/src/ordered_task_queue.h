#pragma once
#include <stdlib.h>
#include <queue>
#include <functional>

struct ordered_task_queue
{
	typedef std::function<void()> function;
	ordered_task_queue() = default;
	ordered_task_queue(ordered_task_queue && other) {
		std::swap(other.queue_, queue_);
	}
	ordered_task_queue(ordered_task_queue const& other) = delete;
	ordered_task_queue& operator=(ordered_task_queue const& other) = delete;
	void push(function const& task){
		queue_.push(task);
	}
	size_t run_one() {
		size_t result = 0;
		if(!queue_.empty()) {
			queue_.front()();
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
	std::queue<function> queue_;
};
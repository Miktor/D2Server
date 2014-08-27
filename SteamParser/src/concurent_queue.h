#ifndef concurent_queue_h__
#define concurent_queue_h__

#include <queue>
#include <condition_variable>

template<typename Data>
class concurrent_queue
{
private:
	std::queue<Data> the_queue;
	mutable std::mutex the_mutex;
	std::condition_variable the_condition_variable;

public:

	void push(Data const& data)
	{
		the_mutex.lock();

		the_queue.push(data);

		the_mutex.unlock();

		the_condition_variable.notify_one();
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lock(the_mutex);
		return the_queue.empty();
	}

	bool try_pop(Data& popped_value)
	{
		std::lock_guard<std::mutex> lock(the_mutex);
		if (the_queue.empty())
		{
			return false;
		}

		popped_value = the_queue.front();
		the_queue.pop();
		return true;
	}

	void wait_and_pop(Data& popped_value)
	{
		std::lock_guard<std::mutex> lock(the_mutex);
		while (the_queue.empty())
		{
			the_condition_variable.wait(lock);
		}

		popped_value = the_queue.front();
		the_queue.pop();
	}

};
#endif // concurent_queue_h__
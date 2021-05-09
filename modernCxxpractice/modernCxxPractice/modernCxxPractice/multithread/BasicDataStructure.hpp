#ifndef __BASICDATASTURCTURE_H__
#define __BASICDATASTURCTURE_H__
#include<queue>
#include <optional>
namespace paraalgorithm
{

	struct empty_stack : std::exception
	{
		const char* what() const throw() { return "empty stack"; }
	};

	template<typename T>
	class threadsafe_stack
	{
	 	public:
		// 		typedef  T task_type;
		typedef std::optional<T> optionalT;
	public:
		threadsafe_stack() {}
		threadsafe_stack(const threadsafe_stack& other)
		{
			std::lock_guard<std::mutex> lock(other.m);
			data = other.data;
		}
		threadsafe_stack& operator=(const threadsafe_stack&) = delete;
		void push(T new_value)
		{
			std::lock_guard<std::mutex> lock(m);
			data.push(new_value);
		}
		optionalT pop()
		{

			std::lock_guard<std::mutex> lock(m);
			if (data.empty()) 	return {};
				//throw empty_stack();
			//std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
			T res = data.top();
			data.pop();
			return optionalT(res);
		}
		void pop(T& value)
		{
			std::lock_guard<std::mutex> lock(m);
			if (data.empty())
				throw empty_stack();
			value = data.top();
			data.pop();
		}
		bool try_pop(T& value)
		{
			std::lock_guard<std::mutex> lock(m);
			if (data.empty())
				return false;
			value = data.top();
			data.pop();
			return true;
		}
		bool empty() const
		{
			std::lock_guard<std::mutex> lock(m);
			return data.empty();
		}
	private:
		std::stack<T> data;
		mutable std::mutex m;
	};


	template<typename T>
	class threadsafe_queue
	{
	
	public:
		threadsafe_queue()
		{}
		threadsafe_queue(threadsafe_queue const& other)
		{
			std::lock_guard<std::mutex> lk(other.mut);
			data_queue = other.data_queue;
		}
		void push(T new_value)
		{
			std::lock_guard<std::mutex> lk(mut);
			data_queue.push(new_value);
			data_cond.notify_one();
		}
		void wait_and_pop(T& value)
		{
			std::unique_lock<std::mutex> lk(mut);
			data_cond.wait(lk, [this] {return !data_queue.empty(); });
			value = data_queue.front();
			data_queue.pop();
		}
		std::shared_ptr<T> wait_and_pop()
		{
			std::unique_lock<std::mutex> lk(mut);
			data_cond.wait(lk, [this] {return !data_queue.empty(); });
			std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
			data_queue.pop();
			return res;
		}
		bool try_pop(T & value)
		{
			std::lock_guard<std::mutex> lk(mut);
			if (data_queue.empty())
				return false;
			value = data_queue.front();
			data_queue.pop();
			return true;
		}
		std::shared_ptr<T> try_pop()
		{
			std::lock_guard<std::mutex> lk(mut);
			if (data_queue.empty())
				return std::shared_ptr<T>();
			std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
			data_queue.pop();
			return res;
		}
		bool empty() const
		{
			std::lock_guard<std::mutex> lk(mut);
			return data_queue.empty();
		}
	private:
		mutable std::mutex mut;
		std::queue<T> data_queue;
		std::condition_variable data_cond;
	};


	template<typename F, typename A>
	std::future<typename std::result_of<F(A&&)>::type>
		spawn_task(F&& f, A&& a)
	{
		typedef std::result_of<F(A&&)>::type result_type;
		std::packaged_task<result_type(A&&)>
			task(std::move(f));
			std::future<result_type> res(task.get_future());
			std::thread t(std::move(task), std::move(a));
			t.detach();
			return res;
	}

}//end of paraalgorithm

#endif //end of __BASICDATASTURCTURE_H__

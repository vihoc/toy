//自製綫程池
//author:vihoc

#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <stack>
#include <iostream>
#include "utilities.hpp"
#include "parallel_accumulate.h"
//#define THREADPOOL_DEBUG

namespace paraalgorithm
{

	struct empty_stack : std::exception
	{
		const char* what() const throw() { return "empty stack"; }
	};
	struct destuctahead : std::exception
	{
		const char* what() const throw() { return "hey, there's plenty data need to execute!"; }
	};

	//綫程池工作内容
	//繼承此類override runtask()
	//runtask為綫程池運行内容
	//moveable not copyable
	//TODO 添加綫程工作完成後回調
	class task
	{
	public:
		explicit task():finished(false) {}
		task(const task&) = delete;
		task operator=(const task&) = delete;
		task(const task&& rhs) {}
	public:
		virtual void runtask() = 0;
	public:
		void finish() { finished = true; }
		auto isfinished() { return finished; }
	private:
		bool finished;
	};
	//綫程池内部使用棧
	//使用指針請make_shared之後傳遞

	template<typename T>
	class threadsafe_stack
	{
// 	public:
// 		typedef  T task_type;
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
		T pop()
		{
			std::lock_guard<std::mutex> lock(m);
			if (data.empty()) throw empty_stack();
			//std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
			T res = data.top();
			data.pop();
			return res;
		}
		void pop(T& value)
		{
			std::lock_guard<std::mutex> lock(m);
			if (data.empty())
				throw empty_stack();
			value = data.top();
			data.pop();
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

	//綫程池
	//本綫程池無法保證工作綫程中處理的數據的綫程安全,請在傳進來的task中自行保證
	class ThreadPool
	{
	public: 
		//初始化請使用
		//ThreadPool::uint32 threadnum = 10; 
		//ThreadPool pool(threadnum);
		//或者不帶參數初始化.
		typedef std::uint_fast32_t uint32;
	public:
		explicit ThreadPool(uint32 InitmaxThread = std::thread::hardware_concurrency() - 1) 
		{
			stoped = false;
			idleThreadNumber = InitmaxThread;
			pool.reserve(InitmaxThread);
			for (uint32 index = 0; index < InitmaxThread; ++index)
			{
				pool.emplace_back
				(
					[this]()
					{
						while (!this->stoped)
						{
							//如果任務隊列為空, 則釋放時間片.
							if (taskqueue.empty()) std::this_thread::yield();
#ifdef THREADPOOL_DEBUG
							entryThread();

#endif //end of #if THREADPOOL_DEBUG
							++idleThreadNumber;
							//獲取隊列中的一個元素
							try
							{
								auto curtask = taskqueue.pop();
								curtask->runtask();
							}
							catch (std::exception e) {
								if("empty stack" == e.what())  std::this_thread::yield();;
							}
							--idleThreadNumber;
#ifdef THREADPOOL_DEBUG
							exitThread();

#endif //end of #if THREADPOOL_DEBUG
						}
					}
				);
			}
		}
		~ThreadPool()
		{
#ifdef THREADPOOL_DEBUG
			if (!taskqueue.empty()) throw destuctahead();

#endif //end of #if THREADPOOL_DEBUG
			
			stoped = true;
			for (auto& worker : pool)
			{
				if (worker.joinable()) worker.join();
			}
		}
		inline uint32 getIdleThreadNumber() { return idleThreadNumber; }

		void commit(std::shared_ptr<task>& request)
		{
			taskqueue.push(request);
		}

		void stopPool()
		{
			stoped = true;
		}
#ifdef THREADPOOL_DEBUG
	public:
		void entryThread() { std::cout << "Enter" << std::endl; }
		void exitThread() { std::cout << "exit" << std::endl; }
#endif //end of #if THREADPOOL_DEBUG
	private:
		std::atomic<bool> stoped;
		std::atomic<uint32>  idleThreadNumber;
		std::vector<std::thread> pool;
		threadsafe_stack<std::shared_ptr<task>> taskqueue;
	};

	class submit : public paraalgorithm::task
	{
	public:
		submit(std::function<void()> fun) :func(fun)
		{
		}
		submit(submit&) = delete;
		submit operator=(submit&) = delete;
		submit(submit&& rhs) :func(std::move(rhs.func)) {}

		virtual void runtask() override
		{
			func();
			finish();
		}

	private:
		std::function<void()> func;
	};


	//該thread pool的使用例子, 實際上無需一個需求就建立一個類
	//僅需建立一個類,然後對該類實例提交一個無參函數,實際上有了lambda, 有多少個參數都可以用lambda+bind綁定成無參函數 ->參見functional/function101.hpp :curry
	//再實際上,其實task也無需寫成虛函數(我只是想這麽做而已
	std::shared_ptr<paraalgorithm::ThreadPool> testThreadPool()
	{
		std::shared_ptr<std::string> hellocplusplus = std::make_shared<std::string>("Hello World!\n");
		std::shared_ptr<paraalgorithm::ThreadPool> pool = std::make_shared<paraalgorithm::ThreadPool>();
		std::shared_ptr<task> sub1 = std::make_shared<submit>(
			[hellocplusplus]()//請注意,如果lambda讀取變量值,請傳值,如果這裏使用&hellocplusplus hellocplusplus會被析構導致内存錯誤.
			{
				//如果需要通過lambda保存運算結果,請自行保證捕捉函數生命周期
				copy(hellocplusplus->begin(), hellocplusplus->end(), std::ostream_iterator<char>(std::cout));
			}
		);
	
		pool->commit(sub1);
		return std::move(pool);

	}
	template<typename Container, typename T, typename Callback>
//	T parallel_Func(, T init, Callback cb)
	T testParallel(std::shared_ptr<paraalgorithm::ThreadPool>& pool, typename Container::iterator first, typename Container::iterator last, T& init, Callback cb)
	{
		using Iterator = typename Container::iterator;
		static_assert(!std::is_same<T, double>::value, "parallel double is not associative");
		static_assert(!std::is_same<T, float>::value, "parallel float is not associative");
		std::size_t totallength = std::distance(first, last);
		if (0 == totallength) return init;
		constexpr size_t min_per_thread = 25;
		size_t max_threads = (totallength + min_per_thread - 1) / min_per_thread;
		size_t poolidelthread = pool->getIdleThreadNumber();
		size_t num_threads = std::min((0 != poolidelthread ? poolidelthread : 2), max_threads);
		size_t block_size = totallength / num_threads;
		std::vector<T> results(num_threads);
		std::vector<std::shared_ptr<task>> taskqueue(num_threads);
		Iterator block_start = first;
		for (size_t index = 0; index != (num_threads - 1); ++index)
		{
			Iterator block_end = block_start;
			std::advance(block_end, block_size);

			std::shared_ptr < task > temp = std::make_shared<submit>
				(
					[=, &results]()
					{
						results[index] = cb(block_start, block_end, results[index]);
					}
			);
			taskqueue.emplace_back(temp);
			pool->commit(temp);
			block_start = block_end;
		}
		//沒有好的應用場景,先使用此段代碼.
// 		while (true)
// 		{
// 			int count = 0;
// 			for (auto& t : taskqueue)
// 			{
// 				if (t->isfinished())
// 					count++;
// 			}
// 			if (count == (num_threads - 1)) break;
// 		}
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

		results.emplace_back(cb(block_start, last, results.back()));
		return cb(results.begin(), results.end(), init);
	}

}



#endif // !__THREADPOOL_H__

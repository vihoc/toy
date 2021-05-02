//自製綫程池
//author:vihoc

#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future> 
#if _HAS_CXX20
#include <latch>
#endif

#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <memory>
#include <stack>
#include <iostream>
#include "BasicDataStructure.hpp"
#include "utilities.hpp"
#include "parallel_accumulate.h"

#pragma warning( disable  : 4390 ) //see line 152

//#define THREADPOOL_DEBUG

namespace paraalgorithm
{


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
		explicit task() {}
		//explicit task(std::shared_future<int> finish):finishpromise(finish) {}
		task(const task&) = delete;
		task operator=(const task&) = delete;
		task(const task&& rhs) {}
	public:
		virtual void runtask() = 0;
		virtual void afterTask() = 0;
	};
	//綫程池内部使用棧
	//使用指針請make_shared之後傳遞



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
							if (taskqueue.empty())
							{
								std::this_thread::yield();
								continue;
							}
#ifdef THREADPOOL_DEBUG
							entryThread();

#endif //end of #if THREADPOOL_DEBUG
							++idleThreadNumber;
							//獲取隊列中的一個元素

							std::shared_ptr<task> curtask;
							
							if(taskqueue.try_pop(curtask))
								curtask->runtask();
								
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
				if (worker.joinable()) 
					worker.join();//會被阻塞
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




}



#endif // !__THREADPOOL_H__

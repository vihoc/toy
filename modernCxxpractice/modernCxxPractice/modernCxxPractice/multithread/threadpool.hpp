//���u�Q�̳�
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

	//�Q�̳ع�������
	//�^�д��override runtask()
	//runtask��Q�̳��\������
	//moveable not copyable
	//TODO ��ӾQ�̹����������{
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
	//�Q�̳��ڲ�ʹ�×�
	//ʹ��ָ�Ոmake_shared֮����f



	//�Q�̳�
	//���Q�̳؟o�����C�����Q����̎��Ĕ����ľQ�̰�ȫ,Ո�ڂ��M���task�����б��C
	class ThreadPool
	{
	public: 
		//��ʼ��Ոʹ��
		//ThreadPool::uint32 threadnum = 10; 
		//ThreadPool pool(threadnum);
		//���߲���������ʼ��.
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
							//����΄���О��, �tጷŕr�gƬ.
							if (taskqueue.empty())
							{
								std::this_thread::yield();
								continue;
							}
#ifdef THREADPOOL_DEBUG
							entryThread();

#endif //end of #if THREADPOOL_DEBUG
							++idleThreadNumber;
							//�@ȡ����е�һ��Ԫ��

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
					worker.join();//��������
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

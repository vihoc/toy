//���u�Q�̳�
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

	//�Q�̳ع�������
	//�^�д��override runtask()
	//runtask��Q�̳��\������
	//moveable not copyable
	//TODO ��ӾQ�̹����������{
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
	//�Q�̳��ڲ�ʹ�×�
	//ʹ��ָ�Ոmake_shared֮����f

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
							if (taskqueue.empty()) std::this_thread::yield();
#ifdef THREADPOOL_DEBUG
							entryThread();

#endif //end of #if THREADPOOL_DEBUG
							++idleThreadNumber;
							//�@ȡ����е�һ��Ԫ��
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


	//ԓthread pool��ʹ������, ���H�ϟo��һ������ͽ���һ���
	//�H�轨��һ���,Ȼ�ጦԓ����ύһ���o������,���H������lambda, �ж��ق�������������lambda+bind�����ɟo������ ->��Ҋfunctional/function101.hpp :curry
	//�ٌ��H��,�䌍taskҲ�o�茑��̓����(��ֻ�����@��������
	std::shared_ptr<paraalgorithm::ThreadPool> testThreadPool()
	{
		std::shared_ptr<std::string> hellocplusplus = std::make_shared<std::string>("Hello World!\n");
		std::shared_ptr<paraalgorithm::ThreadPool> pool = std::make_shared<paraalgorithm::ThreadPool>();
		std::shared_ptr<task> sub1 = std::make_shared<submit>(
			[hellocplusplus]()//Ոע��,���lambda�xȡ׃��ֵ,Ո��ֵ,����@�Yʹ��&hellocplusplus hellocplusplus�������������ڴ��e�`.
			{
				//�����Ҫͨ�^lambda�����\��Y��,Ո���б��C��׽������������
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
		//�]�кõđ��È���,��ʹ�ô˶δ��a.
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

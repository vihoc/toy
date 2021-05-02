/*
*here is some naive use sample of my naive threadpool
* author:vihoc
*/

#ifndef __THERADPOOLSAMPLE_H__
#define __THERADPOOLSAMPLE_H__

#include "threadpool.hpp"


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
		afterTask();
	}

	virtual void afterTask() 
	{

	}
private:
	std::function<void()> func;
};

class submitwithFinish : public paraalgorithm::task
{
public:
	submitwithFinish(std::function<void()> fun) :func(fun), finishpromise{ nullptr }
	{
	}
	submitwithFinish(submitwithFinish&) = delete;
	submitwithFinish operator=(submit&) = delete;
	submitwithFinish(submitwithFinish&& rhs) :func(std::move(rhs.func)) {}

	void setPromise(std::shared_ptr<std::promise<int>> promise)
	{
		finishpromise = promise;
	}
public:
	virtual void runtask() override
	{
		func();
		afterTask();
	}
	virtual void afterTask() override
	{
		if(nullptr != finishpromise) finishpromise->set_value(1);
	}

private:
	std::function<void()> func;
	std::shared_ptr<std::promise<int>> finishpromise;
};

template<typename Container, typename T, typename Callback>
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
	std::vector<std::shared_ptr<paraalgorithm::task>> taskqueue(num_threads);
	Iterator block_start = first;
	for (size_t index = 0; index != (num_threads - 1); ++index)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);

		std::shared_ptr < paraalgorithm::task > temp = std::make_shared<submit>
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
	//TODO :std::future<>
	//沒有好的應用場景,先使用此段代碼.
	// 
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
		//TODO 因爲綫程池内的task與外部task無關,導致預想的等待計劃沒有實現,等待想個好點的辦法再提交這部分代碼
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	results.emplace_back(cb(block_start, last, results.back()));
	return cb(results.begin(), results.end(), init);
}

//naive的綫程池的睡眠排序應用 
//需要多少個元素就開多少個綫程.
//最後通過latch解決了這個問題:綫程啓動的時候,可能會很慢.
//here is a naive sleep sort implantation by my threadpool
// try to use condition_variable to tell when will the thread to start. but failed
// to use latch(in C++ 20) to ensure the condition_variable will receive notice from main thread.
// i have to say. to implantment sleep sort, we need use every thread in pool. this thing is not suit for threadpool.

template<typename Container>
auto testsleepsort(std::shared_ptr<paraalgorithm::ThreadPool> pool, Container& Data)
{
	//TODO check type
	static_assert (std::is_same<Container::value_type, int>::value, "we only support int element now");
	using elementType = typename Container::value_type;
	std::shared_ptr<std::mutex> m = std::make_shared<std::mutex>();
	std::shared_ptr < std::condition_variable> cv = std::make_shared<std::condition_variable>();
	//this staff provide since C++20
#if _HAS_CXX20
	std::shared_ptr < std::latch> threadInitDown = std::make_shared<std::latch>(Data.size());
	//we use 
#endif
	//std::shared_ptr < std::unique_lock<std::mutex>> lk = std::make_shared<std::unique_lock<std::mutex>>(*m);
	for (auto& element : Data)
	{
		std::shared_ptr < paraalgorithm::task > temp = std::make_shared<submit>
			(
				[element, &Data, cv, m
#if _HAS_CXX20
				, threadInitDown
#endif
				]() // 這裏shared_ptr不能用引用,引用就會av.
				{
#if _HAS_CXX20
					threadInitDown->count_down();
#endif
				
					std::unique_lock lk(*m);
					
					cv->wait(lk);
					std::this_thread::sleep_for(std::chrono::milliseconds(element)); //milliseconds
					Data.push_back(element);
				}
		);
		pool->commit(temp);
	}
	elementType time = *(std::max_element(Data.begin(), Data.end()));
	Data.clear();
#if _HAS_CXX20
	threadInitDown->wait();
#else
	std::this_thread::sleep_for(std::chrono::milliseconds(time + 100));
#endif
	//this line need send after, because this need until thread to wait this signal, for now i try to naivety use sleep
	//this line need move after std::this_thread::sleep_for(std::chrono::milliseconds(time));
	cv->notify_all(); //這裏綫程還沒創建完,需要等待
	
	
}
//here is another sleepsort implantation for sleep sort.
//it's just use std::future
template<typename Container>
auto testsleepsortusefuture(std::shared_ptr<paraalgorithm::ThreadPool> pool, Container& Data)
{
	static_assert (std::is_same<Container::value_type, int>::value, "we only support int element now");
	using elementType = typename Container::value_type;
 	std::vector< std::shared_ptr<std::promise<int>>> wait_promise;
 	std::vector < std::shared_future<int>> wait_forworker;
 	std::promise<int> start;
	std::shared_future startshare = start.get_future().share();
	std::vector< std::shared_ptr<std::promise<int>>> onfinish(Data.size());
	std::vector < std::shared_future<int>> wait_Finish;
	for (auto& element : Data)
	{
		std::shared_ptr<std::promise<int>> temppro = std::make_shared<std::promise<int>>();
		//temppro->set_value(0);
		std::shared_ptr<std::promise<int>> finishpromise = std::make_shared<std::promise<int>>();
		
		auto temp = std::make_shared<submitwithFinish>
			(
				[element, &Data, startshare, temppro]()
				{
					try
					{ 
						temppro->set_value(1);
						startshare.wait();
					}
					catch (const std::future_error& e) {
						std::cout << "Caught a future_error with code \"" << e.code()
							<< "\"\nMessage: \"" << e.what() << "\"\n";
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(element)); //milliseconds
					Data.push_back(element);
				}
				);
		temp->setPromise(finishpromise);
		//std::static_pointer_cast<std::shared_ptr<submitwithFinish>>(temp)->setPromise(finishpromise);
		std::shared_ptr < paraalgorithm::task > tasktocommit = temp;
		pool->commit(tasktocommit);
		wait_forworker.emplace_back(temppro->get_future().share());
		wait_Finish.emplace_back(finishpromise->get_future().share());
	}
	elementType time = *(std::max_element(Data.begin(), Data.end()));
	Data.clear();
	// i try to use wait, seems not work for my code . i assume it only work for sync
	//this is a exam for use native thread, so, better way is using std::package_task
	bool init = false;
	while(!init)
	{ 
		for (auto& w : wait_forworker)
		{
			if (0 == w.get())
			{
				init = false;
				break;
			}
			else
			{
				init = true;
			}
		}
	}

	start.set_value(1);

	int count = 0;
	//wait for all thread finish
	while (true)
	{ 
		count = 0;
		for (auto f : wait_Finish)
		{
			if (1 == f.get())
				count++;
		}
		if (count == wait_Finish.size()) break;
	}
}

//there is two way, a work check the result, and submit a copy of itself 
//or main thread check the promise and the result ,it not sort, submit a task again
//i decide do this both;
//first way we don't need change the original Data, the second way we need change the data from the original data set; 
//for the rest i would like try package_task
template<typename Container>
auto testBogo1(std::shared_ptr<paraalgorithm::ThreadPool> pool, Container& Data)
{
	static_assert (std::is_same<Container::value_type, int>::value, "we only support int element now");
	using elementType = typename Container::value_type;
	bool isunsorted = true;
	std::function<bool(Container&)> package_func;
	package_func = [&Data](Container& data) ->bool
	{
		
		int r = rand() % (data.size());
		for (auto& i : data)
		{
			std::swap(i, data[r]);
		}
		if (is_sorted(data.begin(), data.end()))
		{
			Data.clear();
			std::copy(data.begin(), data.end(), std::back_inserter(Data));
			return true;
		}
		return false;
	};
		std::function<void()> tempfunc;
		tempfunc = [ &Data, &tempfunc, &package_func, pool]()
		{
			Container tempData;
			std::copy(Data.begin(), Data.end(), std::back_inserter(tempData));
			try
			{
			std::packaged_task<bool(Container&)> Bogotask1(package_func);
			Bogotask1(tempData);
			std::future<bool> result = Bogotask1.get_future();
			if (false == result.get())
			{
			
				std::shared_ptr < paraalgorithm::task > tempcommit = std::make_shared< submit >(tempfunc);
				pool->commit(tempcommit);
			}
			}
			catch (const std::future_error& e) {
				std::cout << "Caught a future_error with code \"" << e.code()
					<< "\"\nMessage: \"" << e.what() << "\"\n";
			}
			
			
			
		};
		auto temp = std::make_shared<submitwithFinish>(tempfunc);
		
		std::shared_ptr < paraalgorithm::task > tasktocommit = temp;
		pool->commit(tasktocommit);
		//do something 
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}


}

template<typename Container>
auto testBogo2(std::shared_ptr<paraalgorithm::ThreadPool> pool, Container& Data)
{
	static_assert (std::is_same<Container::value_type, int>::value, "we only support int element now");
	using elementType = typename Container::value_type;
	bool isunsorted = true;

	while (isunsorted)
	{
		std::packaged_task<bool(Container&)> Bogotask2(
			[](Container& data) ->bool
			{
				int r = rand() % (data.size());
				for (auto& i : data)
				{
					std::swap(i , data[r]);
				}
				return is_sorted(data.begin(), data.end());
			});
		std::future<bool> result = Bogotask2.get_future();
		std::shared_ptr<std::promise<int>> finishpromise = std::make_shared<std::promise<int>>();
		auto temp = std::make_shared<submitwithFinish>
			(
				[&Data, &Bogotask2]()
				{
					return Bogotask2(Data);
				}
		);
		temp->setPromise(finishpromise);
		std::shared_ptr < paraalgorithm::task > tasktocommit = temp;
		pool->commit(tasktocommit);
		std::shared_future<int> isjobdone = finishpromise->get_future().share();
		while (1 != isjobdone.get())
				;
		if (true == result.get())
		{
			isunsorted = false;
		}
	}
	
}
//this is a sample code for use promise to return some info
template<typename Container>
auto testSpaghettiSort(std::shared_ptr<paraalgorithm::ThreadPool> pool, Container& Data)
{
	static_assert (std::is_same<Container::value_type, int>::value, "we only support int element now");
	using elementType = typename Container::value_type;
	std::packaged_task<Container(Container&)> SpaghettiSort(
		[](Container& data) ->Container
		{
			Container temp;
			temp.reserve(data.size());
			Container ret;
			ret.reserve(data.size());
			std::transform(data.begin(), data.end(), std::back_inserter(temp), [](elementType& x)
				{
					return x;
				});//suppose iota is a infinite Spaghetti
			while (0 != temp.size())
			{
				typename Container::iterator it = std::min_element(temp.begin(), temp.end());
				ret.emplace_back(*it);
				temp.erase(it);
			}
			return ret;
		});

		std::future<Container> result = SpaghettiSort.get_future();
		std::shared_ptr<std::promise<int>> finishpromise = std::make_shared<std::promise<int>>();
		auto temp = std::make_shared<submitwithFinish>
			(
				[&Data, &SpaghettiSort]()
				{
					return SpaghettiSort(Data);
				}
		);
		temp->setPromise(finishpromise);
		std::shared_ptr < paraalgorithm::task > tasktocommit = temp;
		pool->commit(tasktocommit);
		std::shared_future<int> isjobdone = finishpromise->get_future().share();
		
		while (1 != isjobdone.get())
			;
		utilities::println_range(result.get(), "Data:");

}

/// <summary>
/// 猴子排序 TODO
/// </summary>
void test();

void runtestparallel_accumulate();

void runtestparallel_Func();

void runtestparallel_quicksort();


#endif // end of __THERADPOOLSAMPLE_H__

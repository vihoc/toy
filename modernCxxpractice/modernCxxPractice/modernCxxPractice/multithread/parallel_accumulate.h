//一些并行算法的實現, since now, it based on book "C++ concurency in Action"
// may not in future
//author:vihoc 
//


#ifndef __PARALLEL_ACCUMULATE_H__
#define __PARALLEL_ACCUMULATE_H__
#include <thread>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include "testParaalgorithmFrame.hpp"
namespace paraalgorithm
{
	//C++ concurrency in action中的parallel_accumulate實現
	template<typename Container, typename T>
	T parallel_accumulate(typename Container::iterator first,typename Container::iterator last, T init)
	{
		using Iterator = typename Container::iterator;
		static_assert(!std::is_same<T, double>::value, "parallel double is not associative");
		static_assert(!std::is_same<T, float>::value, "parallel float is not associative");
		std::size_t totallength =  std::distance(first, last);
		if (0 == totallength) return init;
		constexpr size_t min_per_thread = 25;
		size_t max_threads = (totallength + min_per_thread - 1) / min_per_thread;
		size_t hardware_threads = std::thread::hardware_concurrency();
		size_t num_threads = std::min((0 != hardware_threads ? hardware_threads : 2), max_threads);
		size_t block_size = totallength / num_threads;
		std::vector<T> results(num_threads);
		std::vector<std::thread> threads(num_threads - 1);
		Iterator block_start = first;
		for (size_t index = 0; index != (num_threads - 1); ++index)
		{
			Iterator block_end = block_start;
			std::advance(block_end, block_size);

			threads[index] = std::thread
			{
				[=,&results] {
					results[index] = std::accumulate(block_start, block_end, results[index]);
				}
			};

			block_start = block_end;
		}
		for (auto& t : threads)  t.join();
		results.emplace_back(std::accumulate(block_start, last, results.back()));
		return std::accumulate(results.begin(), results.end(), init);
	}
	void runtest()
	{
		//init vector
		constexpr std::size_t maxelement = 50000000;
		std::vector<int> v(maxelement);
		
		std::iota(v.begin(), v.end(), 1);

		testFranework<std::vector<int>> test(v);
		test.runtest(std::bind(parallel_accumulate<std::vector<int>, int>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(std::accumulate<std::vector<int>::iterator, int>,  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	}

	
	template<typename Container, typename T, typename Callback>
	T parallel_Func(typename Container::iterator first, typename Container::iterator last, T init, Callback cb)
	{
		std::cout << "func" << std::endl;
		using Iterator = typename Container::iterator;
		static_assert(!std::is_same<T, double>::value, "parallel double is not associative");
		static_assert(!std::is_same<T, float>::value, "parallel float is not associative");
		std::size_t totallength = std::distance(first, last);
		if (0 == totallength) return init;
		constexpr size_t min_per_thread = 25;
		size_t max_threads = (totallength + min_per_thread - 1) / min_per_thread;
		size_t hardware_threads = std::thread::hardware_concurrency();
		size_t num_threads = std::min((0 != hardware_threads ? hardware_threads : 2), max_threads);
		size_t block_size = totallength / num_threads;
		std::vector<T> results(num_threads);
		std::vector<std::thread> threads(num_threads - 1);
		Iterator block_start = first;
		for (size_t index = 0; index != (num_threads - 1); ++index)
		{
			Iterator block_end = block_start;
			std::advance(block_end, block_size);

			threads[index] = std::thread
			{
				[=,&results]() {
					//results[index] = std::accumulate(block_start, block_end, results[index]);
					results[index] = cb(block_start, block_end, results[index]);
				}
			};

			block_start = block_end;
		}
		for (auto& t : threads)  t.join();
		results.emplace_back(cb(block_start, last, results.back()));
		return std::accumulate(results.begin(), results.end(), init);
	}

	void runtest_func()
	{
		//init vector
		constexpr std::size_t maxelement = 50000000;
		std::vector<int> v(maxelement);

		std::iota(v.begin(), v.end(), 1);

		testFranework<std::vector<int>> test(v);
		test.runtest(std::bind(parallel_Func<std::vector<int>, int, std::function<int(std::vector<int>::iterator, std::vector<int>::iterator, int)>>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
			[](std::vector<int>::iterator begin, std::vector<int>::iterator end, int x)
			{
				return accumulate(begin, end, x);
			}),
			std::bind(std::accumulate<std::vector<int>::iterator, int>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	}
	

} // end of paraalgorithm





//修改代碼的minimal版本,副產物,如果想要修改模板相關參數,可以先修改這裏
namespace minimal
{
	//////////////////////////////////////////////////////////////////////////
	//start of minimal parallel_accumulate
	template<typename Container, typename T>
	T somefunc(typename Container::iterator first, typename Container::iterator last, T init)
	{
		std::distance(first, last);
		return init;
	}
	template<typename Container, bool isdebug = true>
	class test
	{
	public:
		test(Container& rhs) :v{ rhs } {}
		template<typename CallParallel, typename CallNomal>
		auto runtest(CallParallel callbackp, CallNomal callbackn)
		{

			int init = 0;
			return [&]()
			{
				callbackp(v.begin(), v.end(), init);
				callbackn(v.begin(), v.end(), init);
			};

		}
	private:
		Container& v;
	};

	// template<typename Iterator, typename T>
	// T somefunc(typename Iterator first, typename Iterator last, T init)
	// {
	// 	return init;
	// }
	// //template<typename Container, bool isdebug = true>
	// class test
	// {
	// public:
	// 	//using Iterator = typename Container::iterator;
	// 	template<typename callbackA, typename callbackB>
	// 	void runtest(callbackA callbackp, callbackB callbackn)
	// 	{
	// 		v.reserve(10);
	// 		int init = 0;
	// 		callbackp(v.begin(), v.end(), init);
	// 		callbackn(v.begin(), v.end(), init);
	// 	}
	// private:
	// 	std::vector<int> v;
	// };

	void testminimal()
	{
		// 	std::vector<int> v;
//   	test<std::vector<int>> t(v);
//   	t.runtest(std::bind(somefunc<std::vector<int>, int>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
//   		std::bind(somefunc<std::vector<int>, int>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}

	//end of minimal parallel_accumulate
	//////////////////////////////////////////////////////////////////////////////////////////


} // end of minimal


#endif // end of __PARALLEL_ACCUMULATE_H__
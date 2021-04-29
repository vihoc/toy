// modernCxxPractice.cpp : This file contains the 'main' function. Program execution begins and ends there.
//author: vihoc
// 函數式編程例子相關放在functional文件夾下
//模板類將放在functional/template子文件夾下
// 多綫程類將放在multithread下
// C++11/14/17内容例子將放在./下
// rx/range庫實戰將放在reactive文件夾下
//規約:會在其他文件中定義很多的namespace, 而namespace::main 則是測試函數

#ifdef _MSC_VER
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#pragma warning( disable : 4700 ) // 因爲要使用一些不需要值的功能,但是因爲實例化會被報錯.
#endif
//包含windows會跟std的min, max起衝突,于range也會有問題.所以需要如下内容來解決
//並注釋掉包含range的所有文件
#if defined _MSC_VER
#define NOMINMAX
#include <Windows.h>
#endif
#include <iostream>
#include <string>
#include <iterator>

// #include "Cpp101.hpp"
// #include "functional/function101.hpp"
// #include "functional/range101.hpp"
// #include "multithread/parallel_accumulate.h"
#include "utilities.hpp"
#include "multithread/threadpool.hpp"



//step 1, declare a modern main . 現代C++實戰的第一步:定義一個看上去不一樣且有點炫酷的main函數
auto main() -> int 
{
   // CPP101::main();
   // functional::main();
    //rangeuse::main();
    //paraalgorithm::runtest();
	//paraalgorithm::runtest_func();
	auto p = paraalgorithm::testThreadPool();
 	constexpr std::size_t maxelement = 50000000;
	std::vector<int> v(maxelement); 
 	std::iota(v.begin(), v.end(), 1);
	int init = 0;
	
	std::chrono::duration<double> RunningTime;
	auto call = [&v, &p, &init]()
	{
		return paraalgorithm::testParallel<std::vector<int>, int, std::function<int(std::vector<int>::iterator, std::vector<int>::iterator, int) >>(p, v.begin(), v.end(), init,
			std::bind(std::accumulate<std::vector<int>::iterator, int>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	};
	RunningTime = utilities::TestFuncRunningTime(call, init);
	//utilities::println(RunningTime.count(), " parallel result Take Time:");
	
	//utilities::println(init, " Result:");
	
	std::cout << " parallel result Take Time:" << RunningTime.count() << "      result is: " << init << std::endl;
	std::string hellocplusplus = "Hello World!\n";
	//Sleep(1000000);
	
// 	std::string hellocplusplus = "Hello World!\n";
 	copy(hellocplusplus.begin(), hellocplusplus.end(), std::ostream_iterator<char>(std::cout));
}


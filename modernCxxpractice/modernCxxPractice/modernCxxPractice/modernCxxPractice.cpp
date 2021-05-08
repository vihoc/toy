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
// #if defined _MSC_VER
// #define NOMINMAX
// #include <Windows.h>
// #endif
#include <iostream>
#include <string>
#include <iterator>

 #include "Cpp101.hpp"
 #include "functional/function101.hpp"
 #include "functional/range101.hpp"
 #include "multithread/parallel_accumulate.h"
#include "utilities.hpp"
#include "multithread/threadpool.hpp"
#include "multithread/ThreadpoolSample.h"


//step 1, declare a modern main . 現代C++實戰的第一步:定義一個看上去不一樣且有點炫酷的main函數
auto main() -> int 
{
   // CPP101::main();
    functional::main();
    //rangeuse::main();
    //paraalgorithm::runtest();
	//paraalgorithm::runtest_func();
	//test();

	//std::this_thread::sleep_for(std::chrono::milliseconds(20000));
	std::string hellocplusplus = "Hello World!\n";
	
// 	std::string hellocplusplus = "Hello World!\n";
 	copy(hellocplusplus.begin(), hellocplusplus.end(), std::ostream_iterator<char>(std::cout));
}


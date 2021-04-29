// modernCxxPractice.cpp : This file contains the 'main' function. Program execution begins and ends there.
//author: vihoc
// ����ʽ�����������P����functional�ļ��A��
//ģ�������functional/template���ļ��A��
// ��Q�������multithread��
// C++11/14/17�������ӌ�����./��
// rx/range�쌍��������reactive�ļ��A��
//Ҏ�s:���������ļ��ж��x�ܶ��namespace, ��namespace::main �t�ǜyԇ����

#ifdef _MSC_VER
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#pragma warning( disable : 4700 ) // ��Ҫʹ��һЩ����Ҫֵ�Ĺ���,�����򠑌������������e.
#endif
//����windows����std��min, max���nͻ,��rangeҲ���І��}.������Ҫ�������݁��Q
//�Kעጵ�����range�������ļ�
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



//step 1, declare a modern main . �F��C++����ĵ�һ��:���xһ������ȥ��һ�������c�ſ��main����
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


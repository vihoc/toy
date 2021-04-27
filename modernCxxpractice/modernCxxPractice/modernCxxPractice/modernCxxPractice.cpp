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
#include <iostream>
#include <string>
#include <iterator>
#include "Cpp101.hpp"
#include "functional/function101.hpp"
#include "functional/range101.hpp"
#include "multithread/parallel_accumulate.h"


//step 1, declare a modern main . �F��C++����ĵ�һ��:���xһ������ȥ��һ�������c�ſ��main����
auto main() -> int 
{
   // CPP101::main();
   // functional::main();
    //rangeuse::main();
    paraalgorithm::runtest();

	std::string hellocplusplus = "Hello World!\n";
	copy(hellocplusplus.begin(), hellocplusplus.end(), std::ostream_iterator<char>(std::cout));
}


// toy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//#define _DEBUG
#include "crtdbg.h"

#include <iostream>

#include <algorithm>

#include "../../DataStructuresToy/array_list.h"
#include "../../DataStructuresToy/doubly_circular_list.h"
#include "../../DataStructuresToy/linked_list.h"
#include "../../DataStructuresToy/tree.h"
#include "../../DataStructuresToy/binary_tree.h"
#include<vector>
#include <tuple>
// template <typename... arg>
// class T
// {
// 	using storeType = void*;
// 	//using para = arg;
// 	using args = typename... arg;
// 	using adictuple = std::tuple<arg...>;
// 	template<args>
// 	void setData(args&& arg) { adictuple arg_sss = new adictuple(std::forward<arg>(...)); args_ = static_cast<storeType*>(arg_sss); }
// 
// private:
// 	storeType args_;
// };

int main()
{




   // _CrtSetDbgFlag(true);
    std::cout << "Hello World!\n";
    VihoStructures::ArrayList<int> test = {5, 1, 2, 3, 4, 4, 3, 2, 1, 5};
	std::vector<int> z = { 1, 2, 5, 3, 4 };

	std::cout << "fucking array data" << std::endl;
	int x = 0;
    for (auto i : test)
    {
        std::cout << i;
    }
    std::cout<<std::endl;
	std::cout << "move "<<std::endl;


	for (auto i : test)
	{
		std::cout << i;
	}
	std::cout << std::endl;
	std::cout << "operator[]"<<std::endl;
	const VihoStructures::ArrayList<int> consttest(test);
	for (size_t i = 0; i < test.size(); ++i)
	{
		std::cout << consttest[i] << "    ";
		std::cout << consttest.at(i) << "    ";
	}
	std::cout << std::endl;
    std::cout << test.end() - test.begin();
	
    std::sort(test.begin(), test.end(), [&test](auto a, auto b)
		{

            return a > b;
        });
	std::cout << "const_iterator" << std::endl;
    for (auto i = test.cbegin(); i!= test.cend(); ++i , ++x)
    {
//         if (1 == *i)
//             i = test.erase(i);
		std::cout << *i<<std::endl;
     }
	std::cout << std::endl;
    VihoStructures::DoublyCircularList<int> test1 = { 1, 2, 3, 4, 5 };
   
	std::cout << "====test DoublyCircularList reverse" << std::endl;
	for (auto iter = test1.rbegin(); iter != test1.rend(); x++, iter++)
	{
		//      auto a = iter;


		std::cout << *iter << std::endl;
	}
	for (auto iter = test1.begin(); iter != test1.end(); ++iter)
	{
		std::cout << *iter << std::endl;
	}

    VihoStructures::LinkedList<int> testlist = {1, 2, 3, 4, 5};
	std::cout << "====test LinkedListIter" << std::endl;
	for (auto iter = testlist.begin(); iter != testlist.end(); x++, iter++)
	{
		//      auto a = iter;

		
		std::cout << *iter << std::endl;
	}
	std::cout << "====test const LinkedListIter" << std::endl;
	for (auto iter = testlist.cbegin(); iter != testlist.cend(); x++, iter++)
	{
		//      auto a = iter;


		std::cout << *iter << std::endl;
	}

	std::cout << "====test reverse LinkedListIter" << std::endl;
	for (auto iter = testlist.rbegin(); iter != testlist.rend(); x++, iter++)
	{
		//      auto a = iter;


		std::cout << *iter << std::endl;
	}
	//VihoStructures::Tree<int, VihoStructures::sampleNode<int>> testTree = { 1, 3,5, 6, 8 };

//	VihoStructures::BinaryTree<int> testbinary(test);
//	testbinary.ImportDataFromArray(test);
//	testbinary.print();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

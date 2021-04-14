// toy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//#define _DEBUG
#include "crtdbg.h"

#include <iostream>

#include <algorithm>

#include"array_list.h"
#include"doubly_circular_list.h"
#include"linked_list.h"
#include"tree.h"
#include "binary_tree.h"
#include<vector>
int main()
{
   // _CrtSetDbgFlag(true);
    std::cout << "Hello World!\n";
    VihoStructures::ArrayList<int> test = {5, 1, 2, 3, 4, 4, 3, 2, 1, 5};
	std::vector<int> z = { 1, 2, 5, 3, 4 };
// 	std::sort(z.begin(), z.end(), [&test](auto a, auto b)
// 		   {
// 
// 	             return a < b;
// 	         });
// 	std::cout << "fucking array data" << std::endl;
// 	for (auto e : z)
// 	{
// 		std::cout << e << std::endl;
// 	}
	std::cout << "fucking array data" << std::endl;
	int x = 0;
    for (auto i : test)
    {
        std::cout << i;
    }
    std::cout<<std::endl;
	std::cout << "move "<<std::endl;
	//*(test.begin()) = std::move(*(test.begin() + 1));

	//插入排序
	for (auto iter = test.begin() + 1 ; iter != test.end(); ++iter)
	{
		for(auto iter2 = test.begin(); iter2 != iter; ++ iter2)
			if (*iter > *iter2)
			{
				std::cout<< "      " << "this time " << *iter << "    " << *iter2 << std::endl;
				//std::swap(*iter, *iter2);
				auto T = *iter;
				*iter = *iter2;
				*iter2 = T;
			}
	}

	for (auto i : test)
	{
		std::cout << i;
	}
	std::cout << std::endl;
    std::cout << test.end() - test.begin();
	
	//std::swap(*(test.begin()), *(test.end() - 2));
//     std::sort(test.begin(), test.end(), [&test](auto a, auto b)
// 		{
// 			std::cout << "fucking array data" << std::endl;
// 				for (auto e : test)
// 				{
// 				std::cout << e <<"   ";
// 				}
// 				std::cout << std::endl;
//             return a > b;
//         });
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
	VihoStructures::BinaryTree<int> testbinary;
	testbinary.ImportDataFromArray(test);
	testbinary.print();
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

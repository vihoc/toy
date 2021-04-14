// toy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include"array_list.h"
#include <algorithm>
#include<vector>
#include <list>
#include"doubly_circular_list.h"
#include"linked_list.h"
int main()
{
    std::cout << "Hello World!\n";
    VihoStructures::ArrayList<int> test = {5, 1, 2, 3, 4, 4, 3, 2, 1, 5};
    for (auto i : test)
    {
        std::cout << i;
    }
    std::cout<<std::endl;
    std::cout << test.end() - test.begin();
    std::sort(test.begin(), test.end(), [](auto a, auto b)
        {
            return a < b;
        });
    for (auto i = test.begin(); i!= test.end(); ++i )
    {
        if (1 == *i)
            i = test.erase(i);
     }
    VihoStructures::DoublyCircularList<int> test1 = { 1, 2, 3, 4, 5 };
    int x = 0;
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
// 	for (auto iter = testlist.rbegin(); iter != testlist.rend(); x++, iter++)
// 	{
// 		//      auto a = iter;
// 
// 
// 		std::cout << *iter << std::endl;
// 	}
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

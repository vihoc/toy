//測試并行算法與普通版本的框架,
//  this file is simple vision of test  Test the running time of range based parallel and non-parallel algorithms 
// the framework to take no resrouce.
//author:vihoc 
//to improve: unzip parameter.

#ifndef __TEST_PARAALGORITHMFRAME_H__
#define __TEST_PARAALGORITHMFRAME_H__
#include "utilities.hpp"
namespace paraalgorithm
{
	//i try to make ref of container as const, but failed.
	template<typename Container, bool isdebug = true>
	class testFranework
	{
	public:
		typedef Container ContainerType;
// 		typedef  Container<T> ContainerType;
 		using value_type = typename ContainerType::value_type;
	public:
		//not copyable, but move able
		testFranework(const testFranework&) = delete;
		testFranework operator=(const testFranework&) = delete;

		testFranework(const testFranework&& rhs) :container{ rhs.container } {}
		testFranework operator=(const testFranework&& rhs) = delete;
	public:
		//RAII
		testFranework(ContainerType& ocontainer) :container{ ocontainer }
		{

		}
		template<typename CallParallel, typename CallNomal>
		void runtest(CallParallel callbackp, CallNomal callbackn)
		{
			value_type resultP = 0, resultN = 0;
			std::chrono::duration<double> parallelRunTime;
			std::chrono::duration<double> NomalRunTime;
			std::thread ThreadParallel
			{
				[&resultP, this, &parallelRunTime, &callbackp]()
				{
					parallelRunTime = utilities::TestFuncRunningTime(std::bind(callbackp, (this->container).begin(), (this->container).end(), resultP), resultP);
					return 0;
				}
			};
			std::thread threadNomal
			{
				[&resultN, this, &NomalRunTime, &callbackn]()
				{
/
					NomalRunTime = utilities::TestFuncRunningTime(std::bind(callbackn, (this->container).begin(), (this->container).end(), resultN), resultN);
					return 0;
				}
			};
			//wait for worker finish
			if (isdebug)
			{
				if(ThreadParallel.joinable()) ThreadParallel.join();
				if (threadNomal.joinable()) threadNomal.join();
			}
			else
			{
				if (ThreadParallel.joinable())ThreadParallel.detach();
				if (threadNomal.joinable()) threadNomal.detach();
			}
			std::cout << " parallel result Take Time:" << parallelRunTime.count() << "      result is: "  << resultP << std::endl;
			std::cout << " normal result Take Time:" << NomalRunTime.count() << "      result is: " << resultN << std::endl;
		}

	private:
		ContainerType& container;

	};

}


//做同步,給work綫程發消息,work一起工作,工作完了返回信號給管理綫程,管理綫程調用一個回調函數

#endif

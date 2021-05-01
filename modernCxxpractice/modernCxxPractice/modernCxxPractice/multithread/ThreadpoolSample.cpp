#include "ThreadpoolSample.h"


//該thread pool的使用例子, 實際上無需一個需求就建立一個類
//僅需建立一個類,然後對該類實例提交一個無參函數,實際上有了lambda, 有多少個參數都可以用lambda+bind綁定成無參函數 ->參見functional/function101.hpp :curry
//再實際上,其實task也無需寫成虛函數(我只是想這麽做而已
std::shared_ptr<paraalgorithm::ThreadPool> testThreadPool()
{
	std::shared_ptr<std::string> hellocplusplus = std::make_shared<std::string>("Hello World!\n");
	std::shared_ptr<paraalgorithm::ThreadPool> pool = std::make_shared<paraalgorithm::ThreadPool>();
	std::shared_ptr<paraalgorithm::task> sub1 = std::make_shared<submit>(
		[hellocplusplus]()//請注意,如果lambda讀取變量值,請傳值,如果這裏使用&hellocplusplus hellocplusplus會被析構導致内存錯誤.
		{
			//如果需要通過lambda保存運算結果,請自行保證捕捉函數生命周期
			copy(hellocplusplus->begin(), hellocplusplus->end(), std::ostream_iterator<char>(std::cout));
		}
	);

	pool->commit(sub1);
	return std::move(pool);

}



void test()
{

	/*
	* 測試綫程池相關
	*/

// 	auto p = testThreadPool();
// 	 	constexpr std::size_t maxelement = 50000000;
// 			std::vector<int> v(maxelement); 
// 		  	std::iota(v.begin(), v.end(), 1);
// 		 	int init = 0;
// 		 	
// 			std::chrono::duration<double> RunningTime;
// 		 	auto call = [&v, &p, &init]()
// 		 	{
// 		 		return testParallel<std::vector<int>, int, std::function<int(std::vector<int>::iterator, std::vector<int>::iterator, int) >>(p, v.begin(), v.end(), init,
// 		 			std::bind(std::accumulate<std::vector<int>::iterator, int>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
// 		 	};
// 		 	RunningTime = utilities::TestFuncRunningTime(call, init);
// 
// 		std::cout << " parallel result Take Time:" << RunningTime.count() << "      result is: " << init << std::endl;



		std::vector<int> v;
		//綫程開多了會很卡,數據80%大概是對的.
		for (int i = 1; i < 1000; i += 100)
		{
			v.emplace_back(i);
		}
		std::shared_ptr<paraalgorithm::ThreadPool> p = std::make_shared<paraalgorithm::ThreadPool>(v.size());
	//testsleepsort(p, v);
	//std::this_thread::sleep_for(std::chrono::seconds(60));
		//testsleepsortusefuture(p, v);
		testBogo1(p, v);
//		testBogo2(p, v);
	utilities::println_range(v, "Data:");
		///////////////////////////////////////測試綫程池相關結束
}

void runtestparallel_accumulate()
{
	//init vector
	constexpr std::size_t maxelement = 50000000;
	std::vector<int> v(maxelement);

	std::iota(v.begin(), v.end(), 1);

	paraalgorithm::testFranework<std::vector<int>> test(v);
	test.runtest(std::bind(paraalgorithm::parallel_accumulate<std::vector<int>, int>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
		std::bind(std::accumulate<std::vector<int>::iterator, int>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

}


//在這裏我們將要并行計算的函數放在了外面,這樣我們可以方便的替換成別的函數.
//TODO,接口做的更加通用一點

void runtestparallel_Func()
{
	//init vector
	constexpr std::size_t maxelement = 50000000;
	std::vector<int> v(maxelement);

	std::iota(v.begin(), v.end(), 1);

	paraalgorithm::testFranework<std::vector<int>> test(v);
	test.runtest(std::bind(paraalgorithm::parallel_Func<std::vector<int>, int, std::function<int(std::vector<int>::iterator, std::vector<int>::iterator, int)>>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
		[](std::vector<int>::iterator begin, std::vector<int>::iterator end, int x)
		{
			return accumulate(begin, end, x);
		}),
		std::bind(std::accumulate<std::vector<int>::iterator, int>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

}
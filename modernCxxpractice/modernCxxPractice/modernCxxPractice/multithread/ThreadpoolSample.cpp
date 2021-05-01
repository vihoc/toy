#include "ThreadpoolSample.h"


//ԓthread pool��ʹ������, ���H�ϟo��һ������ͽ���һ���
//�H�轨��һ���,Ȼ�ጦԓ����ύһ���o������,���H������lambda, �ж��ق�������������lambda+bind�����ɟo������ ->��Ҋfunctional/function101.hpp :curry
//�ٌ��H��,�䌍taskҲ�o�茑��̓����(��ֻ�����@��������
std::shared_ptr<paraalgorithm::ThreadPool> testThreadPool()
{
	std::shared_ptr<std::string> hellocplusplus = std::make_shared<std::string>("Hello World!\n");
	std::shared_ptr<paraalgorithm::ThreadPool> pool = std::make_shared<paraalgorithm::ThreadPool>();
	std::shared_ptr<paraalgorithm::task> sub1 = std::make_shared<submit>(
		[hellocplusplus]()//Ոע��,���lambda�xȡ׃��ֵ,Ո��ֵ,����@�Yʹ��&hellocplusplus hellocplusplus�������������ڴ��e�`.
		{
			//�����Ҫͨ�^lambda�����\��Y��,Ո���б��C��׽������������
			copy(hellocplusplus->begin(), hellocplusplus->end(), std::ostream_iterator<char>(std::cout));
		}
	);

	pool->commit(sub1);
	return std::move(pool);

}



void test()
{

	/*
	* �yԇ�Q�̳����P
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
		//�Q���_���˕��ܿ�,����80%����ǌ���.
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
		///////////////////////////////////////�yԇ�Q�̳����P�Y��
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


//���@�Y�҂���Ҫ����Ӌ��ĺ�������������,�@���҂����Է������Q�Ʉe�ĺ���.
//TODO,�ӿ����ĸ���ͨ��һ�c

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
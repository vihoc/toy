/*
*һ�c�c�F��C++��΢С��׃��,���ļ����漰ģ�����P������,ģ�����P��׃��Ո��functional�ļ��A����.
* author:vihoc
* ���g�x�:gcc-7 -std=c++17, vsʹ��17�˜ʾ��g.
*/

#ifndef __CPP_101__
#define __CPP_101__
#include <iostream>
#include <memory>
#include <functional>
#include <string>
#include <tuple>
#include <chrono>

namespace CPP101
{

	template<typename T>
	auto commenPrint(const std::string& str, T value) ->void
	{
		std::cout << str;
		std::cout << value << std::endl;
	}
// 	template<int>
// 	auto commenPrint(const std::string& str, int value) ->void
// 	{
// 		std::cout << str;
// 		std::cout << (int)value << std::cout;
// 	}
// 	template<typename T>
// 	auto commenPrint(const std::string& str, T value) ->void
// 	{
// 		std::cout << str << std::invoke(value) << std::cout;
// 	}
	auto testtypeid() -> void
	{
		auto a = 1;
		auto b = 1.0;
		auto c = a + b;
		auto d = { b, c };
		//�@�Y�������һЩ����Ҫԇԇ�Ĕ������

		std::cout << "type of a :" << typeid(a).name() << std::endl;
		std::cout << "type of b :" << typeid(b).name() << std::endl;
		std::cout << "type of c :" << typeid(c).name() << std::endl;
		std::cout << "type of d :" << typeid(d).name() << std::endl;
		//���������������Ҳ���һ����Kݔ������
	}

	template<typename T>
	auto printtypename(T t) -> void 
	{
		std::cout << "type of " << t << " :"<< typeid(t).name() << std::endl;
	}


	auto testdecltype_1() -> void
	{
		//��һЩ�|��
		const int func1();
		const int& func2();
		int i;
		struct X { double d; };
		const X* x = new X();
		//ԇԇ��decltype��������Щ�������,����ݔ��һЩɶ
		decltype(func1()) f1;
		decltype(func2()) f2 = 1;//������������,���Խo���x��ֵ
		decltype(i) i2;
		decltype(x->d) d1;
		decltype((x->d)) d2 = 0.0; //������������,���Խo���x��ֵ

		std::cout << "type of f1: "  << typeid(f1).name() << std::endl; //TODO 
		std::cout << "type of f2: " << typeid(f2).name() << std::endl;
		std::cout << "type of i2: " << typeid(i2).name() << std::endl;
		std::cout << "type of d1: " << typeid(d1).name() << std::endl;
		std::cout << "type of d2: " << typeid(d2).name() << std::endl;

	}

	template<typename T, typename N>
	auto helperadd(T t, N n) -> decltype(t + n)
	{
		return t + n;
	}

	auto testdecltype_2() -> void
	{
		auto a = helperadd<int, double>(2, 2.5);
		printtypename(a);
		std::cout << "result of 2 + 2.5 :" << a << std::endl;
	}

	auto non_menber_iterator() -> void
	{
		int arr[] = { 1, 2, 3, 4, 5, 6, 7 };
		std::cout << "non_menber_iterator" << std::endl;
		for (auto i = std::begin(arr); i != std::end(arr); ++i )
		{
			std::cout << *i << " ";
		}
		std::cout << std::endl;
	}

	auto range_base_for() -> void
	{
		int arr[] = { 1, 2, 3, 4, 5, 6, 7 };
		std::cout << "non_menber_iterator" << std::endl;
		for (auto i: arr)
		{
			std::cout << i << " ";
		}
		std::cout << std::endl;
	}

	//lambda�o������, ֻ��һ����������,�����h�κ���r��ʹ��
	auto lambda_sample() -> void
	{
		int a = 5;
		commenPrint("a :", a);
		//[] �� &, ����׽X������, �@�Y�����Ĳ��]��x,�����xֵ����a, ���h������Ҫ��lambda�Y����a��ֵ.Ҳֻ��[&a]����
		auto lambdatest = [&x = a]() { x += 2; };
		lambdatest();//std::invoke(lambdatest);
		commenPrint("now a :", a);
	}

	//generic lambda 
	auto genericlambda_sample() -> void
	{
		auto my_max = [](auto&& x, auto&& y)//ԭ����[](auto& x, auto& y) �����ґеö��x׃��ֱ������ֵ��
		{return x > y ? x : y; };
		commenPrint("Max of int value", my_max(5, 3));
		commenPrint("Max of double value", my_max(5.0, 3.0));
	}
	//�����M��unique_ptr shared_ptr weak_ptr
	class someThing
	{
	public:
		using shardptr = std::shared_ptr<someThing>;
		using uniqueptr = std::unique_ptr<someThing>;
		using weakptr = std::weak_ptr<someThing>;
		someThing() { std::cout << "something Construction" << std::endl; }
		~someThing() { std::cout << "something Destruction" << std::endl; }

		//��ؐ����
		someThing(const someThing& other) { std::cout << "something is copy constructor"; }
		someThing(const someThing&& other) { std::cout << "something is move constructor"; }

		uniqueptr getUniquePtr()
		{
			return std::make_unique<someThing>();
		}
		shardptr getshardPtr()
		{
			return std::make_shared<someThing>();
		}
		weakptr getWeakPtr(shardptr& ptr)
		{
			return weakptr(ptr);
		}

		
		uniqueptr moveTest(uniqueptr ptr)
		{
			return ptr;
		}
	};

	auto testptr() ->void
	{
		someThing s1;
		std::cout << "try copy and move constructor" << std::endl;
		someThing s2(s1);
		someThing s3(std::move(s2));
		std::cout << "try unique_Ptr" << std::endl;
		auto uptr1 = s1.getUniquePtr();
		auto uptr2 = s2.moveTest(std::move(uptr1));
		//auto uptr3 = s2.moveTest(uptr2);
		std::cout << "try shard_Ptr" << std::endl;

		auto shard_intp = std::shared_ptr<int>();
		if (shard_intp) std::cout << "ptr is initialized" << std::endl;
		else  std::cout << "ptr is not initialized" << std::endl;
		someThing s4;
		auto shard_somep1 = s4.getshardPtr();
#if !_HAS_CXX20 //shared_ptr::unique has been removed from C++20
		if (shard_somep1.unique()) std::cout << "shard_somep1 is unique" << std::endl; 
		else std::cout << "shard_somep1 is not unique" << std::endl;
		std::cout << "shard_somep1's point counter" << shard_somep1.use_count()<< std::endl;
		std::cout << "after share:" << std::endl;
		auto shard_somep2 = shard_somep1;
		if (shard_somep1.unique()) std::cout << "shard_somep1 is unique" << std::endl;
		else std::cout << "shard_somep1 is not unique" << std::endl;
		std::cout << "shard_somep1's point counter" << shard_somep1.use_count() << std::endl;
		if (shard_somep2.unique()) std::cout << "shard_somep1 is unique" << std::endl;
		else std::cout << "shard_somep1 is not unique" << std::endl;
		std::cout << "shard_somep1's point counter" << shard_somep2.use_count() << std::endl;

#endif
		std::cout << "try weak ptr" << std::endl;
		
		auto shard_s5 = std::make_shared<int>(1234);
		auto weakptr = std::weak_ptr<int>(shard_s5);
		if (weakptr.expired()) std::cout << "weak_ptr is expired" << std::endl;
		else std::cout << "weak_ptr is  not expired" << std::endl;
		if(weakptr.lock())std::cout << "weak_ptr is locked, value is :" << *(weakptr.lock()) << std::endl;
		else
		{
			std::cout << "weak_ptr is unlock" << std::endl;
			weakptr.reset();
		}
		shard_s5 = nullptr;
		std::cout << std::endl;
		std::cout << "now release the shared_ptr" << std::endl;
		std::cout << std::endl;
		if (weakptr.expired()) std::cout << "weak_ptr is expired" << std::endl;
		else std::cout << "weak_ptr is  not expired" << std::endl;
		if (weakptr.lock())std::cout << "weak_ptr is locked, value is :" << *(weakptr.lock()) << std::endl;
		else
		{
			std::cout << "weak_ptr is unlock" << std::endl;
			weakptr.reset();
		}

	}
	//tuple, ����������tuple ʹ�÷���Ոȥfunctional
	auto testtuple() -> void
	{
		std::tuple<int, std::string, bool> t1(1, "Jack", true);
		auto t2 = std::make_tuple(2, "rose", false);
		std::cout << "element in tuple1"  <<std::endl;
		std::cout << std::get<0>(t1) << std::endl;
		std::cout << std::get<1>(t1) << std::endl;
		std::cout << (true == std::get<2>(t1) ? "male": "female") << std::endl;
		int id;
		std::string name;
		bool gender;
		std::cout << "element in tuple2" << std::endl;
		std::tie(id, name, gender) = t2;
		std::cout << id << std::endl;
		std::cout << name << std::endl;
		std::cout << (true == gender ? "male" : "female") << std::endl;
	}


	//׌�҂�ԇԇC++�ṩ���µĕr�g���P����
	auto testchrono() ->void
	{
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

		std::cout << "printing out 1000 stars...\n";
		for (int i = 0; i < 1000; ++i) std::cout << "*";
		std::cout << std::endl;

		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

		std::cout << "It took me " << time_span.count() << " seconds.";
		std::cout << std::endl;
	}




	auto main() -> int
	{
		testtypeid();
		testdecltype_1();
		testdecltype_2();
		non_menber_iterator();
		range_base_for();
		lambda_sample();
		genericlambda_sample();
		testptr();
		testtuple();
		testchrono();
		
	

		return 0;
	}
}



#endif // end of define 

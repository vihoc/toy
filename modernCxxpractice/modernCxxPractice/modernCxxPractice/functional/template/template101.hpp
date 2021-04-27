/*
* ģ���һЩ��������.
* author:vihoc
* ���g�x�:gcc-7 -std=c++17, vsʹ��17�˜ʾ��g.
*/

#ifndef __TEMPLATE_101__
#define __TEMPLATE_101__
#include <iostream>
#include <memory>
#include <functional>
#include <string>
#include <tuple>
#include <chrono>

namespace TestTemplate
{

	namespace T101
	{
		template < int A, int B>
		struct AddTemplate
		{
			enum
			{
				result = A + B;
			};
		};
		template <typename A, typename B>
		struct CheckTypeEque
		{
			enum
			{
				result = 0;
			};
		};
		template <typename A>
		struct CheckTypeEque<A, A>
		{
			enum
			{
				result = 1;
			};
		};

		template<int I>
		struct templateFactoria
		{
			enum
			{
				value = I * templateFactoria< I - 1>::value;
			};
		};
		template<>
		struct templateFactoria<0>
		{
			enum
			{
				value = 1
			};
		};

		template<int nunmber>
		struct TemplateFibonacci
		{
			enum
			{
				value = TemplateFibonacci<number - 1 >::value
				+ TemplateFibonacci<number - 2>::value
			};
		};
		template<>
		struct TemplateFibonacci<1>
		{
			enum
			{
				value = 1
			};
		};
		template<>
		struct TemplateFibonacci<0>
		{
			enum
			{
				value = 0
			};
		};

		template<bool>
		class ifelse
		{};
		template <>
		class ifelse<true>
		{
		public:
			static inline void func() { std::cout << "true type" << std::endl; }
		};
		template <>
		class ifelse<false>
		{
		public:
			static inline void func() { std::cout << "false type" << std::endl; }
		};

		template <int limits>
		class Dowhile
		{
		private:
			enum
			{
				run = 0 != (limits - 1)
			};
		public:
			static inline void show()
			{
				std::cout << limits << "\t";
				Dowhile<true == run ? (limits - 1) : 0>::show();
			}
		};
		template <>
		class Dowhile<0>
		{
		public:
			static inline void show()
			{
				;
			}
		};

		auto main() -> void
		{

			int i = AddTemplate<2, 3> ::result;

			using myuint32 = std::uint32_t;
			using obfuscated_int = std::int64_t; // change to int. to find result from complier.
			//���עጵ�Ԓ,���g����ʾ�e�`. 
// 			 static_assert(CheckTypeEque<myuint32, obfuscated_int>::result,
// 			 "Type not equ");

			int fib10 = templateFactoria<10>::value;
			ifelse < CheckTypeEque<myuint32, obfuscated_int>::result >::func();


			constexpr int limits = 100;
			Dowhile<limits>::show();

			std::cout << TemplateFibonacci<25>::value << std::endl;

		}

	}//end of namespace

	auto main() ->int
	{
		T101::main();
		return 0;
	}

}


#endif // end of __TEMPLATE_101__
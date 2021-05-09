//function101.hpp  C++����ʽ�������T���A����
//author: vihoc
//���g�x�:gcc-7 -std=c++17, vsʹ��17�˜ʾ��g.
// ���ʹ��:���ļ��A���麯��ʽ�����e��, ����Ǻ���ʽ����˼����e��,�t�ļ�����functional*.hpp �����ʹ��rangev3�����Ĵ��a,�tʹ��range*.hpp
//���ļ���functional ����˼�����ӻ��ߌ��F
#ifndef __Function_101_H__
#define __Function_101_H__

#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <functional>
#include <chrono>
#include <map>
#include <tuple>
#include <fstream>
#include <iostream>

//�����@�δ��a߀�]��÷������Y
namespace functional
{
	namespace starter
	{
		//ʲ���Ǻ���ʽ����
		//�팑һ��C++����ʽ��hallo world��
		auto helloworld() ->void
		{
			std::string hello = "Hello World.";
			copy(begin(hello), end(hello), std::ostream_iterator<char>(std::cout));
		}

		//׌�҂��ف�һ�����Ӱ�
		//��һ�����ļ��M�в����^,������_һ���ļ�, ѭ�h�鿴�Ƿ��eof,getline()һ��һ���xȡ
		//׌�҂����������C++�M�к���ʽ�����xȡһ���ļ�
		auto testReadFullFile(const std::string& filename)
		{
			std::ifstream input(filename);
			std::string buffer;
			std::copy(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>(), std::back_inserter(buffer));
			return buffer;
		}
		class Line : std::string
		{
			friend std::istream& operator>>(std::istream& is, Line& line)
			{
				return std::getline(is, line);
			}
		};

// 		auto tryGetline(const std::string& filename)
// 		{
// 			std::ifstream input(filename);
// 			std::vector<std::string> buffer;
// 			while (!input.eof())
// 			{
// 				std::string stringbuffer;
// 				std::copy(std::istream_iterator<Line>(input), std::istream_iterator<Line>(), std::back_inserter(stringbuffer));
// 				input.seekg(stringbuffer.size());
// 				buffer.emplace_back();
// 			}
// 		}


		auto main() ->void
		{
			helloworld();
		}
	}


	namespace f101
	{
		//�����@�δ��a߀�]��÷������Y
		template <typename A, typename B, typename C>
		std::function<C(A)> compose(
			std::function<C(B)> f,
			std::function<B(A)> g) {
			return [f, g](A x) {
				return f(g(x));
			};
		}
		
		//�\�Еr�o������������������
		//�������M�ж��A׃��
		auto testfunctioncomposition() ->void
		{
			typedef std::function<double(double)> hyperbolicFunc;
			//sinh, cosh, tanh g++-7�ܾ��g,vc���� ������ƌ���� 
			std::vector<hyperbolicFunc> func = { 
				[](double x)->double {return sinh(x); },
				[](double x)->double {return cosh(x); }, 
				[](double x)->double {return tanh(x); },
				[](double x)->double{return x * x;}
			};
			std::vector<hyperbolicFunc> inverseFuncs = {
				[](double x)->double {return asinh(x); },
				[](double x)->double {return acosh(x); },
				[](double x)->double {return atanh(x); },
				[](double x) {return exp(log(x) / 2); }
			};
			
			std::vector<hyperbolicFunc> composedFuncs;

			
			std::vector<double> nums;
			for (int i = 1; i <= 5; ++i)
				nums.push_back(i * 0.2);

			std::transform(
				std::begin(inverseFuncs),
				std::end(inverseFuncs),
				std::begin(func),
				std::back_inserter(composedFuncs),
				compose<double, double, double>);

			for (auto num : nums)
			{
				for (auto func : composedFuncs)
					std::cout << "f(g(" << num << ")) = " << func(num) << std::endl;

				std::cout << "---------------" << std::endl;
			}

		}
		//����ʽ����101:����һ���xֵ,�K��׃.���º������F�����������������M�и�׃,���ǌ��Y���惦���~��Ĕ����Y����.
		// ��춺���ʽ���́��v,���}�ε��\��ĳ������,�Y����Ҫ��һ�ӵ�.
		//���A����map�e��, map�ǌ�һ�����еČ�������M��ĳ�Ӌ��,�������׃�ķ��ؽY����һ�N������ָ.
		//�@�Y���]�Ќ��Fmap, ������std::transform���.
		auto testmap()->void
		{
			std::vector<int> v1;
			for (int i = 0; i < 5; ++i)
			{
				v1.emplace_back(i);
			}
			std::vector<int> v2;
			v2.reserve(v1.size());
			std::transform(begin(v1), end(v1), std::back_inserter(v2), [](int i) {return i * i; });
			std::cout << "after map operation" << std::endl;
			std::copy(begin(v2), end(v2), std::ostream_iterator<int>(std::cout, " "));
		}

		//���A����filter�e��, filter��ָ��һ�����еČ����M�в���,����һ���µĔ�������, �����҂��O�õėl���������M���^�V
		//�@�YҲ�]�Ќ��Fmap.ʹ��std::copy_if��remove_copy_if����.
		auto testfilter()-> void
		{
			std::vector<int> v1;
			for (int i = 0; i <= 5; ++i)
			{
				v1.emplace_back(i);
			}
			std::vector<int> v2;
			v2.reserve(v1.size());
			std::vector<int> v3;
			v3.reserve(v1.size());
			std::cout << "after filter operation" << std::endl;
			std::cout << "primes queue" << std::endl;
			std::copy_if(std::begin(v1), std::end(v1), std::back_inserter(v2),
				[](int n) ->bool
				{
					if (n < 2) return 0 != n ? true : false;
					else
					{
						for (int j = 2; j < n; ++j) if (0 == n % j) return false;
					}
					return true;
				}
			);
			std::copy(std::begin(v2), std::end(v2), std::ostream_iterator<int>(std::cout, " "));
			std::cout << std::endl;
			std::cout << "non primes queue" << std::endl;
			std::remove_copy_if(std::begin(v1), std::end(v1), std::back_inserter(v3),
				[](int n) -> bool
				{
					if (n < 2) return 0 != n ? true : false;
					else
					{
						for (int j = 2; j < n; ++j) if (0 == n % j) return false;
					}
					return true;
				}
			);
			std::copy(std::begin(v3), std::end(v3), std::ostream_iterator<int>(std::cout, " "));
			std::cout << std::endl;
		}
		//���A����fold, ���ڌ�����Ӌ��K�p����1���Y��. 
		//foldl: (((0 + 1) + 2) + 3)
		//foldr  (0 + (1 + (2 + 3)))
		//�@�YҲ�]�Ќ��F,����ʹ��stl�ĺ����������M���n��
		auto testfold() -> void
		{
			std::vector<int> number = { 0, 1, 2, 3 };
			auto foldl = std::accumulate(std::begin(number), std::end(number), 0, std::plus<int>());
			auto foldr = std::accumulate(std::rbegin(number), std::rend(number), 0, std::plus<int>());
			std::cout << "foldl result: " << foldl << std::endl;
			std::cout << "foldr result: " << foldr << std::endl;
		}
		/*currying:����׃�Q�p�ٺ����ą���, �e�� auto add(int x, int y) -> int
		*		{[](int x) {return [&](int y){return x + y;}}}
		*		std::bind()Ҳ��curry��һ�N
		* �@�Y���e��ʹ���˿�׃����,��ҪԔ���t��,���Բ��template�ļ��A��variadic templates ���P�ļ�.
		*/
		template<typename Func, typename... Args>
		auto curry(Func func, Args... args)
		{
			return [=](auto... lastParam)
			{
				return func(args..., lastParam...);
			};
		}
		auto areaofRectangle(int length, int width) ->int
		{
			return length * width;
		}
		auto volumeOfRectangle(int length, int width, int height) ->int
		{
			return length * width * height;
		}
		auto testcurrying() ->void
		{
			constexpr int maxlength = 5;
			auto area = curry(areaofRectangle, maxlength);
			for (int i = 0; i < maxlength; ++i)
			{
				std::cout << "length(" << i << ")";
				std::cout << area(i) << std::endl;
			}
			auto volume = curry(volumeOfRectangle, maxlength, maxlength - 1);
			for (int i = 0; i < maxlength; ++i)
			{
				std::cout << "length(" << i << ")";
				std::cout << volume(i) << std::endl;
			}
		}

	}

	//����,��ʽ�_ʼ�M�к��εĺ���ʽ���̾���
	auto immutablesample1()
	{
		std::vector<int> v1(10, 100);
		std::vector<int> v2(10, 0);
		//v1.emplace_back(100);
		int i = 0;
		std::transform(std::begin(v1), std::end(v1), std::back_inserter(v2), [&i](int x) {return x + 2 * i; });
		std::cout << "after transform " << std::endl;
		for (auto data : v2)
		{
			std::cout << data << " ";
		}
		std::cout << std::endl;
		auto foldl = std::accumulate(std::begin(v2), std::end(v2), 0, std::plus<int>());
		std::cout << "result after foldl: " << foldl << std::endl;
	}

	//next , we try to create a mutable object;
	namespace testmutable
	{
		class student
		{
		private:
			int id;
			std::string firstname; 
			std::string lastname;
			double GPA;
		public:
			student(const int otherid, const std::string& otherfirstname, const std::string& otherlastname, double otherGPA)
				:id(otherid), firstname(otherfirstname) , lastname(otherlastname), GPA(otherGPA){}
			student() = delete;
			~student() = default;
			const int getID() const { return id; }
			const std::string& getFirstName()const { return firstname; }
			
			const std::string& getLastName()const { return lastname; }
			const double getGPA() const { return GPA; }
			const student setStudentId(const int newid) const { return student(newid, firstname, lastname, GPA);  }
			const student setStudentFirstName(const std::string& newfirstname) const { return student(id, newfirstname, lastname, GPA); }
			const student setStudentLastName (const std::string& newlastnamd) const { return student(id, firstname, newlastnamd, GPA); }
			const student setStudentGpa(const double newgpa) const { return student(id, firstname, lastname, newgpa); }
			void printstudentInfo() const
			{
				std::cout << "Content of this student" << std::endl;
				std::cout << "ID:" << id << std::endl;
				std::cout << "name:" << firstname << " " << lastname << std::endl;
				std::cout << "GPA:" << GPA << std::endl;
			}
		};
	}
	auto testMutableclass() -> void
	{
		using student = testmutable::student;
		student s1(1, "viho", "chen", 0.0);
		s1.printstudentInfo();
		auto s2 = s1.setStudentId(2);
		auto s3 = s2.setStudentFirstName("Alex");
		auto s4 = s3.setStudentLastName("Wang");
		auto s5 = s4.setStudentGpa(5.0);
		s5.printstudentInfo();
	}

	//�ں���ʽ������, curry׃�Q���Բ������_(strict evaluation)��׃�Q����(���� testcurrying), �@�Ӻ������Р���һ��.
	//Ҳ���Բ�ȡ���f�����_(non-strict)�ĺ���(����:compose), ���������\�㼚���[���ڂ��f�M��ĺ�����. ��ʹ��lambda, ��׃�Q�������oʹ�Ô����ĳ���T.
	namespace testLazyeva
	{
		using std::function;
		template<typename T>
		class Delay
		{
		private:
			function<T()> func;
		public:
			Delay(function<T()> ofunc)
			: func(ofunc)
			{
			}
			T fetch()
			{
				return std::invoke(func);
			}
		};

		template<typename T>class memoizetion
		{
		private:
			T const& (*sub)(memoizetion*);
			mutable T Recordingfunc;
			function<T()> func;

			static T const& focussubRoutine(memoizetion* d)
			{
				return d->doRecoding();
			}

			static T const& fetchsubRoutine(memoizetion* d)
			{
				return d->doFetch();
			}

			T const& doFetch()
			{
				return Recordingfunc;
			}

			T const& doRecoding()
			{
				Recordingfunc = func();
				sub = &fetchsubRoutine; //�@�Ymen_fn�����e,ע��Ҫ�����aŪ���
				return doFetch();
			}

		public:
			memoizetion(function<T()> ifunc) :func(ifunc),
				sub(&focussubRoutine),
				Recordingfunc(T())

			{}
			auto fetch()->T
			{
				return sub(this);
			}
		};
		int fibonacci(int n)
		{
			if (n <= 1) return n;
			return fibonacci(n - 1) + fibonacci(n - 2);
		}
		//for here, we write those code in header file, should be template<>
		class fibnaccimem
		{
		public:
			fibnaccimem() :value{ 0, 1 } {};
			~fibnaccimem() = default;
			unsigned int operator()(unsigned int n) const
			{
				if (value.size() > n) return value[n];
				else
				{
					const auto result = operator()(n - 1) + operator()(n - 2);
					value.emplace_back(result);
					return result;
				}
			}
		private:
			mutable std::vector<unsigned int> value;
		};


		template <typename Result, typename... Arg>
		auto make_memoized(Result (*f)(Arg...))
		{
			std::map<std::tuple<Arg...>, Result> cache;
			return [f, cache](Arg... args) mutable ->Result
			{
				const auto arg_tuple = std::make_tuple(args...);
				const auto cached = cache.find(arg_tuple);
				if (cached == cache.end())
				{
					auto result = f(args...);
					cache[arg_tuple] = result;
					return result;
				}
				else return cached->second;
			};
		}
		template <typename Result, typename... Arg>
		auto make_memoized(std::function<Result (Arg...)> f)
		{
			std::map<std::tuple<Arg...>, Result> cache;
			return [f, cache](Arg... args) mutable ->Result
			{
				const auto arg_tuple = std::make_tuple(args...);
				const auto cached = cache.find(arg_tuple);
				if (cached == cache.end())
				{
					auto result = f(args...);
					cache[arg_tuple] = result;
					return result;
				}
				else return cached->second;
			};
		}

	}

	auto testDelay() -> void
	{
		constexpr int a = 10, b = 5;
		int multexe = 0;
		testLazyeva::Delay<int> multiply([&]()
			{
				return a * b;
			});
		static_assert(0 != b , "oops, division by 0 is not permit");//�������b = 5, ���ĳ�b = 0,ԇԇ��
		testLazyeva::Delay<int> devision([=]()
			{
				return a / b;
			});
		std::cout << "result of a * b:" << multiply.fetch() << std::endl;
		std::cout << "result of a / b:" << devision.fetch() << std::endl;
		//���º����mȻ��Ӌ���ṩ�˸���ď���,����߀ӛ�Æ�? ����ʽ���̵���Ҫһ�h��,�o�����\��,����׃������r��,�Y��Ҫһ��.
		testLazyeva::Delay<int> multiplyexer([&]()
			{
				return multexe * a * b;
			});
		for (; multexe < 5; ++multexe)
		{
			std::cout << "result of multexe * a * b:" << multiplyexer.fetch() << std::endl;
		}
		//����, ����ԓ�@����, ����, �����Ĵ��a�o�e���õĕr��, �e���@������, Ҳ����ԓ׌���ó�
		//ͨ�^��׽׃�����Fӛ������ (memoization technique)
		testLazyeva::memoizetion<int> purefun_multi([&]()
			{
				return multexe * a * b;
			});
		for (; multexe > 0; --multexe)
		{
			std::cout << "result of multexe * a * b:" << purefun_multi.fetch() << std::endl;
		}

	}
	auto testFib() -> void
	{
		testLazyeva::fibnaccimem lazy;
		for(int i = 0; i < 50; ++i)
		{
			std::cout << lazy(i) << " " <<std::endl;
		}
	}

	auto testmakemem()->void
	{
		std::vector<unsigned int> value = { 0, 1 };
		std::function<unsigned int(unsigned int)> fib;
		fib = [&value, &fib](unsigned int n)
		{
			if (value.size() > n) return value[n];
			else
			{
				auto result = fib(n - 1) + fib(n - 2);
				value.emplace_back(result);
				return result;
			}
//			return 0 == n ? 0 : 1 == n ? 1 : fib(n - 1) + fib(n - 2);
		};
		auto fibomem = testLazyeva::make_memoized(fib);
		for (int i = 0; i < 50; ++i)
		{
			std::cout << fibomem(i) << " " << std::endl;
		}
	}


	namespace lazystring_concat
	{
		template <typename... String>
		class lazy_string_concat_helper;
		template <typename LastString, typename... String>
		class lazy_string_concat_helper<LastString, String...>
		{
		private:
			const LastString& data;
			lazy_string_concat_helper<String... > tail;
		public:
			lazy_string_concat_helper(const LastString& otherData, lazy_string_concat_helper<String...> otherTail)
				:data(otherData), tail(otherTail)
			{}
			std::size_t size()const { return data.size() + tail.size(); }
			template <typename It>
			void save(It end) const
			{
				const auto begin = end - data.size();
				std::copy(data.cbegin(), data.cend(), begin);
				tail.save(begin);
			}

			operator std::string()const
			{
				std::string result(size(), '\0');
				save(result.end());
				return result;
			}


			lazy_string_concat_helper<std::string, LastString, String...>
				operator + (const std::string& other)const 
			{
				return lazy_string_concat_helper<std::string, LastString, String...>(other, *this);
			}
			lazy_string_concat_helper<std::string, LastString, String...>
				operator + (const std::string&& other)const
			{
				return lazy_string_concat_helper<std::string, LastString, String...>(other, *this);
			}

		};
		template <>
		class lazy_string_concat_helper<>
		{
		private:
			
		public:
			lazy_string_concat_helper()
			{}
			int size() const { return 0; }
			template <typename It>
			void save(It end) const
			{
				
			}


			lazy_string_concat_helper<std::string>
				operator + (const std::string& other)const
			{
				return lazy_string_concat_helper<std::string>(other, *this);
			}
			lazy_string_concat_helper<std::string>
				operator + (const std::string&& other)const
			{
				return lazy_string_concat_helper<std::string>(other, *this);
			}

		};

		template <typename Stream, typename... Strings>
		Stream& operator<<(Stream& stream,
			const lazy_string_concat_helper<Strings...>& strings)
		{
			return stream << static_cast<std::string>(strings);
		}


	}


	auto testlazystring_concat() ->void
	{
		std::string firstName = "viho";
		std::string lastName = "zhu";
		lazystring_concat::lazy_string_concat_helper<> lazy;
		const auto result = lazy + "OMG, it's " + firstName + ", " + lastName + " sama";
		lastName = "chen";
		const std::string resultstring = lazy + "OMG, it's " + firstName + ", " + lastName + " sama";
		std::cout << result << std::endl;
		std::cout << resultstring << std::endl;
	
	}

	auto countTime() -> void
	{
		constexpr int maxcount = 5;
		constexpr int step = 5;//��Ҫ�ĳ�10 Ӌ��fibi(50) �r�g���L23333
		//����ԇԇ���e������ chrono::high_resolution_clock���P����ȥ��cpp101
		std::vector<int> v1;
		for (int i = 1; i <= maxcount; ++i)
		{
			v1.emplace_back(i * step); 
		}
		std::vector<int> result;
		auto start = std::chrono::high_resolution_clock::now();

		//a[](int x) {return testLazyeva::fibonacci(x);
		std::transform(std::begin(v1), std::end(v1), std::back_inserter(result), 
			[](int x)
			{
				testLazyeva::Delay<int> deley([&]() {return testLazyeva::fibonacci(x); });
				return deley.fetch();
			});
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> elapsed = finish - start;
		std::cout << "consuming time = " << elapsed.count() << " milliseconds " << std::endl;
		std::cout << "result of the vector is :";
		for (auto r : result)
		{
			std::cout << r << " ";
		}
		std::cout << std::endl;

		//ʹ���҂�֮ǰ���� "ӛ���w����" �@�r�򿴿��^�e
		std::vector<int> v2;
		for (int i = 1; i <= maxcount; ++i)
		{
			v2.emplace_back(40);
		}
		std::vector<int> result2;
		start = std::chrono::high_resolution_clock::now();
		//�r�g���c�L,�����ұ��C,������ѭ�h �P�߹Pӛ��i7������40��
		std::transform(std::begin(v2), std::end(v2), std::back_inserter(result2),
			[](int x)
			{
				testLazyeva::memoizetion<int> memfib([&]() {return testLazyeva::fibonacci(x); });
				return memfib.fetch();
			});
		finish = std::chrono::high_resolution_clock::now();
		elapsed = finish - start;
		std::cout << "consuming time = " << elapsed.count() << " milliseconds " << std::endl;
		std::cout << "result of the vector is :";
		std::copy(std::begin(v2), std::end(v2), std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;

		std::cout << "It's try count the time to single object" << std::endl;
		
		testLazyeva::memoizetion<int> memtest([&]() {return testLazyeva::fibonacci(40); });
		for (int i = 1; i <= maxcount; ++i)
		{
			start = std::chrono::high_resolution_clock::now();
			memtest.fetch();
			finish = std::chrono::high_resolution_clock::now();
			elapsed = finish - start;
			std::cout << "consuming time = " << elapsed.count() << " milliseconds " << std::endl;
		}
		//wow
		
	}
	namespace trylazy 
	{
		template <typename T>
		class Row;
		template<typename T>
		class chunk
		{
		protected:
			T value;
			Row<T> last_row;
		public:
			chunk() {}
			chunk(T ovalue, Row<T> other_row) :value(ovalue), last_row(std::move(other_row)) {}
			explicit chunk(T ovalue) : value(ovalue) {}
			const T Value() const { return value; }
			const Row<T> LastRow() const { return last_row; }
		};

		template<class T> class chunkPreparation
		{
		public:
			T m_value;
			Row<T> m_row;

			chunkPreparation(T value, Row<T> row) :
				m_value(value),
				m_row(std::move(row))
			{
			}

			explicit chunkPreparation(T value) :
				m_value(value)
			{
			}

			chunk<T> operator()()
			{
				return chunk<T>(
					m_value,
					m_row);
			}
		};

		template<typename  T> class Row
		{
			using valuechunk = testLazyeva::memoizetion<chunk<T>>;
		private:
			std::shared_ptr <valuechunk> lazyChunk;

		public:
			Row()
			{
			}

			explicit Row(T value)
			{
				auto chunk = chunkPreparation<T>(value);
				lazyChunk = std::make_shared<valuechunk >(
					chunk);
			}

			Row(T value, Row row)
			{
				auto chunk = chunkPreparation<T>(
					value,
					std::move(row));

				lazyChunk = std::make_shared<valuechunk>(
					chunk);
			}

			Row(std::function<chunk<T>()> func)
				: lazyChunk(
					std::make_shared<valuechunk>(func))
			{
			}

			bool IsEmpty() const
			{
				return !lazyChunk;
			}

			T Fetch() const
			{
				return lazyChunk->Fetch().Value();
			}

			Row<T> LastRow() const
			{
				return lazyChunk->Fetch().LastRow();
			}

			Row Pick(int n) const
			{
				if (n == 0 || IsEmpty())
					return Row();

				auto chunk = lazyChunk;
				return Row([chunk, n]()
					{
						auto val = chunk->Fetch().Value();
						auto row = chunk->Fetch().LastRow();
						return chunk<T>(val, row.Pick(n - 1));
					});
			}
			
		};//end of Row
		template<class T> Row<T> ConcatenateRows(
			Row<T> leftRow,
			Row<T> rightRow)
		{
			if (leftRow.IsEmpty())
				return rightRow;

			return Row<T>([=]()
				{
					return chunk<T>(
						leftRow.Fetch(),
						ConcatenateRows<T>(
							leftRow.LastRow(),
							rightRow));
				});
		}
		template<class T, class U> void ForEach(
			Row<T> row, U func)
		{
			while (!row.IsEmpty())
			{
				func(row.Fetch());
				row = row.ShiftLastToFirst();
			}
		}
	}//end of trylazy
	//�@����߀δ���




	auto main() -> int
	{
		f101::testfunctioncomposition();
		f101::testmap();
		f101::testfilter();
		f101::testfold();
		f101::testcurrying();
		immutablesample1();
		testMutableclass();
		testDelay();
		testFib();
		testmakemem();
		testlazystring_concat();
		countTime();
		return 0;
	}
}




#endif // end of define

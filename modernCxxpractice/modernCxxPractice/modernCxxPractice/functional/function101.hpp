//function101.hpp  C++函數式編程入門基礎部分
//author: vihoc
//編譯選項:gcc-7 -std=c++17, vs使用17標準編譯.
// 如何使用:本文件夾以下為函數式編程舉例, 如果是函數式本身思想的舉例,則文件名為functional*.hpp 如果是使用rangev3編寫的代碼,則使用range*.hpp
//本文件為functional 基本思想例子或者實現
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

//以下這段代碼還沒想好放在那裏
namespace functional
{
	namespace starter
	{
		//什麽是函數式編程
		//來寫一個C++函數式的hallo world吧
		auto helloworld() ->void
		{
			std::string hello = "Hello World.";
			copy(begin(hello), end(hello), std::ostream_iterator<char>(std::cout));
		}

		//讓我們再來看一個例子吧
		//你一定對文件進行操作過,當你打開一個文件, 循環查看是否為eof,getline()一行一行讀取
		//讓我們來看看如果是C++進行函數式編程讀取一個文件
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
		//以下這段代碼還沒想好放在那裏
		template <typename A, typename B, typename C>
		std::function<C(A)> compose(
			std::function<C(B)> f,
			std::function<B(A)> g) {
			return [f, g](A x) {
				return f(g(x));
			};
		}
		
		//運行時給函數綁定函數的例子
		//將函數進行多階變化
		auto testfunctioncomposition() ->void
		{
			typedef std::function<double(double)> hyperbolicFunc;
			//sinh, cosh, tanh g++-7能編譯,vc不能 死活不能推導類型 
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
		//函數式編程101:數據一次賦值,終身不變.以下函數實現均爲不對數據本身進行改變,而是將結果存儲到額外的數據結構内.
		// 對於函數式編程來講,重複次的運行某個函數,結果需要是一樣的.
		//高階函數map舉例, map是對一個序列的對象按順序進行某項計算,最後順序不變的返回結果的一種操作代指.
		//這裏并沒有實現map, 而是用std::transform替代.
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

		//高階函數filter舉例, filter是指用一個序列的對象進行操作,生成一個新的數據對象, 根據我們設置的條件對數據進行過濾
		//這裏也沒有實現map.使用std::copy_if和remove_copy_if代替.
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
		//高階函數fold, 用于將數據計算並減少至1個結果. 
		//foldl: (((0 + 1) + 2) + 3)
		//foldr  (0 + (1 + (2 + 3)))
		//這裏也沒有實現,而是使用stl的函數對數據進行纍加
		auto testfold() -> void
		{
			std::vector<int> number = { 0, 1, 2, 3 };
			auto foldl = std::accumulate(std::begin(number), std::end(number), 0, std::plus<int>());
			auto foldr = std::accumulate(std::rbegin(number), std::rend(number), 0, std::plus<int>());
			std::cout << "foldl result: " << foldl << std::endl;
			std::cout << "foldr result: " << foldr << std::endl;
		}
		/*currying:利用變換減少函數的參數, 舉例 auto add(int x, int y) -> int
		*		{[](int x) {return [&](int y){return x + y;}}}
		*		std::bind()也是curry的一種
		* 這裏的舉例使用了可變參數,想要詳細瞭解,可以查閲template文件夾下variadic templates 相關文件.
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

	//好了,正式開始進行簡單的函數式編程練習
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

	//在函數式編程中, curry變換可以采用明確(strict evaluation)的變換函數(例子 testcurrying), 這樣函數的行爲是一定.
	//也可以采取傳遞非明確(non-strict)的函數(例子:compose), 將參數的運算細節隱藏在傳遞進來的函數内. 而使用lambda, 將變換細節交給使用數據的程序員.
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
				sub = &fetchsubRoutine; //這裏men_fn會出錯,注意要寫代碼弄清楚
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
		static_assert(0 != b , "oops, division by 0 is not permit");//將上面的b = 5, 更改成b = 0,試試看
		testLazyeva::Delay<int> devision([=]()
			{
				return a / b;
			});
		std::cout << "result of a * b:" << multiply.fetch() << std::endl;
		std::cout << "result of a / b:" << devision.fetch() << std::endl;
		//以下函數雖然為計算提供了更多的彈性,但是還記得嗎? 函數式編程的重要一環是,無數次運行,不改變參數情況下,結果要一致.
		testLazyeva::Delay<int> multiplyexer([&]()
			{
				return multexe * a * b;
			});
		for (; multexe < 5; ++multexe)
		{
			std::cout << "result of multexe * a * b:" << multiplyexer.fetch() << std::endl;
		}
		//所以, 不應該這麽做, 至少, 如果你的代碼給別人用的時候, 別人這麽做了, 也不應該讓他得逞
		//通過捕捉變量實現記憶功能 (memoization technique)
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
		constexpr int step = 5;//不要改成10 計算fibi(50) 時間超長23333
		//接下來試試看別的作用 chrono::high_resolution_clock相關可以去看cpp101
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

		//使用我們之前寫的 "記憶體對象" 這時候看看區別
		std::vector<int> v2;
		for (int i = 1; i <= maxcount; ++i)
		{
			v2.emplace_back(40);
		}
		std::vector<int> result2;
		start = std::chrono::high_resolution_clock::now();
		//時間有點長,但是我保證,不是死循環 筆者筆記本i7都花了40秒
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
	//這部分還未完成




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

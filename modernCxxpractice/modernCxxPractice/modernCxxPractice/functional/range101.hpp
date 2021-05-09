#ifndef __RANGE_101__
#define __RANGE_101__
#include <iostream>
#include <all.hpp> // get everything
#include <string>
#include "utilities.hpp"

namespace rangeuse
{
	//let us getting start to range for this follow function, it general infinite number to screen.
	//如此代碼運行所示, range的編程思想已經跟傳統的oo,或者結構化編程大相徑庭了. 我們面對的已經不是一個區間[first, end)
	//而是可以看成數據是無限的,而我們處理的是一個view,個人覺得可以理解爲投影
	auto firstattemped()
	{
		
		for (int i : ranges::views::iota(1))
		{
			std::cout << i << ' ';
		}

// 		for (int i : ranges::views::iota(1)
// 			| ranges::views::transform([](int i) { return i * i; })
// 			| ranges::views::filter([](int i) { return i % 4 == 0; })
// 			| ranges::views::take(5)) {
// 			std::cout << i << ' ';
// 		}
	}


	namespace employee
	{
		//this class from the book <functional programming in c++>
		class person_t {
		public:
			enum gender_t {
				female,
				male,
				other
			};

			enum output_format_t {
				name_only,
				full_name
			};

			person_t()
				: m_name("John")
				, m_surname("Doe")
				, m_gender(other)
			{
			}

			person_t(std::string name, gender_t gender, int age = 0)
				: m_name(name)
				, m_surname("Doe")
				, m_gender(gender)
				, m_age(age)
			{
			}

			person_t(std::string name, const std::string& surname, gender_t gender, int age = 0)
				: m_name(name)
				, m_surname(surname)
				, m_gender(gender)
				, m_age(age)
			{
			}

			std::string name() const
			{
				return m_name;
			}

			std::string surname() const
			{
				return m_surname;
			}

			gender_t gender() const
			{
				return m_gender;
			}

			int age() const
			{
				return m_age;
			}

			void print(std::ostream& out,
				person_t::output_format_t format) const
			{
				if (format == person_t::name_only) {
					out << name() << '\n';

				}
				else if (format == person_t::full_name) {
					out << name() << ' '
						<< surname() << '\n';

				}
			}

		private:
			std::string m_name;
			std::string m_surname;
			gender_t m_gender;
			int m_age;
		};

		enum class Category { First, Second, Third };

		struct A
		{
			Category category;
			int value;

			constexpr A(int v) noexcept
				: category(static_cast<Category>(v % 3)),
				value(v)
			{ }

			constexpr int Value() const noexcept
			{
				return value;
			}
		};

		const char* to_string(Category category)
		{
			return category == Category::First ?
				"First" : category == Category::Second ? "Second" : "Third";
		}

		std::ostream& operator << (std::ostream& stream, const A& a)
		{
			stream << to_string(a.category) << ' ' << a.value << ' ';
			return stream;
		}

		auto rangeexample1()
		{
			std::vector<person_t> people{
				 { "David"  , person_t::male   },
				 { "Jane"   , person_t::female },
				 { "Martha" , person_t::female },
				 { "Peter"  , person_t::male   },
				 { "Rose"   , person_t::female },
				 { "Tom"    , person_t::male   }
			};
			auto is_Famale = [](const person_t& people)
			{
				return person_t::female == people.gender();
			};

			auto result = people 
				| ranges::views::filter(is_Famale)
				| ranges::views::transform([](const person_t& people) { return people.name(); })

				;//for here , semicolon in independent line, i can add my highorder function to above line with any mistake.

			std::cout << result << std::endl;
			int a = 1;
		}

		auto rangeexample2()
		{
			 
			int values[] = { 0,1,2,3,4,5 };

			auto items = values
				| ranges::views::transform([](int i) { return A(i); })
				| ranges::to<std::vector>()
				;
			std::copy(items.begin(), items.end(), std::ostream_iterator<A>(std::cout));
		}
	} //end of employee


	namespace selfviewexample
	{
		template<typename T>
		using IteratorBase = decltype(std::begin(std::declval<T&>()));



		template <typename Rng>
		class SquareView : public ranges::view_base
		{
		private:
			using RangeType = ranges::views::all_t<Rng>;
			RangeType range_;

			class IteratorType : public IteratorBase<Rng>
			{
			public:
				using Base = IteratorBase<Rng>;
				using value_type = typename std::iterator_traits<Base>::value_type;

				IteratorType() { }
				IteratorType(const Base& other) : Base(other) { }

				IteratorType& operator++()
				{
					++static_cast<Base&>(*this);
					return *this;
				}
				IteratorType operator++(int)
				{
					return static_cast<Base&>(*this)++;
				}
				// Where the magic happens
				value_type operator*() const
				{
					value_type value = *static_cast<Base>(*this);
					return value * value;
				}
			};

		public:
			using iterator = IteratorType;
			SquareView()
			{ }

			SquareView(Rng&& range)
				: range_(ranges::views::all(static_cast<Rng&&>(range)))
			{ }

			iterator begin() const { return ranges::begin(range_); }

			auto end() const { return ranges::end(range_); }
		};

		template <typename Rng>
		SquareView(Rng&&)->SquareView<Rng>;

		struct SquareFn
		{
			template <typename Rng>
			auto operator()(Rng&& range) const
			{
				return SquareView(std::forward<Rng>(range));
			}

			template <typename Rng>
			friend auto operator|(Rng&& range, const SquareFn&)
			{
				return SquareView(std::forward<Rng>(range));
			}
		};

		SquareFn constexpr Square;


		auto testexample1()
		{
			std::vector<int> inputValues{ 1,2,3,4,5,6,7 };

			auto view = inputValues
				| Square
				| ranges::views::drop(2)
				
				;
			//accumulate can not pipe;
			const auto result = ranges::accumulate(view, 0);
			return result;
		}

		
		constexpr size_t ValueCount = 1000000ULL;
		//u can change here for shorten the loop
		constexpr size_t IterationCount = 10000ULL;
		auto testPerformance()
		{
			auto RangeTest = []()
			{

				size_t result = 0;

				for (size_t i = 0; i < IterationCount; i++)
				{
					auto values = ranges::views::iota(0ULL, ValueCount)
						| Square;
					result += ranges::accumulate(values, 0);
				}
				return result;
			};
			size_t result;
			utilities::PerformanceTime_t time = utilities::TestFuncRunningTime(RangeTest, result);
			std::cout << "test range view,  time:" << time.count() << ". result :" << result << std::endl;
		}
	}//end of selfviewexample


	namespace selfviewadaptorexample
	{
		template<class Rng>
		class SquareView : public ranges::view_adaptor<SquareView<Rng>, Rng>
		{
			friend ranges::range_access;
			class adaptor : public ranges::adaptor_base
			{
			public:
				adaptor() = default;

				auto read(ranges::iterator_t<Rng> it) const -> decltype((*it)* (*it))
				{
					auto value = *it;
					return value * value;
				}
			};
			adaptor begin_adaptor() const { return { }; }
			adaptor end_adaptor() const { return { }; }
		public:
			SquareView() = default;
			SquareView(Rng&& rng)
				: SquareView::view_adaptor{ std::forward<Rng>(rng) }
			{}
		};

		// Deduction guideline needed to help the 
		// compiler make the right choice for us
		template <typename Rng>
		SquareView(Rng&&)->SquareView<Rng>;

		struct SquareFn
		{
			template <typename Rng>
			auto operator()(Rng&& range) const
			{
				return SquareView(std::forward<Rng>(range));
			}

			template <typename Rng>
			friend auto operator|(Rng&& range, const SquareFn&)
			{
				return SquareView(std::forward<Rng>(range));
			}
		};


		SquareFn constexpr Square;
		auto testexample()
		{
			std::vector inputValues{ 1,2,3,4,5,6,7 };

			auto result = inputValues
				| Square
				| ranges::views::drop(2);

			std::cout << ranges::views::all(result) << std::endl;
		}

		constexpr size_t ValueCount = 1000000ULL;
		//u can change here for shorten the loop
		constexpr size_t IterationCount = 1000ULL;
		auto testPerformance()
		{
			auto RangeTest = []()
			{

				size_t result = 0;

				for (size_t i = 0; i < IterationCount; i++)
				{
					auto values = ranges::views::iota(0ULL, ValueCount)
						| Square;
					result += ranges::accumulate(values, 0);
				}
				return result;
			};
			size_t result;
			utilities::PerformanceTime_t time = utilities::TestFuncRunningTime(RangeTest, result);
			std::cout << "test range view,  time:" << time.count() << ". result :" << result << std::endl;
		}

	}//end of selfviewadaptorexample

	int main()
	{
		//for this function, if u want test to the rest function, must be annotate, it's INFINITE loop.
		//firstattemped();

		employee::rangeexample1();
		employee::rangeexample2();
		selfviewexample::testexample1();

		//for this function, my default setting set a large number, it will take much time to run, but it's not endless loop;
		// u have my word.
		//selfviewexample::testPerformance();

		selfviewadaptorexample::testexample();

		selfviewadaptorexample::testPerformance();
		return 0;
	}//end of namespace range

}
#endif //end of __RANGE_101__
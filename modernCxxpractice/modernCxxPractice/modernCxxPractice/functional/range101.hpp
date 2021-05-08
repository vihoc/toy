#ifndef __RANGE_101__
#define __RANGE_101__
#include <iostream>
#include <all.hpp> // get everything
#include <string>

namespace rangeuse
{
	//let us getting start to range for this follow function, it general infinite number to screen.
	//如此代碼運行所示, range的編程思想已經跟傳統的oo,或者結構化編程大相徑庭了. 我們面對的已經不是一個區間[first, end)
	//而是可以看成數據是無限的,而我們處理的是一個view,個人覺得可以理解爲投影
	auto firstattemped()
	{
		//auto result = ranges::views::iota(1);
		for (int i : ranges::views::iota(1) /*| ranges::views::take(1000)*/)
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

		auto tryfilterfamale()
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

			// 			std::vector<int> a = { 1, 2, 3, 4 };
			// 			auto x = a | ranges::view::filter([](int x) {return x; });

			auto result = people | ranges::views::transform([](const person_t& data) { return data; })
				| ranges::views::filter(is_Famale)
				| ranges::views::transform([](const person_t& people) { return people.name(); })
				//| ranges::views::take(3)
				;

			std::cout << result << std::endl;
			int a = 1;
		}
	} //end of employee

	int main()
	{
		//firstattemped();
		employee::tryfilterfamale();
		return 0;
	}//end of namespace range

}
#endif //end of __RANGE_101__
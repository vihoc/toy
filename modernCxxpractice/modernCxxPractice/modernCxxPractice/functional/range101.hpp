#ifndef __RANGE_101__
#define __RANGE_101__
#include <iostream>
#include <all.hpp> // get everything
#include <string>

namespace rangeuse
{
	//let us getting start to range for this follow function, it general infinite number to screen.
	auto firstattemped()
	{
		for (int i : ranges::views::iota(1) | ranges::views::take(1000))
		{
			std::cout << i << ' ';
		}

		for (int i : ranges::views::iota(1)
			| ranges::views::transform([](int i) { return i * i; })
			| ranges::views::filter([](int i) { return i % 4 == 0; })
			| ranges::views::take(5)) {
			std::cout << i << ' ';
		}
	}

	int main()
	{
		firstattemped();

		return 0;
	}//end of namespace range

}
#endif //end of __RANGE_101__
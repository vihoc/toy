#ifndef __UTILITIES_H__
#define __UTILITIES_H__
#include <numeric>
#include  <chrono>
namespace utilities
{
	//œyÔ‡º¯”µµÄß\ÐÐ•rég ·µ»Østd::chrono::duration<double>
	template<typename Callback, typename T>
	auto TestFuncRunningTime(Callback cb, T& result)
	{
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		result = cb();
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
	}


	template<typename Range>
	inline auto println_range(Range const& range, const std::string& describe)
	{
		std::cout << describe <<std::endl;
		std::copy(begin(range), end(range), std::ostream_iterator<decltype(*begin(range))>(std::cout, " "));
		return std::cout << std::endl;
	}

	template<typename Printable>
	inline auto println(Printable const& to_print, const std::string& describe)
	{
		return std::cout << "describe" << to_print << std::endl;
	}

	template<typename Printable>
	inline auto println(Printable&& to_print, const std::string&& describe)
	{
		return std::cout << describe << std::move(to_print) << std::endl;
	}


}


#endif // end of __UTILITIES_H__

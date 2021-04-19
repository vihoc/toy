
//this file is a excise of the book: practical C++ meta programming
//the book author don't provide code. so i write one.
//author: vihoc

#include <iostream>
#include <type_traits>
#include <ctime>
#include <tuple>
namespace sence1
{
	//generate error when we complite code
    void sence_1()
    {
// 		using myuint32 = std::uint32_t;
// 		using obfuscated_int = std::int64_t; // change to int. to find result from complier.
// 		static_assert(std::is_same<myuint32, std::uint32_t>::value,
// 			"invaild integer detected");
// 		static_assert(std::is_same<obfuscated_int, std::int64_t>::value,
// 			"invaild integer detected"); // ERROR
// 		static_assert(4 == sizeof(obfuscated_int),
// 			"invaild integer size detected");
// 
// 
// 		//check 64bit platform
// 		static_assert(8 == sizeof(void*), "except 64 bit platfrom");
    }

	void sence_2()
	{
		//change platform to see the deference
		static const std::uint64_t dafault_buffer = std::conditional<8 == sizeof(void*),
		std::integral_constant<std::uint64_t, 100 * 1024 * 1024>,
		std::integral_constant<std::uint64_t, 1024 * 1024 * 1024>>::type::value;
		std::cout << dafault_buffer << std::endl;
	}
	
}

namespace sence2
{
	//assuming by the book, this function is in something like DLL. we can't change the funtion at all;
	//for here we need pass the compiler, so i give the empty body.
	void adjust_value(double* alpha1, double* beta1,
						double* alpha2, double* beta2)
	{
	}
	//and we need empty struct location
	struct location
	{

	};
	class Reading
	{
	public:
		//and this two function, we need a empty body again
		double alpha_value(location l, std::time_t t) const
		{
			return 0.0;
		}
		double beta_value(location l, std::time_t t) const
		{
			return 0.0;
		}
	};

	//here is our adapter prototype 1
	std::tuple<double, double, double, double> get_adjusted_values(
		const Reading& r, location l, std::time_t t1, std::time_t t2)
	{
		double alpha1 = r.alpha_value(l, t1);
		double beta1 = r.beta_value(l, t1);
		double alpha2 = r.alpha_value(l, t2);
		double beta2 = r.beta_value(l, t2);
		adjust_value(&alpha1, &beta1, &alpha2, &beta2);
		return std::make_tuple(alpha1, beta1, alpha2, alpha2);
	}

	//here is our template version adapter prototype1
	template<typename ReadingType>
	std::tuple<double, double, double, double> get_adjusted_values(
		const ReadingType& r, location l, std::time_t t1, std::time_t t2)
	{
		double alpha1 = r.alpha_value(l, t1);
		double beta1 = r.beta_value(l, t1);
		double alpha2 = r.alpha_value(l, t2);
		double beta2 = r.beta_value(l, t2);
		adjust_value(&alpha1, &beta1, &alpha2, &beta2);
		return std::make_tuple(alpha1, beta1, alpha2, alpha2);
	} // is  looks like useless. we still write some bullshit

	//here is our final template , since we use the template, we got power.
	template<typename Alpha_F, typename Beta_F>
	std::tuple<double, double, double, double> get_adjusted_values(
		Alpha_F alphaValue, Beta_F bateValue, location l, std::time_t t1, std::time_t t2)
	{
		double alpha1 = alphaValue(l, t1);
		double beta1 = bateValue(l, t1);
		double alpha2 = alphaValue(l, t2);
		double beta2 = bateValue(l, t2);
		adjust_value(&alpha1, &beta1, &alpha2, &beta2);
		return std::make_tuple(alpha1, beta1, alpha2, alpha2);
	}
	//now time to test
	void test_adapter()
	{
		Reading r;
		location l;
		std::time_t time1 = std::time(nullptr);
		std::time_t time2 = std::time(nullptr);
		//do some Initialing 
		auto res = get_adjusted_values(
			[&r](location loca, std::time_t t) ->double {return r.alpha_value(loca, t); },
			[&r](location loca, std::time_t t) ->double {return r.beta_value(loca, t); },
			l, time1, time2);
	}
	//well done, it's looks readable


}
//hold tight, the follow is section of magic 
//全是奇婬技巧, 人爲了增加編譯時間去光明正大摸魚也是夠拼的(雖然有用
namespace sence3
{
	//the follow function show you a pass a return value's address to another function which take a pointer parameter.
	// the point of this is those function take the some data type. 
	template<typename ValueFunction, typename PointerFunction>
	double magic_wang(ValueFunction vf, PointerFunction pf, double dparam)
	{
		double v = vf(dparam);
		pf(&v);
		return v;
	}
	//since our data's type is the same, in above case, is double. and we use template.
	//i think this version of magic_wang maybe better than the book's author
	
	template<typename Datatype, typename ValueFunction, typename PointerFunction>
	Datatype magic_wang(ValueFunction vf, PointerFunction pf, Datatype dparam)
	{
		Datatype v = vf(dparam);
		pf(&v);
		return v;
	}

	//type_traits101 
	using double_ptr_type = std::add_pointer<double>::type;
	using double_type = std::remove_pointer<double_ptr_type>::type;
	using same_double = std::remove_reference<double_type>::type;

	//here's function can't compile for purpose, we could add some magic blow for compile it, feel free to try
	//這段代碼直接能編譯算我輸
// 	MagicListofValue generic_magic_wang(oldFunction old_f,
// 		ListofFunctions functions, Listofparameter param)
// 	{
// 		MagicListofValue values;
// 		/*
// 		* we try to, but we can't do(for now)
// 		* for( auto f : functions)
// 		* {
// 		* values.emplace_back(f(params));
// 		* }
// 		* 
// 		*/
// 		old_f(get_pointer(values));
// 		return values;
// 	}


	template<typename F>
	struct make_tuple_of_param;
	template<typename Ret, typename... Arg>
	struct make_tuple_of_param<Ret (Arg...)>
	{
		using type = std::tuple<Arg...>;
		//using NonPtr_type = std::tuple<Arg...>;
	};
	template<typename F>
	struct make_tuple_of_param_nonptr;
	template<typename Ret, typename... Arg>
	struct make_tuple_of_param_nonptr<Ret (Arg...)>
	{
		using type = std::tuple<std::remove_pointer<Arg>...>;
		
	};
	template<typename F>
	using make_tuple_of_param_t = typename make_tuple_of_param<F>::type;
	template<typename F>
	using make_tuple_of_NonPtr_param_t = typename make_tuple_of_param_nonptr<F>::type;
//	template<typename F>
// 	void new_magic_wang(F f)
// 	{
// 		//if F is from "[](double*, double*)->void"
// 		//make_tuple_of_param generate tuple<double*, double>
// 		make_tuple_of_NonPtr_param_t<F> Param;
// 	}// this function still can't work

	//since we have ...arg we think we might write those code 
// 	template<typename... Functions, typename... Params>
// 	void magic_wang(/*somecode*/ Functions... f, Params... para)
// 	{
// 	}//we don't!
	template<typename... Function, typename... Params>
	void new_magic_wangV1(
		const std::tuple<Function...>& f,
		const std::tuple<Params...>& p1,
		const std::tuple<Params...>& p2
	)
	{
	}
	//static const std::size_t param_count = size...(Param);
	//std::make_index_sequence<param_count>()
	template<typename F, typename Params, std::size_t... I>
	auto dispatch_params_proto(F f, Params& param, std::index_sequence<I...>)
	{
		return f(std::get<I>(param)...);
	}

	template<std::size_t FunctionIndex, typename FunctionTuple, typename Params, std::size_t... I>
	auto dispatch_params(FunctionTuple& callback, Params& param, std::index_sequence<I...>)
	{
		return (std::get<FunctionIndex>(callback))(std::get<I>(param)...);
	}
	template<typename FunctionTuple,  std::size_t... I, typename Params, typename ParamSeq>
	auto dispatch_function(FunctionTuple& callback, std::index_sequence<I...>, Params& params, ParamSeq param_seq)
	{
		return std::make_tuple(dispatch_params<I>(callback, params, param_seq)...);
	}

	template<typename F, typename Tuple, std::size_t... I>
	void dispatch_to_c(F f, Tuple& t, std::index_sequence<I...>)
	{
		f(&std::get<I>(t)...);
	}


	void testNewMagicwangV1()
	{
		sence2::Reading r;
		sence2::location l;
		std::time_t time1 = std::time(nullptr);
		std::time_t time2 = std::time(nullptr);
		new_magic_wangV1(
			std::make_tuple([&r](sence2::location loca, std::time_t t) ->double {return r.alpha_value(loca, t); },
				[&r](sence2::location loca, std::time_t t) ->double {return r.beta_value(loca, t); }),
			std::make_tuple(l, time1),
			std::make_tuple(l, time2)
		);
	}
	template<typename LegacyFunction, typename... Function, typename... Params>
	auto new_magic_wangV2(LegacyFunction legacy,
		const std::tuple<Function...>& functions,
		const std::tuple<Params...>& p1,
		const std::tuple<Params...>& p2)
	{
		static const std::size_t function_count = sizeof...(Function);
		static const std::size_t Param_count = sizeof...(Params);
		using Tuple_type = make_tuple_of_NonPtr_param_t<LegacyFunction>;
		Tuple_type params =
			//we would like todo
// 			for (auto f : functions) f(p1);
// 			for (auto f : functions) f(p2);
			std::tuple_cat(
				dispatch_function(functions, std::make_index_sequence<function_count>(), p1, std::make_index_sequence<Param_count>()),
				dispatch_function(functions, std::make_index_sequence<function_count>(), p2, std::make_index_sequence<Param_count>())
			);
		static const std::size_t count = std::tuple_size<Tuple_type>::value;
		dispatch_to_c(legacy, params, std::make_index_sequence<count>());
		return params;
	}


	void testNewMagicwangV2()
	{
		sence2::Reading r;
		sence2::location l;
		std::time_t time1 = std::time(nullptr);
		std::time_t time2 = std::time(nullptr);
		using LegacyFunction = double(double*, double*, double*, double*);
		new_magic_wangV2(sence2::adjust_value,
			std::make_tuple(
				[&r](sence2::location loca, std::time_t t) ->double {return r.alpha_value(loca, t); },
				[&r](sence2::location loca, std::time_t t) -> double{return r.beta_value(loca, t); }),
			std::make_tuple(l, time1),
			std::make_tuple(l, time2)
		);
	}



}

int main()
{
	//sence1::sence_1();
	//sence1::sence_2();
	//sence2::test_adapter();
	
	// speaking frankly, i don't know this function's usability.
// 		double d = 0.0f;
// 		sence3::magic_wang([](double data) -> double { return data; },
// 			[](double* p) -> double { return *p; }, d
// 		);
	//sence3::testNewMagicwangV1();
	sence3::testNewMagicwangV2();
}


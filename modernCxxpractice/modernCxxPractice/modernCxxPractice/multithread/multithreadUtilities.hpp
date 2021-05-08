#ifndef __MULTITHREADUTILITIES_H__
#define __MULTITHREADUTILITIES_H__
#include<thread>
#include "parallel_accumulate.h"
namespace threadUtilities
{
	class thread_guard
	{
	public:
		explicit thread_guard(std::thread& t_):t(t_)
		{}
		~thread_guard()
		{
			if (t.joinable())
			{
				t.join();
			}
		}
		thread_guard(thread_guard const&) = delete;
		thread_guard& operator=(thread_guard const&) = delete;
	private:
		std::thread& t;
	};

	class scoped_thread
	{
	public:
		explicit scoped_thread(std::thread t_) :
			t(std::move(t_))
		{
			if (!t.joinable())
				throw std::logic_error("No thread");
		}
		~scoped_thread()
		{
			 t.join();
		}
	public:
		scoped_thread(scoped_thread const&) = delete;
		scoped_thread& operator=(scoped_thread const&) = delete;
	private:
		std::thread t;
	};

	template <typename T>
	class Data_wrapper
	{
	public:
		explicit Data_wrapper(const T& unprotected) : data{ unprotected } {}
		Data_wrapper(const Data_wrapper&& otherData) :data{ std::move(otherData.data) } {}
		~Data_wrapper() 
		{
			lock.unlock();
		}
	public:
		Data_wrapper(const Data_wrapper&) = delete;
		Data_wrapper operator=(const Data_wrapper&) = delete;
		Data_wrapper operator = (const Data_wrapper&&) = delete;
	public:
		//do something
		 template <typename Callback>
		 auto process(Callback cb)
		 {
			 std::lock_guard guard(lock);
			 return cb(std::ref(data));
		 }
	private:
		T& data;
		mutable std::mutex lock;
	};

}// end of namespace threadUtilities



#endif // end of   __MULTITHREADUTILITIES_H__

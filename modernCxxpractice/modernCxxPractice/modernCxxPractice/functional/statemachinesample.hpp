///this file is a sample statemachine implament from <functional programming in C++>
//count the file line.
//author: vihoc

#ifndef __STATE_MACHINESAMPLE_H__
#define __STATE_MACHINESAMPLE_H__

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <variant>

#include <cassert>

namespace statemachine
{
	class state_t
	{
	protected: 
		state_t(int type) :type(type){}

	public:
		virtual ~state_t() {}
		int type;
	};

	class init_t:public state_t
	{
	public:
		enum { id = 0 };
		init_t() :state_t(id) {}
	};

	class running_t :public state_t
	{
	public:
		enum { id = 1 };
		running_t(const std::string& file_name) :state_t(id),  m_file(file_name) {}

		inline unsigned getcount() const { return count; }
		void count_words()
		{
			count = std::distance(
				std::istream_iterator<std::string>(m_file),
				std::istream_iterator<std::string>());
		}
	private:
		unsigned count;
		std::ifstream m_file;
		//someother 
	};

	class finished_t :public state_t
	{
	public:
		enum { id = 2 };
		finished_t(unsigned count = 0): state_t(id),count(count) {}
		inline unsigned getcount() const { return count; }
	private:
		unsigned count;
	};

	template <typename... Fs>
	struct overloaded : Fs... { using Fs::operator()...; };

	template <typename... Fs> overloaded(Fs...)->overloaded<Fs...>;

	class program_t
	{
	public:
		program_t()
			: state(init_t())
		{
		}

		void count_words(const std::string& file_name)
		{
			assert(state.index() == 0);

			state = running_t(file_name);

			std::get<running_t>(state).count_words();

			counting_finished();
		}


		void counting_finished()
		{
			
			const auto* pstate = std::get_if<running_t>(&state);

			assert(nullptr != pstate);

			state = finished_t(pstate->getcount());
		}

		unsigned count() const
		{
	
			return std::visit(overloaded{
						[](init_t) {
							return (unsigned)0;
						},
						[](const running_t& state) {
							return state.getcount();
						},
						[](const finished_t& state) {
							return state.getcount();
						}
				}, state);
		}
	private:
		std::variant < init_t
			, running_t
			, finished_t
		> state;

	};

	auto main()->void
	{
		program_t program;
		program.count_words("range101.cpp");
	}
}


#endif // !__STATE_MACHINESAMPLE_H__

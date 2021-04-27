/**
*使用循環鏈表實現的隊列,接口同stl
*author: vihoc
*/

#ifndef _VIHO_STRUCTURES_QUEUE_H
#define _VIHO_STRUCTURES_QUEUE_H

#include <cstdint>

#include <doubly_circular_list.h>
#include <traits.h>

namespace VihoStructures {

template <typename T, typename Container>
class QueueWrapper {
public:
	void push(const T& data) { return cont.push_back(data); }
	T pop() { return cont.pop_front(); }
	T& front() { return cont.front(); }
	const T& front() const { return cont.front(); }
	T& back() { return cont.back(); }
	const T& back() const { return cont.back(); }
	void clear() { return cont.clear(); }
	std::size_t size() const { return cont.size(); }

private:
	Container cont;
};

template <typename T>
class Queue : public QueueWrapper<T, DoublyCircularList<T>> {};

}  // namespace VihoStructures
}  // namespace VihoStructures

/* name trait */
template <>
const std::string traits::type<VihoStructures::Queue>::name = "Queue";

#endif

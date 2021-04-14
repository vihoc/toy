/**
基于C++1單鏈表 , 支持std方式编程.
author: vihoc
*/
#ifndef _VIHO_STRUCTURES_LINKED_LIST_H
#define _VIHO_STRUCTURES_LINKED_LIST_H

#include <cstdint>
#include <stdexcept>

#include <traits.h>

namespace VihoStructures {

/**
 * @單鏈表數據結構實現
 * @每個元素存儲一個指針,鏈表尾部next指向nullptr
 * 
 *
 * @模板參數T為保存的數據中類型
 */
template <typename T, class A = std::allocator<T>>
class LinkedList {
	struct Node;
public:
	typedef std::allocator_traits<A> allocator_type;
	typedef typename allocator_type::value_type value_type;
	//typedef typename std::allocator_traits<A>::reference reference;
//	typedef typename std::allocator_traits<A>::const_reference const_reference;
	typedef typename allocator_type::difference_type difference_type;
	typedef typename allocator_type::size_type size_type;

public:
	using iterator = bidirectional_iterator<Node>;
	using const_iterator = const_bidirectional_iterator<const Node>;
	using reverse_iterator = std::reverse_iterator<const iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
	LinkedList() = default;
/**
 * @构造函数
 * @允许使用初始化列表构造链表
 */

	LinkedList(std::initializer_list <T>&& other)
	{
		for (auto& e : other)
		{
			push_back(e);
		}
		
	}

	LinkedList(const LinkedList<T>& other)
		: head{copy_list(other.head)}, size_{other.size_} {}

	LinkedList(LinkedList<T>&& other) : head{other.head}, size_{other.size_} {
		other.head = nullptr;
		other.size_ = 0;
	}

	LinkedList<T>& operator=(const LinkedList<T>& other) {
		LinkedList<T> copy{other};
		std::swap(head, copy.head);
		std::swap(size_, copy.size_);
		return *this;
	}

	LinkedList<T>& operator=(LinkedList<T>&& other) {
		LinkedList<T> copy{std::move(other)};
		std::swap(head, copy.head);
		std::swap(size_, copy.size_);
		return *this;
	}

	virtual ~LinkedList() { clear(); }

	/**
	 * @清空列表
	 */
	void clear() {
		while (!empty())
			pop_front();
	}

	/**
	 * @向鏈表尾部添加對象
	 *
	 * @參數:'data' 會被添加到鏈表尾部
	 */
	void push_back(const T& data) { insert(data, size_); }

	/**
	 * @向鏈表添加元素
	 *
	 * @參數'data'為要加入鏈表的數據
	 */
	void push_front(const T& data) {
		head = new Node(data, head, this);
		++size_;
	}

	/**
	 * @向指定位置插入數據
	 *
	 * @參數:'data' 為要插入鏈表的對象
	 * @參數:'index'為data要插入的位置
	 */
	void insert(const T& data, std::size_t index) {
		if (index == 0) {
			return push_front(data);
		} else if (index > size_) {
			throw std::out_of_range("Invalid index");
		} else {
			Node* it = head;
			for (std::size_t i = 0; i < index - 1; ++i) {
				it = it->next;
			}
			it->next = new Node(data, it->next, this);

			++size_;
		}
	}

	/**
	 * @向已排序的數據的數組插入數據
	 *
	 * @參數'data'會被按數據已有順序插入
	 */
	void insert_sorted(const T& data) {
		if (empty() || data <= head->data) {
			return push_front(data);
		} else {
			Node* it = head;
			while (it->next != nullptr && data > it->next->data) {
				it = it->next;
			}
			it->next = new Node(data, it->next, this);

			++size_;
		}
	}

	/**
	 * @提供下表訪問鏈表
	 * @參數:'index' 為被訪問的下表
	 *
	 * @返回index位置存儲的值.
	 */
	T& at(std::size_t index) {
		return const_cast<T&>(static_cast<const LinkedList*>(this)->at(index));
	}

	const T& at(std::size_t index) const {
		if (index >= size_) {
			throw std::out_of_range("Index out of bounds");
		} else {
			Node* it = head;
			for (std::size_t i = 0; i < index; i++) {
				it = it->next;
			}
			return it->data;
		}
	}

	/**
	 * @從鏈表中移除元素
	 *
	 * @參數:'index'為被刪除的元素
	 *
	 * @返回值:被刪除的元素
	 */
	T erase(std::size_t index) {
		if (index >= size_) {
			throw std::out_of_range("Index out of bounds");
		} else if (index == 0) {
			return pop_front();
		} else {
			Node* it = head;
			for (std::size_t i = 0; i < index - 1; ++i) {
				it = it->next;
			}

			T removed = it->next->data;
			Node* p_removed = it->next;
			it->next = it->next->next;

			--size_;
			delete p_removed;
			return removed;
		}
	}

	/***
	*
	* iterator版earse
	*/
	iterator earse(iterator iter)
	{
		difference_type diff = distance(begin(), iter);
		if (diff >= size_)
		{
			throw std::out_of_range("Index out of bounds(iterator erase)");
		}
		erase(diff);
		auto newIter = begin();
		advance(newIter, diff);
		return newIter;

	}
	/**
	 * @彈出列表尾部節點(被刪除)
	 *
	 * @返回值: 返回被刪除的元素
	 */
	T pop_back() { return erase(size_ - 1); }

	/**
	 * @刪除列表頭部節點
	 *返回被刪除的節點值
	 */
	T pop_front() {
		if (empty()) {
			throw std::out_of_range("List is empty");
		} else {
			T removed = head->data;
			Node* old_head = head;
			head = head->next;
			delete old_head;
			--size_;
			return removed;
		}
	}

	/**
	 * @刪除鏈表中元素,
	 *
	 * @參數:'data'返回被刪除的值
	 */
	void remove(const T& data) {
		if (head->data == data) {
			pop_front();
			return;
		} else {
			Node* it;
			for (it = head; it->next->data != data; it = it->next) {
				if (it->next == nullptr)
					return;
			}

			Node* p_removed = it->next;
			it->next = it->next->next;
			delete p_removed;

			--size_;
		}
	}

	/**
	 * @檢查鏈表是否為空
	 *
	 * @爲空返回true
	 */
	bool empty() const { return size_ == 0; }

	/**
	 * @查找是否有某元素
	 *
	 * @參數:'data'是需要查找的值
	 *
	 * @如果找到返回true
	 */
	bool contains(const T& data) const { return find(data) != size_; }

	/**
	 * @查找元素,返回其在鏈表中的位置
	 *
	 * @參數:'data'被查找的元素
	 *
	 * @返回其在鏈表中的位置
	 */
	std::size_t find(const T& data) const {
		std::size_t index = 0;
		for (Node* it = head; it != nullptr; it = it->next) {
			if (it->data == data)
				break;
			++index;
		}
		return index;
	}

	/**
	 * @獲取鏈表長度
	 *
	 * @返回值:長度
	 */
	std::size_t size() const { return size_; }

	T& front() { return head->data; }

	const T& front() const { return head->data; }

	T& back() {
		Node* it = head;
		for (std::size_t i = 1; i < size_; ++i) {
			it = it->next;
		}
		return it->data;
	}

	const T& back() const {
		Node* it = head;
		for (std::size_t i = 1; i < size_; ++i) {
			it = it->next;
		}
		return it->data;
	}


	public:
		/**
		 * @stl风格容器支持,与stl相同.故不写注释
		 *
		 */
		iterator begin() noexcept { return iterator(head); }
		iterator end() noexcept { return iterator(nullptr); }


		const_iterator begin() const { return iterator(head); }
		const_iterator end() const { return iterator(nullptr); }

		const_iterator cbegin() const { return const_iterator(head); }
		const_iterator cend() const { return const_iterator(nullptr); }

		reverse_iterator rbegin() { iterator iter = begin();  std::advance(iter, size_); return reverse_iterator(iter); }
		reverse_iterator rend() { return reverse_iterator(iterator(nullptr)); }

		reverse_iterator rcbegin() const { iterator iter = begin();  std::advance(iter, size_); return const_reverse_iterator(iter); }
		reverse_iterator rcend() const { return const_reverse_iterator(const_iterator(nullptr)); }

		const_reverse_iterator crbegin() const{ iterator iter = begin();  std::advance(iter, size_); return const_reverse_iterator(iter); }
		const_reverse_iterator crend() const { return const_reverse_iterator(const_iterator(nullptr)); }

private:
	using List =  LinkedList<T>;
	struct Node {
		explicit Node(const T& data) : data{ data }, list{nullptr} {}
		Node(const T& data, const List& listref) : data{ data }, list{ listref } {}
		Node(const T& data, Node* next, const List* listref) : data{ data }, next{ next }, list{listref}{}
		/*
		*臨時解決方式,存放鏈表指針獲取begin
		* stl為保存了prev_
		*/
		const List* list;
		T data;
		Node* next{nullptr};
	public:
		Node* operator--() { difference_type diff = std::distance(list->begin(), iterator(this)); /*檢查diff*/iterator iter = std::advance(list->begin(), diff - 1); return iter; }
		Node* operator--(int junk) { difference_type diff = std::distance(list->begin(), iterator(this)); /*檢查diff*/iterator iter = std::advance(list->begin(), diff - 1); return iter; }

		Node* operator--() const { difference_type diff = std::distance(list->begin(), const_iterator(&this)); /*檢查diff*/const_iterator iter = std::advance(list->begin(), diff - 1); return iter; }
		Node* operator--(int junk) const { difference_type diff = std::distance(list->begin(), const_iterator(&this)); /*檢查diff*/const_iterator iter = std::advance(list->begin(), diff - 1); return iter; }

		Node* operator++() { return this->next; }
		Node* operator++(int junk) { return this->next; }
		//Node* operator=(const Node* other) { std::swap(this, other); };
		Node* operator++() const { return this->next; }
		Node* operator++(int junk) const { return this->next; }
		const T& operator*() const { return this->data; }
		Node* operator->() const { return this; }

	public:

		typedef T value_type;
		typedef value_type& reference;

	};

	static Node* copy_list(const Node* other_head) {
		auto new_tail = new Node(other_head->data);
		auto new_head = new_tail;
		auto it = other_head->next;

		while (it != nullptr) {
			new_tail->next = new Node(it->data);
			new_tail = new_tail->next;
			it = it->next;
		}

		return new_head;
	}

	Node* head{nullptr};
	std::size_t size_{0u};
};

}  // namespace structures

/* list trait */
template <>
const bool traits::is_list<VihoStructures::LinkedList>::value = true;

/* name trait */
template <>
const std::string traits::type<VihoStructures::LinkedList>::name = "LinkedList";

#endif

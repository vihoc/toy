/**
 * @帶哨兵的双向链表实现 支持C++ stl風格編程
 * @模板参数T为存储对象的类型
 * @哨兵節點除了在少數函數内會變動外,請不要隨便修改
 * 
 * @author:vihoc 
 */


#ifndef _VIHO_STRUCTURES_DOUBLY_CIRCULAR_LIST_H
#define _VIHO_STRUCTURES_DOUBLY_CIRCULAR_LIST_H

#include <stdexcept>
#include <initializer_list>
#include <traits.h>

namespace VihoStructures {


template <typename T, class A = std::allocator<T>>
class DoublyCircularList {
	struct Node;
public:
	typedef std::allocator_traits<A> allocator_type;
	typedef typename allocator_type::value_type value_type;
	//typedef typename std::allocator_traits<A>::reference reference;
//	typedef typename std::allocator_traits<A>::const_reference const_reference;
	typedef typename allocator_type::difference_type difference_type;
	typedef typename allocator_type::size_type size_type;

public:
	using iterator = forward_iterator<Node>;
	using const_iterator = const_forward_iterator<const Node>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
	DoublyCircularList() = default;

/**
 * @构造函数
 * @允许使用初始化列表构造链表
 */
	
	DoublyCircularList(std::initializer_list <T>&& other)
	{
		sentinal = new Node(0);
		sentinal->prev = sentinal;
		sentinal->next = sentinal;
		for (auto& e : other)
		{
			push_back(e);
		}
	}

	DoublyCircularList(const DoublyCircularList<T>& other)
		: sentinal{copy_list(other.sentinal)}, size_{other.size_} {}

	DoublyCircularList(DoublyCircularList<T>&& other)
		: sentinal{other.sentinal }, size_{other.size_} {
		other.head = nullptr;
		other.size_ = 0;
	}

	DoublyCircularList<T>& operator=(const DoublyCircularList<T>& other) {
		DoublyCircularList<T> copy{other};
		std::swap(sentinal, copy.sentinal);
		std::swap(size_, copy.size_);
		return *this;
	}

	DoublyCircularList<T>& operator=(DoublyCircularList<T>&& other) {
		DoublyCircularList<T> copy{std::move(other)};
		std::swap(sentinal, copy.sentinal);
		std::swap(size_, copy.size_);
		return *this;
	}

	~DoublyCircularList() { clear(); delete sentinal; sentinal = nullptr; }

	/**
	 * @清空链表
	 */
	void clear() {
		while (!empty()) {
			pop_back();
		}
	}

	/**
	 * @向链表尾部插入元素
	 *
	 * @参数'data'将会被插入链表尾部
	 */

	void push_back(const T& data) 
	{
		
		auto newNode = new Node(data, sentinal->prev, sentinal);
		newNode->prev->next = newNode;
		sentinal->prev = newNode;
	
		++size_;
	}

	/**
	 * @向链表头部添加元素
	 *
	 * @参数'data',会被添加到链表头部
	 */
	void push_front(const T& data)
	{

		auto newNode = new Node(data, sentinal, sentinal->next);
		
		sentinal->next = newNode;

		++size_;


	}

	/**
	 * @向指定的位置插入元素
	 *
	 * @参数:'data' 将会被插入到index所示的位置
	 * @参数:'index' 第index位将会被插入data
	 */
	void insert(const T& data, std::size_t index)
	{
		if (index == 0) {
			push_front(data);
		} else if (index > size_) {
			throw std::out_of_range("Invalid index (insert())");
		} else {
			auto it = sentinal->next;
			for (std::size_t i = 0; i < index - 1; ++i) {
				it = it->next;
			}
			it->next = new Node(data, it, it->next);
			it->next->next->prev = it->next;
			++size_;
		}
	}

	/**
	 * @向已排序完的链表插入data
	 *
	 * @参数'data'会按链表的顺序被插入到相应位置
	 */
	void insert_sorted(const T& data)
	{
		if (empty() || data <= sentinal->data)
			return push_front(data);
		auto it = sentinal->next;
		while (it->next != sentinal && data > it->next->data) {
			it = it->next;
		}
		auto newNode = new Node(data, it, it->next);
		it->next->prev = newNode;
		it->next = newNode;
		++size_;
	}

	/**
	 * @删除链表第n个元素
	 *
	 * @参数:'index'位置的元素将被删除
	 *
	 * @返回值:返回被删除的元素值
	 */
	T erase(std::size_t index) {
		if (index >= size_)
			throw std::out_of_range("Index out of bounds (erase by pos())");
		auto to_delete = sentinal;
		for (std::size_t i = 0; i < index + 1; ++i) {
			to_delete = to_delete->next;
		}
		auto out = delete_node(to_delete);
		
		return out;
	}

	/**
	 * @支持stl的erase
	 *
	 * 與stl標準容器中erase表現一樣
	 */
	iterator erase(iterator pos)
	{
		difference_type diff = std::distance(begin(), pos);
		if(size_ < static_cast<size_t>(diff))	throw std::out_of_range("Index out of bounds (erase by iterator pos())");
		erase(diff);
		iterator iter = begin();
		std::advance(iter, diff);
		return  iter;


	}

	/**
	 * @删除链表尾部元素
	 *元素会被弹出
	 * @返回值:被删除的元素值
	 */
	T pop_back() {
		if (empty())
			throw std::out_of_range("List is empty (pop_back())");
	
		return delete_node(sentinal->prev);
	}

	/**
	 * @删除元素头部节点
	 *元素会被弹出
	 * @返回值:返回被删除的元素内容
	 */
	T pop_front() {
		if (empty())
			throw std::out_of_range("List is empty (pop_front())");
		
		return delete_node(sentinal->next);
	}

	/**
	 * @删除链表内元素.如果某个元素存在于链表里,则删除
	 *
	 * @参数:'data',如果有data,则被删除
	 */
	void remove(const T& data)
	{
		auto it = sentinal->next;
		for (; it->data != data; it = it->next) {
			if (it == sentinal)
				return;  // Reached end of the list
		}

		delete_node(it);
	}

	/**
	 * @检查链表是否为空
	 *
	 * @如果空则返回true
	 */
	bool empty() const { return size_ == 1; }

	/**
	 * @检查链表内是否有某元素.
	 *
	 * @参数:'data'会被查找.
	 *
	 * @返回值: 如果有某值则返回true
	 */
	bool contains(const T& data) const
	{
		if (empty())
			return false;

// 		if (sentinal->data == data)
// 			return true;

		for (auto it = sentinal->next; it != sentinal; it = it->next) {
			if (it->data == data)
				return true;
		}
		return false;
	}

	/**
	 * @允许下标访问链表的方法.(兼容stl,.at &&[])
	 * 
	 * @如果下标越界,抛出如下异常
	 * std::out_of_range exception
	 *
	 * @参数:'index' "数组"的下标,从head节点开始
	 * 
	 * @返回值: 返回该下标对象的引用
	 * 链表存储对象可能被更改! 如不需要更改,请使用const DoublyCircularList<T>
	 */
	T& at(std::size_t index) 
	{
		return const_cast<T&>(
			static_cast<const DoublyCircularList*>(this)->at(index));
	}

	const T& at(std::size_t index) const 
	{
		if (index >= size_)
			throw std::out_of_range("Index out of bounds");
		auto it = sentinal;
		for (std::size_t i = 0; i < index; ++i) {
			it = it->next;
		}
		return it->data;
	}

	/**
	 * @查找链表里是否有某元素
	 *
	 * @参数: 'data'会被链表查找
	 * 
	 * @返回值:返回元素位置.
	 */
	std::size_t find(const T& data) const
	{
		if (size_ == 0)
			return 0;

		if (sentinal->data == data)
			return 0;

		std::size_t index = 1;
		for (auto it = sentinal->next; it != sentinal; it = it->next) {
			if (it->data == data)
				break;
			++index;
		}
		return index;
	}

	/**
	 * @返回元素大小
	 */
	std::size_t size() const { return size_; }

	T& front() { return sentinal->data; }

	const T& front() const { return sentinal->data; }

	T& back() { return sentinal->prev->data; }

	const T& back() const { return sentinal->prev->data; }

public:
/**
 * @stl风格容器支持,与stl相同.故不写注释
 *
 */
	iterator begin() noexcept { return iterator(sentinal->next); }
	iterator end() noexcept  { return iterator(sentinal); }


	const_iterator begin() const { return iterator(sentinal->next); }
	const_iterator end() const { return iterator(sentinal); }

	const_iterator cbegin() { return const_iterator(sentinal->next); }
	const_iterator cend() { return const_iterator(sentinal); }

	reverse_iterator rbegin() { return reverse_iterator(sentinal->prev); }
	reverse_iterator rend() { return reverse_iterator(sentinal); }

	reverse_iterator rcbegin() const { return reverse_iterator(sentinal->prev); }
	reverse_iterator rcend() const { return reverse_iterator(sentinal); }

	const_reverse_iterator crbegin() { return const_reverse_iterator(sentinal->prev); }
	const_reverse_iterator crend() { return const_reverse_iterator(sentinal); }


private:
	struct Node {
		explicit Node(const T& data) : data{data} {}
		Node(const T& data, Node* next) : data{data}, next{next} {}
		Node(const T& data, Node* prev, Node* next)
			: data{data}, prev{prev}, next{next} {}
		T data;
		Node* prev{nullptr};
		Node* next{nullptr};
		Node* operator--() { return this->prev; }
		Node* operator--(int junk) { return this->prev; }

		Node* operator--() const{ return this->prev; }
		Node* operator--(int junk) const{ return this->prev; }

		Node* operator++() { return this->next; }
		Node* operator++(int junk) { return this->next; }

		Node* operator++() const{ return this->next; }
		Node* operator++(int junk) const{ return this->next; }
		const T& operator*() const { return this->data; }
		Node* operator->() const { return this; }
		//Node* operator=(Node* other) { return this->prev; }

	public: 
		
		typedef T value_type;
		typedef value_type& reference;
	};

	T delete_node(Node* To_delete)
	{
		if (empty())
			throw std::out_of_range("List is empty (Internal delete)");
		//	sentinal 不允許被刪除
		if (To_delete == sentinal)
			throw std::out_of_range("Index Out of Range (Internal delete)");
		To_delete->next->prev = To_delete->prev;
		To_delete->prev->next = To_delete->next;

		T out = To_delete->data;
		delete To_delete;
		--size_;
		return out;
	}

	static Node* copy_list(const Node* other_head) 
	{
		DoublyCircularList<T> copy;
		copy.push_back(other_head->data);

		for (auto it = other_head->next; it != other_head; it = it->next) {
			copy.push_back(it->data);
		}

		auto p = copy.head;
		copy.sentinal = nullptr;
		copy.size_ = 0;

		return p;
	}

	Node* sentinal{nullptr};
	std::size_t size_{0u};
};

}  // namespace VihoStructures

/* list trait */
template <>
const bool traits::is_list<VihoStructures::DoublyCircularList>::value = true;

/* name trait */
template <>
const std::string traits::type<VihoStructures::DoublyCircularList>::name =
	"DoublyCircularList";

#endif

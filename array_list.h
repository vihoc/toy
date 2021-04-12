/**
基于C++11动态数组 , 支持std方式编程.
author: vihoc
*/

#ifndef STRUCTURES_ARRAY_LIST_H
#define STRUCTURES_ARRAY_LIST_H

#include <cstdint>
#include <memory>
#include <stdexcept>

#include <traits.h>
#include <utils.h>

namespace VihoStructures {

/**
 * @使用数组实现列表容器 list(data structure).也就是动态数组 
 *
 * @模板参数为需要存储的数据类
 */
template <class T, class A = std::allocator<T>>
class ArrayList {

public:
	typedef A allocator_type;
	typedef typename A::value_type value_type;
	typedef typename A::reference reference;
	typedef typename A::const_reference const_reference;
	typedef typename A::difference_type difference_type;
	typedef typename A::size_type size_type;

	class iterator {
	public:
		typedef typename A::difference_type difference_type;
		typedef typename A::value_type value_type;
		typedef typename A::reference reference;
		typedef typename A::pointer pointer;
		typedef std::random_access_iterator_tag iterator_category; 
		iterator();
		iterator(const iterator&);
		~iterator();

		iterator& operator=(const iterator&);
		bool operator==(const iterator&) const;
		bool operator!=(const iterator&) const;
		bool operator<(const iterator&) const; 
		bool operator>(const iterator&) const; 
		bool operator<=(const iterator&) const; 
		bool operator>=(const iterator&) const; 

		iterator& operator++();
		iterator operator++(int); 
		iterator& operator--(); 
		iterator operator--(int); 
		iterator& operator+=(size_type); 
		iterator operator+(size_type) const; 
		friend iterator operator+(size_type, const iterator&); 
		iterator& operator-=(size_type);            
		iterator operator-(size_type) const; 
		difference_type operator-(iterator) const; 

		reference operator*() const;
		pointer operator->() const;
		reference operator[](size_type) const; 
	};
	class const_iterator {
	public:
		typedef typename A::difference_type difference_type;
		typedef typename A::value_type value_type;
		typedef typename const A::reference reference;
		typedef typename const A::pointer pointer;
		typedef std::random_access_iterator_tag iterator_category; 

		const_iterator();
		const_iterator(const const_iterator&);
		const_iterator(const iterator&);
		~const_iterator();

		const_iterator& operator=(const const_iterator&);
		bool operator==(const const_iterator&) const;
		bool operator!=(const const_iterator&) const;
		bool operator<(const const_iterator&) const; 
		bool operator>(const const_iterator&) const; 
		bool operator<=(const const_iterator&) const; 
		bool operator>=(const const_iterator&) const; 

		const_iterator& operator++();
		const_iterator operator++(int); 
		const_iterator& operator--(); 
		const_iterator operator--(int); 
		const_iterator& operator+=(size_type); 
		const_iterator operator+(size_type) const; 
		friend const_iterator operator+(size_type, const const_iterator&); 
		const_iterator& operator-=(size_type);          
		const_iterator operator-(size_type) const; 
		difference_type operator-(const_iterator) const; 

		reference operator*() const;
		pointer operator->() const;
		reference operator[](size_type) const;
	};

	typedef std::reverse_iterator<iterator> reverse_iterator; 
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator; 


public:
	ArrayList() = default;

	ArrayList(const ArrayList<T>& other)
		: contents{copy_array(other.contents, other.size_, other.max_size_)}
		, size_{other.size_}
		, max_size_{other.max_size_} {}

	ArrayList(ArrayList<T>&& other)
		: contents{std::move(other.contents)}
		, size_{std::move(other.size_)}
		, max_size_{std::move(other.max_size_)} {
		other.size_ = 0;
		other.max_size_ = 0;
	}

	ArrayList<T>& operator=(const ArrayList<T>& other) {
		ArrayList<T> copy{other};
		std::swap(contents, copy.contents);
		std::swap(size_, copy.size_);
		std::swap(max_size_, copy.max_size_);
		return *this;
	}

	ArrayList<T>& operator=(ArrayList<T>&& other) {
		ArrayList<T> copy{std::move(other)};
		std::swap(contents, copy.contents);
		std::swap(size_, copy.size_);
		std::swap(max_size_, copy.max_size_);
		return *this;
	}

	virtual ~ArrayList() = default;

	/**
	 * @构造函数, 创建包含max_size个元素的列表
	 *
	 * @参数:max_size, 数组大小.
	 */
	explicit ArrayList(std::size_t max_size)
		: contents{new T[max_size]}, max_size_{max_size} {}

	/**
	 * @清除数组里元素
	 */
	void clear() { size_ = 0; }

	/**
	 * @添加'data'到数组尾部
	 *
	 * @参数:data,会存储到数组内.
	 */
	void push_back(const T& data) { insert(data, size_); }


	//

		//emplace
		//emplace_back
	//begin
	//end iterator
	//

	/**
	 * @ 添加'data' 到数组开头
	 *
	 * @参数:data 会被存储到数组内
	 */
	void push_front(const T& data) { insert(data, 0); }

	/**
	 * @为数组某个位置添加元素'data'
	 *
	 * @参数:data 会被插入到数组内.
	 * @参数:index, 插入data的位置 
	 */
	void insert(const T& data, std::size_t index) {
		if (index > size_) {
			throw std::out_of_range("Index out of bounds");
		} else {
			for (std::size_t i = size_; i > index; i--) {
				contents[i] = contents[i - 1];
			}
			contents[index] = data;
			size_++;

			if (max_size_ == size_)
				expand(2);
		}
	}

	/**
	 * @向有序数组插入元素'data' todo:能指定排序的顺序
	 *
	 * @参数'data' 会被插入到满足数组元素本来顺序相应的位置.
	 */
	void insert_sorted(const T& data) {
		std::size_t i = 0;
		while (i < size_ && data >= contents[i])
			i++;
		insert(data, i);
	}

	/**
	 * @在给定位置删除元素
	 *
	 * @参数: index, 想要删除元素的元素的位置.
	 *
	 * @返回值: 被删除的值
	 * TODO:std::move
	 */
	T erase(std::size_t index) {
		if (empty()) {
			throw std::out_of_range("List is empty");
		} else if (index >= size_) {
			throw std::out_of_range("Index out of bounds");
		} else {
			T deleted = contents[index];
			for (std::size_t i = index; i < size_ - 1; ++i) {
				contents[i] = contents[i + 1];
			}
			size_--;

			if (max_size_ / 4 > size_)
				expand(0.5);

			return deleted;
		}
	}

	/**
	 * @删除数组尾部的元素
	 *
	 * @返回值:被删除的元素
	 */
	T pop_back() { return erase(size_ - 1); }

	/**
	 * @b删除数组首部元素
	 *
	 * @返回值:被删除的元素
	 */
	T pop_front() { return erase(0); }

	/**
	 * @删除数组内为'data'的元素 
	 * 如果data不存在于容器内,则不会发生任何事
	 *
	 * @参数: 'data'会被查找,如果找到会被删除 
	 * 
	 */
	void remove(const T& data) { 
		std::size_t pos = find(data);
		if(0 != size_ && pos < size_) erase(pos); 
	}

	/**
	 * @检查是否是空数组
	 *
	 * @返回值: 如果是空的返回true
	 */
	bool empty() const { return size_ == 0; }

	/**
	 * @检查容器内部是否有等于'data'的元素
	 *
	 * @参数: data 用于查找是否存在于容器中
	 *
	 * @返回值: 如果有则返回true
	 */
	bool contains(const T& data) const { return find(data) != size_; }

	/**
	 * @返回容器中'data'的位置.
	 *
	 * @参数:'data'会被查找
	 *
	 * @返回值:如果存在'data',返回位置,不存在,返回数组长度.
	 */
	std::size_t find(const T& data) const {
		for (std::size_t i = 0; i < size_; ++i) {
			if (contents[i] == data)
				return i;
		}
		return size_;
	}

	/**
	 * @获取数组的长度
	 *
	 * @返回值, 数组长度.
	 */
	std::size_t size() const { return size_; }

	/**
	 * @检查数组下标是否有元素,有返回元素
	 * 
	 *
	 * @参数:index, 数组下标.如果数组中此值位置有值,则返回值
	 *
	 * @返回值:如果下标有值,返回此位置值的引用
	 */
	T& at(std::size_t index) {
		return const_cast<T&>(static_cast<const ArrayList*>(this)->at(index));
	}

	const T& at(std::size_t index) const {
		if (index >= size_) {
			throw std::out_of_range("Index out of bounds");
		} else {
			return contents[index];
		}
	}

	/**
	 * @数组下标[]重载
	 * @与.at相同
	 *
	 * @参数:index, 数组下标.如果数组中此值位置有值,则返回值
	 *
	 * @返回值:如果下标有值,返回此位置值的引用
	 */
	T& operator[](std::size_t index) {
		return const_cast<T&>(
			static_cast<const ArrayList*>(this)->operator[](index));
	}

	const T& operator[](std::size_t index) const { return contents[index]; }

	T& front() { return contents[0]; }

	const T& front() const { return contents[0]; }

	T& back() { return contents[size_ - 1]; }

	const T& back() const { return contents[size_ - 1]; }

private:
	void expand(float ratio) {
		contents = copy_array(contents, size_, max_size_ * ratio);
		max_size_ *= ratio;
	}

	static std::unique_ptr<T[]> copy_array(
		const std::unique_ptr<T[]>& original, std::size_t size,
		std::size_t new_size) {
		std::unique_ptr<T[]> copy{new T[new_size]};
		for (std::size_t i = 0; i < size; i++) {
			copy[i] = original[i];
		}
		return copy;
	}

	const static std::size_t starting_size{8};

	std::unique_ptr<T[]> contents = make_unique<T[]>(starting_size);
	std::size_t size_{0u};
	std::size_t max_size_{starting_size};
};

}  // namespace structures

/* list trait */
template <>
const bool traits::is_list<structures::ArrayList>::value = true;

/* name trait */
template <>
const std::string traits::type<structures::ArrayList>::name = "ArrayList";

#endif

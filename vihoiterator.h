#ifndef __VIHO_STRUCTURES_ITERATOR_H
#define __VIHO_STRUCTURES_ITERATOR_H
#include <iterator>
#include <traits.h>

/**
 * @iterator的基本实现
 *
 * @让容器支持stl习惯的写法, 添加函数式编程的支持
 */
template <typename T>
class iterator {
public:
	typedef typename A::difference_type difference_type;
	//typedef typename A::size_type = std::size_t;
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

template <typename T>
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





#endif
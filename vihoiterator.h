/**
 * @iterator�Ļ���ʵ��
 *
 * @������֧��stlϰ�ߵ�д��, ��Ӻ���ʽ��̵�֧��
 * @��ͬ��iterator֧�ֲ�ͬ�ķ��� �����ѡ��.
 * @random_iterator֧�ֵ����,������������±�.ĳЩiterator���ṩbegin(),end()
 * @author:vihoc 
 */

#ifndef __VIHO_STRUCTURES_ITERATOR_H
#define __VIHO_STRUCTURES_ITERATOR_H
#include <iterator>
#include <traits.h>
namespace VihoStructures {

	 /*
	 template <typename T, class A = std::allocator<T>>
	 class random_iterator {
	 public:
		 typedef typename A::difference_type difference_type;
		 //typedef typename A::size_type = std::size_t;
		 typedef typename A::value_type value_type;
		 typedef typename A::reference reference;
		 typedef typename A::pointer pointer;
		 typedef std::random_access_iterator_tag iterator_category;
		 random_iterator();
		 random_iterator(const random_iterator&);
		 ~random_iterator();

		 random_iterator& operator=(const random_iterator&);
		 bool operator==(const random_iterator&) const;
		 bool operator!=(const random_iterator&) const;
		 bool operator<(const random_iterator&) const;
		 bool operator>(const random_iterator&) const;
		 bool operator<=(const random_iterator&) const;
		 bool operator>=(const random_iterator&) const;

		 random_iterator& operator++();
		 random_iterator operator++(int);
		 random_iterator& operator--();
		 random_iterator operator--(int);
		 random_iterator& operator+=(size_type);
		 random_iterator operator+(size_type) const;
		 friend random_iterator operator+(size_type, const iterator&);
		 random_iterator& operator-=(size_type);
		 random_iterator operator-(size_type) const;
		 difference_type operator-(iterator) const;

		 reference operator*() const;
		 pointer operator->() const;
		 reference operator[](size_type) const;
	 };

	 template <typename T, class A = std::allocator<T>>
	 class const_random_iterator {
	 public:
		 typedef typename A::difference_type difference_type;
		 typedef typename A::value_type value_type;
		 typedef typename const A::reference reference;
		 typedef typename const A::pointer pointer;
		 typedef std::random_access_iterator_tag iterator_category;

		 const_random_iterator();
		 const_random_iterator(const const_random_iterator&);
		 const_random_iterator(const iterator&);
		 ~const_random_iterator();

		 const_random_iterator& operator=(const const_random_iterator&);
		 bool operator==(const const_random_iterator&) const;
		 bool operator!=(const const_random_iterator&) const;
		 bool operator<(const const_random_iterator&) const;
		 bool operator>(const const_random_iterator&) const;
		 bool operator<=(const const_random_iterator&) const;
		 bool operator>=(const const_random_iterator&) const;

		 const_random_iterator& operator++();
		 const_random_iterator operator++(int);
		 const_random_iterator& operator--();
		 const_random_iterator operator--(int);
		 const_random_iterator& operator+=(size_type);
		 const_random_iterator operator+(size_type) const;
		 friend const_random_iterator operator+(size_type, const const_iterator&);
		 const_random_iterator& operator-=(size_type);
		 const_random_iterator operator-(size_type) const;
		 difference_type operator-(const_iterator) const;

		 reference operator*() const;
		 pointer operator->() const;
		 reference operator[](size_type) const;
	 };
	 */
	//TODO �±�
	typedef int size_type;
	template <typename T>
	class random_iterator
	{
	public:
		typedef random_iterator self_type;
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;
		//typedef size_t difference_type;
		//typedef std::forward_iterator_tag iterator_category;
		typedef std::random_access_iterator_tag iterator_category;
		typedef int difference_type;
		random_iterator(pointer ptr) : ptr_(ptr) { }
		self_type operator++() { self_type i = *this; ptr_++; return i; }
		self_type operator++(int junk) { ptr_++; return *this; }
		self_type operator--() { self_type i = *this; ptr_--; return i; }
		self_type operator--(int junk) { ptr_--; return *this; }
		self_type operator+(int junk) { ptr_ += junk; return *this; }
		self_type operator-(int junk) { ptr_ -=junk; return *this; }
		difference_type operator-(const self_type junk) { return  ptr_ - junk.ptr_ ; }//TODO���߽�
		difference_type operator-(const self_type junk) const{ return  ptr_ - junk.ptr_; }//TODO���߽�
		reference operator*() { return *ptr_; }
		pointer operator->() { return ptr_; }
		reference operator*() const{ return *ptr_; }
		pointer operator->() const { return ptr_; }
		bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
		bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
		bool operator==(const self_type& rhs) const { return ptr_ == rhs.ptr_; }
		bool operator!=(const self_type& rhs) const { return ptr_ != rhs.ptr_; }

		bool operator<(const self_type& rhs) { return ptr_ < rhs.ptr_; }
		bool operator<(const self_type& rhs) const { return ptr_ < rhs.ptr_; }
		bool operator>(const self_type& rhs) { return ptr_ > rhs.ptr_; }
		bool operator>(const self_type& rhs) const { return ptr_ > rhs.ptr_; }
	private:
		pointer ptr_;
	};
	template <typename T>
	class const_random_iterator
	{
	public:
		typedef const_random_iterator self_type;
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;
		typedef int difference_type;
		typedef std::random_access_iterator_tag iterator_category;
		const_random_iterator(pointer ptr) : ptr_(ptr) { }
		self_type operator++() { self_type i = *this; ptr_++; return i; }
		self_type operator++(int junk) { ptr_++; return *this; }
		self_type operator--() { self_type i = *this; ptr_--; return i; }
		self_type operator--(int junk) { ptr_--; return *this; }
		self_type operator+(int junk) { ptr_ += junk; return *this; }
		self_type operator-(int junk) { ptr_ -= junk; return *this; }
		difference_type operator-(const self_type junk) { return junk.ptr_ - ptr_; }//TODO���߽�
		difference_type operator-(const self_type junk) const{ return  ptr_ - junk.ptr_; }//TODO���߽�
		const reference operator*() { return *ptr_; }
		const pointer operator->() { return ptr_; }
		const reference operator*() const{ return *ptr_; }
		const pointer operator->() const{ return ptr_; }
		bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
		bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
		bool operator==(const self_type& rhs) const { return ptr_ == rhs.ptr_; }
		bool operator!=(const self_type& rhs) const { return ptr_ != rhs.ptr_; }

		bool operator<(const self_type& rhs) { return ptr_ < rhs.ptr_; }
		bool operator<(const self_type& rhs) const{ return ptr_ < rhs.ptr_; }
		bool operator>(const self_type& rhs) { return ptr_ > rhs.ptr_; }
		bool operator>(const self_type& rhs) const{ return ptr_ > rhs.ptr_; }
	private:
		pointer ptr_;
	};

//TODO �ĵĸ�����
	//TODO �Ļ�Node,�͑����a��ԓ�� iter->fucker���L��Node
	typedef int size_type;
	template <typename T>
	class forward_iterator
	{
	public:
		typedef forward_iterator self_type;
		typedef typename T::value_type value_type;
		typedef typename T::value_type reference; //ʹ��T::value_refence ��������.
		typedef T* pointer;
		typedef size_t difference_type;
		//typedef std::forward_iterator_tag iterator_category;
		typedef std::forward_iterator_tag iterator_category;
//		typedef int difference_type;
		forward_iterator(pointer ptr) : ptr_(ptr) { }
		
		self_type operator++() { self_type i = *this; ptr_ = (*ptr_)++; return i; }
		self_type operator++(int junk) { ptr_ = (*ptr_)++; return *this; }
		self_type operator--() { self_type i = *this; ptr_ = (*ptr_)--; return i; }
		self_type operator--(int junk) { ptr_ = (*ptr_)--; return *this; }
	
		reference operator*() { return **ptr_; }
		pointer operator->() { return *ptr_; }
		const reference operator*() const { return **ptr_; }
		const pointer operator->() const { return *ptr_; }
		bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
		bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
		bool operator==(const self_type& rhs) const { return ptr_ == rhs.ptr_; }
		bool operator!=(const self_type& rhs) const { return ptr_ != rhs.ptr_; }

		bool operator<(const self_type& rhs) { return ptr_ < rhs.ptr_; }
		bool operator<(const self_type& rhs) const { return ptr_ < rhs.ptr_; }
		bool operator>(const self_type& rhs) { return ptr_ > rhs.ptr_; }
		bool operator>(const self_type& rhs) const { return ptr_ > rhs.ptr_; }
	private:
		pointer ptr_;
	};
	template <typename T>
	class const_forward_iterator
	{
	public:
		typedef const_forward_iterator self_type;
		typedef typename T::value_type value_type;
		typedef typename T::value_type& reference;
		typedef value_type* pointer;
		typedef size_t difference_type;
		typedef std::forward_iterator_tag iterator_category;
		const_forward_iterator(pointer ptr) : ptr_(ptr) { }
		self_type operator++() { self_type i = *this; ptr_++; return i; }
		self_type operator++(int junk) { ptr_++; return *this; }
		self_type operator--() { self_type i = *this; ptr_--; return i; }
		self_type operator--(int junk) { ptr_--; return *this; }
		const reference operator*() { return *ptr_; }
		const pointer operator->() { return ptr_; }
		const reference operator*() const { return *ptr_; }
		const pointer operator->() const { return ptr_; }
		bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
		bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
		bool operator==(const self_type& rhs) const { return ptr_ == rhs.ptr_; }
		bool operator!=(const self_type& rhs) const { return ptr_ != rhs.ptr_; }

		bool operator<(const self_type& rhs) { return ptr_ < rhs.ptr_; }
		bool operator<(const self_type& rhs) const { return ptr_ < rhs.ptr_; }
		bool operator>(const self_type& rhs) { return ptr_ > rhs.ptr_; }
		bool operator>(const self_type& rhs) const { return ptr_ > rhs.ptr_; }
	private:
		pointer ptr_;
	};
}

#endif
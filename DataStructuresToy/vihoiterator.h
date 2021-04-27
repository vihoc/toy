/**
 * @iterator的基本实现
 *
 * @让容器支持stl习惯的写法, 添加函数式编程的支持
 * @不同的iterator支持不同的方法 请谨慎选择.
 * @random_iterator支持的最多,可以任意访问下标.某些iterator仅提供begin(),end()
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
	//TODO 下标
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
		//operator++() pre-increment
		self_type operator++() { ptr_++; return *this; }
		//operator++() post-increment
		self_type operator++(int junk) { self_type i = *this; ptr_++; return i; }
		self_type operator--() { ptr_--; return *this; }
		self_type operator--(int junk) { self_type i = *this; ptr_--; return i; }
		//+不能修改本身. 于+=相同
		self_type operator+(int junk) { self_type i = *this;  i.ptr_ += junk; return i; }
		self_type operator-(int junk) { self_type i = *this;  i.ptr_ -= junk; return i; }

		//difference_type operator+(const self_type junk) { return  ptr_ + junk.ptr_ ; }//TODO检查边界
		difference_type operator-(const self_type junk) const{ return  ptr_ - junk.ptr_; }//TODO检查边界
		reference operator*() { return *ptr_; }
		pointer operator->() { return ptr_; }
		reference operator*() const{ return *ptr_; }
		pointer operator->() const { return ptr_; }
		//TODO untested
		bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
		bool operator!=(const self_type& rhs) { return  ptr_ != rhs.ptr_; }
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
		typedef const T& reference;
		typedef T* pointer;
		typedef int difference_type;
		typedef std::random_access_iterator_tag iterator_category;
		const_random_iterator(pointer ptr) : ptr_(ptr) { }
		self_type operator++() { ptr_++; return *this; }
		self_type operator++(int junk) { self_type i = *this; ptr_++; return i; }
		self_type operator--() { ptr_--; return *this; }
		self_type operator--(int junk) { self_type i = *this; ptr_--; return i; }
		self_type operator+(int junk) { self_type i = *this;  i.ptr_ += junk; return i; }
		self_type operator-(int junk) { self_type i = *this;  i.ptr_ -= junk; return i; }
		difference_type operator-(const self_type junk) { return junk.ptr_ - ptr_; }//TODO检查边界
		difference_type operator-(const self_type junk) const{ return  ptr_ - junk.ptr_; }//TODO检查边界
		reference operator*() { return *ptr_; }
		pointer operator->() { return ptr_; }
		reference operator*() const { return *ptr_; }
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

//TODO 改的更清晰
	typedef int size_type;
	template <typename T>
	class bidirectional_iterator
	{
	public:
		typedef bidirectional_iterator self_type;
		typedef typename T::value_type value_type;
		typedef typename T::value_type reference; //使用T::value_refence 会编译错误.
		typedef T* pointer;
		typedef size_t difference_type;
		//typedef std::forward_iterator_tag iterator_category;
		typedef std::bidirectional_iterator_tag iterator_category;
//		typedef int difference_type;

		explicit bidirectional_iterator(pointer ptr) : ptr_(ptr) { }
		bidirectional_iterator(bidirectional_iterator& iter) :ptr_(iter.ptr_) {}
		bidirectional_iterator(bidirectional_iterator&& iter) :ptr_(iter.ptr_) {}
		bidirectional_iterator(const bidirectional_iterator& iter) :ptr_(iter.ptr_) {}
		bidirectional_iterator(const bidirectional_iterator&& iter) :ptr_(iter.ptr_) {}

		self_type operator++() { ptr_ = (*ptr_)++; return *this; }
		self_type operator++(int junk) { self_type i = *this; ptr_ = (*ptr_)++; return i; }
		self_type operator--() { ptr_ = (*ptr_)--; return *this; }
		self_type operator--(int junk) { self_type i = *this; ptr_ = (*ptr_)--; return i; }


		self_type operator++() const { ptr_ = (*ptr_)++; return *this; }
		self_type operator++(int junk) const { self_type i = *this; ptr_ = (*ptr_)++; return i; }
		self_type operator--() const{ ptr_ = (*ptr_)--; return *this; }
		self_type operator--(int junk) const{ self_type i = *this; ptr_ = (*ptr_)--; return i; }

		self_type operator=(self_type other) { ptr_ = other.ptr_; return *this; }

		reference operator*() { return **ptr_; }
		pointer operator->() { return ptr_; }

		
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
/*protected:*/
//	const pointer getiterptr() { return *ptr_; }
	private:
		pointer ptr_;
	};
	template <typename T>
	class const_bidirectional_iterator
	{
	public:
		typedef const_bidirectional_iterator self_type;
		typedef typename T::value_type value_type;
		typedef typename T::value_type reference;
		typedef T* pointer;
		typedef size_t difference_type;
		typedef std::bidirectional_iterator_tag iterator_category;
		explicit const_bidirectional_iterator(pointer ptr) : ptr_(ptr) { }
		const_bidirectional_iterator(const_bidirectional_iterator& iter) :ptr_(iter.ptr_) {}
		const_bidirectional_iterator(const_bidirectional_iterator&& iter) :ptr_(iter.ptr_) {}
		//~bidirectional_iterator() { }
		//const_bidirectional_iterator(const const_bidirectional_iterator&) = default;
		self_type operator++() { ptr_ = (*ptr_)++; return *this; }
		self_type operator++(int junk) { self_type i = *this; ptr_ = (*ptr_)++; return i; }
		self_type operator--() { ptr_ = (*ptr_)--; return *this; }
		self_type operator--(int junk) { self_type i = *this; ptr_ = (*ptr_)--; return i; }


		self_type operator++() const { ptr_ = (*ptr_)++; return *this; }
		self_type operator++(int junk) const { self_type i = *this; ptr_ = (*ptr_)++; return i; }
		self_type operator--() const { ptr_ = (*ptr_)--; return *this; }
		self_type operator--(int junk) const { self_type i = *this; ptr_ = (*ptr_)--; return i; }

		self_type operator=(self_type other) { std::swap(*this, other); }

		const reference operator*() { return **ptr_; }
		const pointer operator->() { return *ptr_; }
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



	typedef int size_type;
	template <typename T>
	class forward_iterator
	{
	public:
		typedef forward_iterator self_type;
		typedef typename T::value_type value_type;
		typedef typename T::value_type reference; //使用T::value_refence 会编译错误.
		typedef T* pointer;
		typedef size_t difference_type;
		//typedef std::forward_iterator_tag iterator_category;
		typedef std::forward_iterator_tag iterator_category;
		//		typedef int difference_type;
		forward_iterator(pointer ptr) : ptr_(ptr) { }

		self_type operator++() { ptr_ = (*ptr_)++; return *this; }
		self_type operator++(int junk) { self_type i = *this; ptr_ = (*ptr_)++; return i; }
		self_type operator--() { ptr_ = (*ptr_)--; return *this; }
		self_type operator--(int junk) { self_type i = *this; ptr_ = (*ptr_)--; return i; }

		reference operator*() { return **ptr_; }
		pointer operator->() { return *ptr_; }
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
		typedef typename T::value_type reference;
		typedef T* pointer;
		typedef size_t difference_type;
		typedef std::forward_iterator_tag iterator_category;
		const_forward_iterator(pointer ptr) : ptr_(ptr) { }
		self_type operator++() { ptr_ = (*ptr_)++; return *this; }
		self_type operator++(int junk) { self_type i = *this; ptr_ = (*ptr_)++; return i; }
		self_type operator--() { ptr_ = (*ptr_)--; return *this; }
		self_type operator--(int junk) { self_type i = *this; ptr_ = (*ptr_)--; return i; }


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

} //namespace VihoStructures

#endif

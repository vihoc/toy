/*
*可定制的二叉樹基本實現, 二叉平衡樹可以直接用AVLtree
* 
* 
*/

#ifndef _VIHO_STRUCTURES_TREE_H
#define _VIHO_STRUCTURES_TREE_H

#include <array_list.h>

namespace VihoStructures {
	/**
	* 簡易N類 ,請不要使用.
	* 定義一個能讓Tree<T, N>使用的最小N類
	* 例如:VihoStructures::Tree<int, VihoStructures::sampleNode<int>> testTree = { 1, 3, 5, 6, 8 };
	* 目前只支持二叉樹,TODO 特化成二叉樹
	*/
	template<typename T>
	class sampleNode
	{
	public:
		explicit sampleNode(const T& data_){}

		sampleNode(const T& data_, sampleNode<T>* parent_) : sampleNode <T>{ data_, parent_ } {}
		//定義此函數向樹内插入某值
		static sampleNode<T>* insert(sampleNode<T>* node, const T& data) { return nullptr; }
		//定義此函數直接刪除某值
		static sampleNode<T>* remove(sampleNode<T>* node, const T& data) { return nullptr; }

		bool contains(const T& data) { return false; }
		bool contains(const T& data) const { return false; }
		void post_order(ArrayList<T>&) const { }
		void in_order(ArrayList<T>&) const { }
		void pre_order(ArrayList<T>&) const { }
		//virtual void print(int indent) const = 0;
	private:
		//virtual void destoryTree() = 0;
	private:
		sampleNode<T>* childList;
		T data;
		//optional
		sampleNode<T>* father;
	};
	

/**
 * @ 模板參數T為存儲的數據類型
 * @ 模板參數Node為tree内部保存節點的數據類型
 * @ Node類定義如上
 * 
 */
template <typename T, typename Node>
class Tree {
public:
	
	Tree() = default;
	using self_type = Tree<T, Node>;
	using array_type = ArrayList<T>;
	/*
	*支持C++11的初始化列表初始化樹
	*/
	Tree(std::initializer_list <T>&& list)
	{
		for (auto e : list)
		{
			insert(e);
		}
	}

	explicit Tree(const Tree<T, Node>& other) {
		auto list = other.items();
		for (std::size_t i = 0; i < list.size(); i++) {
			insert(list[i]);
		}
	}
	/*
	explicit Tree(array_type& array)
	{
		//clear();
		for (auto e : array)
		{
			insert(e);
		}
	}
	*/

	Tree(self_type&& other) : root{other.root}, size_{other.size_} {
		other.root = nullptr;
		other.size_ = 0;
	}

	self_type & operator=(const self_type& other) {
		Tree copy{other};
		std::swap(root, copy.root);
		std::swap(size_, copy.size_);
		return *this;
	}

	self_type& operator=(self_type && other) {
		Tree copy{std::move(other)};
		std::swap(root, copy.root);
		std::swap(size_, copy.size_);
		return *this;
	}



	/**
	 * @析構函數
	 */
	~Tree() 
	{ 
		if (nullptr != root)
		{
			delete root;
		}
	}
	void ImportDataFromArray(const array_type& array)
	{
		//clear();
		for (auto e : array)
		{
			insert(e);
		}
	}
	/**
	 * @向樹内添加值
	 */
	bool insert(const T& data)
	{
		if (root) {
			if (!Node::insert(root, data))
				return false;
		} else {
			root = new Node(data);
		}
		++size_;
		return true;
	}

	/**
	 * @從樹内刪除數據
	 * @參數'data',被刪除的值,如果存在樹内
	 * 返回值.data被刪除了,返回true, 其他為false
	 */
	bool remove(const T& data)
	{
		if (nullptr != root) {
			if (root->data == data) {
				if ((nullptr != root->right) && (nullptr != root->left)) {
					root->data = root->substitute();
					Node::remove((Node*) root->right, root->data);
				} else {
					Node* n;
					if (nullptr != root->right) {
						n = (Node*) root->right;
						root->right = nullptr;  // Avoids recursive deletion
					} else {
						n = (Node*) root->left;
						root->left = nullptr;  // Avoids recursive deletion
					}

					delete root;
					root = (Node*) n;
					if (nullptr != root)
						root->parent = nullptr;
				}
				--size_;
				return true;
			} else if (Node::remove(root, data)) {
				--size_;
				return true;
			} else {
				return false;
			}
		}
		return false;
	}

	/**
	 * @檢查樹内是否有'data'
	 * 
	 */
	bool contains(const T& data) const
	{
		return root ? root->contains(data) : false;
	}

	void clear() 
	{
		while (size_ > 0)
			remove(root->data);
	}

	/**
	 * @返回數的節點個數
	 */
	std::size_t size() const { return size_; }

	//TODO, 替換成模板 
	ArrayList<T> items() const { return std::move(ArrayList<T>{pre_order()}); }

	/**
	 * @前序遍歷樹
	 */
	ArrayList<T> pre_order() const 
	{
		ArrayList<T> out{size_};
		if (nullptr != root)
			root->pre_order(out);
		return out;
	}

	/**
	 * @中序遍歷
	 */
	ArrayList<T> in_order() const
	{
		ArrayList<T> out{size_};
		if (nullptr != root)
			root->in_order(out);
		return out;
	}

	/**
	 * @層序遍歷
	 */
	ArrayList<T> post_order() const 
	{
		ArrayList<T> out{size_};
		if (nullptr!= root)
			root->post_order(out);
		return out;
	}

	/**
	 * @圖形化打印樹的内容
	 */
	void print() const
	{
		std::cout << "Tree size = " << size_;
		if (root) {
			std::cout << std::endl << std::endl;
			root->print(0);
			for (int i = 0; i < 80; ++i) {
				std::cout << "-";
			}
		}
		std::cout << std::endl;
	}

protected:
	Node* root{nullptr};
	std::size_t size_{0u};
};

}  // namespace VihoStructures

#endif

/*
*二叉樹節點實現
*author:vihoc
*
*/

#ifndef _VIHO_STRUCTURES_BINARY_TREE_H
#define _VIHO_STRUCTURES_BINARY_TREE_H

#include <iostream>

#include <traits.h>
#include <tree.h>

namespace VihoStructures {

template <typename T>
class AVLNode;

template <typename T>
struct Node {
	explicit Node(const T& data_) : data{data_} {}

	Node(const T& data_, Node* parent_) : data{data_}, parent{parent_} {}

	virtual ~Node() {
		if(nullptr != left) delete left;
		if(nullptr != right) delete right;
	}

	static Node<T>* insert(Node<T>* node, const T& data_) {
		if (data_ < node->data) {
			// insert left
			if (node->left) {
				return insert(node->left, data_);
			} else {
				node->left = new Node(data_, node);
				return node->left;
			}
		} else if (data_ > node->data) {
			// insert right
			if (node->right) {
				return insert(node->right, data_);
			} else {
				node->right = new Node(data_, node);
				return node->right;
			}
		} else {
			return nullptr;
		}
	}

	static Node<T>* remove(Node<T>* node, const T& data_) {
		if (node->data == data_) {
			if (node->right && node->left) {
				node->data = node->substitute();
				return remove(node->right, node->data);
			} else {
				auto n = node->right ? node->right : node->left;

				if (node->parent->right == node) {
					node->parent->right = n;
				} else {
					node->parent->left = n;
				}

				if (n)
					n->parent = node->parent;

				node->left = nullptr;
				node->right = nullptr;

				auto parent = node->parent;
				delete node;
				return parent;
			}
		} else {
			auto n = data_ < node->data ? node->left : node->right;
			return n ? remove(n, data_) : nullptr;
		}
	}

	bool contains(const T& data_) const {
		if (data == data_) {
			return true;
		} else {
			if (data_ < data) {
				return left ? left->contains(data_) : false;
			} else {
				return right ? right->contains(data_) : false;
			}
		}
	}

	void pre_order(ArrayList<T>& v) const {
		v.push_back(data);
		if (left)
			left->pre_order(v);
		if (right)
			right->pre_order(v);
	}

	void in_order(ArrayList<T>& v) const {
		if (left)
			left->in_order(v);
		v.push_back(data);
		if (right)
			right->in_order(v);
	}

	void post_order(ArrayList<T>& v) const {
		if (left)
			left->post_order(v);
		if (right)
			right->post_order(v);
		v.push_back(data);
	}

	virtual void destoryTree()
	{

	}
	// 返回樹内最小值
	T substitute() const {
		Node* it = right;
		while (it->left) {
			it = it->left;
		}
		return it->data;
	}

	virtual void print(int indent) const {
		if (nullptr != right) right->print(indent + 1);
		for (int i = 0; i < indent; ++i)
			std::cout << "    ";
		std::cout << data << std::endl;
		if (nullptr != left) left->print(indent + 1);
	}

	T data{};
	Node* parent{nullptr};
	Node* left{nullptr};
	Node* right{nullptr};

protected:
	Node<T>* find_node_to_delete(const T& data_) {
		if (data == data_) {
			if (right && left) {
				data = substitute();
				return right->find_node_to_delete(data);
			} else {
				return this;
			}
		} else {
			if (data_ < data)
				return left ? left->find_node_to_delete(data_) : nullptr;
			else
				return right ? right->find_node_to_delete(data_) : nullptr;
		}
	}
};

/**
 * @非平衡二叉樹
 *
 * @時間複雜度為O(logn) , 因爲不是平衡樹最壞情況下為O(N),某些插入操作可能會變成O(n)
 * 
 */
template <typename T>
class BinaryTree : public Tree<T, Node<T>> {};

}  // namespace VihoStructures

/* set trait */
template <>
const bool traits::is_set<VihoStructures::BinaryTree>::value = true;

/* name trait */
template <>
const std::string traits::type<VihoStructures::BinaryTree>::name = "BinaryTree";

#endif

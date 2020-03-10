#pragma once
#include<iostream>
enum Boolean { FALSE, TRUE }; //自定义布尔类

template<class T>
class Element {  //可以添加更多的数据，灵活调整
public:
	T key;
};

template <class T> class BSTree; //加入BSTree的声明，防止在声明友元时出错

template<class T>
class BSNode {
	friend class BSTree<T>; //将BSTree作为友元使之可以访问私有成员
public:
	Element<T> data;
	BSNode<T> *left;
	BSNode<T> *right;
	void display(int i);
};

template <class T>
class BSTree
{
private:
	BSNode<T> *root;
public:
	BSTree(BSNode<T>*init = nullptr) { root = init; }
	~BSTree() {
		clear(root); root = nullptr;
	}
	void clear(BSNode<T>*node);
	Boolean Insert(const Element<T> &data);
	BSNode<T> *Search(const Element<T> &data);
	BSNode<T> *Search(BSNode<T>*, const Element<T> &data);
	BSNode<T> *IterSearch(const Element<T>&data);
	BSNode<T> *remove(Element<T> &data);
	BSNode<T> *remove(BSNode<T> *node, Element<T> &data);
	void MidOrder();
	void MidOrder(BSNode<T>* node);
	void display();
};

template<class T>
void BSTree<T>::clear(BSNode<T>* node) {
	if (node)
	{
		clear(node->left);
		clear(node->right);
		delete node;
		node = nullptr;
	}
}

template <class T>
void BSNode<T>::display(int i)
{
	std::cout << "Position: " << i; // i是结点的位置，按照层序遍历往下数
	std::cout << "   DataKey: " << data.key << std::endl;  // 显示数据
	if (left) left->display(2 * i);
	if (right) right->display(2 * i + 1);
}

template<class T>
Boolean BSTree<T>::Insert(const Element<T> &data) {
	BSNode<T> *p = root;       // 当前节点
	BSNode<T> *q = nullptr;    // 用于指向当前节点的父节点
	//insert之前需要先查找
	while (p) {
		q = p; //每次改变p之前，用q记录
		if (data.key == p->data.key) return FALSE;//插入失败
		else if (data.key < p->data.key) { p = p->left; }//向左
		else p = p->right; //向右
	}
	//当循环结束后找到的位置为q
	p = new BSNode<T>;
	p->left = p->right = nullptr;
	p->data = data;
	if (!root) root = p;
	else if (data.key < q->data.key) q->left = p;
	else q->right = p;
	return TRUE;//表示插入成功
}

template <class T>
BSNode<T> * BSTree<T>::remove(Element<T> &data) {
	root = remove(root, data);
	return root;
}

// 删除结点 并 返回被删除的节点
template<class T>
BSNode<T> * BSTree<T>::remove(BSNode<T> *node, Element<T> &data) {
	if (node == nullptr) return node;
	if (data.key < node->data.key)
		node->left = remove(node->left, data);
	else if (data.key > node->data.key)
		node->right = remove(node->right, data);
	else
	{ //找到了待删除结点
		if (node->left != nullptr &&  node->right != nullptr) //若待删除结点度为2
		{
			BSNode<T> * maxleft = node->left; //去找左子树的最大值替换待删除结点
			while (maxleft->right != nullptr)
			{
				maxleft = maxleft->right;
			}
			node->data = maxleft->data; // 替换
			node->left = remove(node->left, data); // 删除最大结点
		}
		else // 待删除结点的度是0或者1
		{
			BSNode<T> *temp = nullptr;
			if (node->left == nullptr) {
				temp = node->right;
				delete node;
				return temp;
			}
			else if(node->right == nullptr)
			{
				temp = node->left;
				delete node;
				return temp;
			}
		}
	}
	return node;
}


template<class T>
void BSTree<T>::display() {
	if (root) {
		root->display(1);
	}
	else std::cout << "empty tree\n";
}

// 递归的查找
template<class T>
BSNode<T>* BSTree<T>::Search(const Element<T> &data) {
	return Search(root, data);
}
template<class T>
BSNode<T>* BSTree<T>::Search(BSNode<T>* node, const Element<T> &data) {
	if (!node) return nullptr;
	if (node->data.key == data.key) return node;
	else if ((node->data.key < data.key))
	{
		return Search(node->right, data);
	}
	else
	{
		return Search(node->left, data);
	}
}

//迭代的查找
template<class T>
BSNode<T>* BSTree<T>::IterSearch(const Element<T>&data) {
	for (BSNode<T>*node = root; node;)
	{
		if (node->data.key == data.key) return node;
		else if (node->data.key < data.key) node = node->right;
		else node = node->left;
	}
}

template <class T>
void BSTree<T>::MidOrder() {
	MidOrder(root);
}

template <class T>
void BSTree<T>::MidOrder(BSNode<T>* node) {
	if (!node) return;
	MidOrder(node->left);
	std::cout << node->data.key << std::endl;
	MidOrder(node->right);
}

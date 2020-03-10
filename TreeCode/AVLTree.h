#pragma once
#include<iostream>
template<class T>
class AVLNode {
public:
	T key;
	int height;
	AVLNode * left;
	AVLNode * right;
	AVLNode(T value, AVLNode *l, AVLNode *r) :
		key(value), height(0), left(l), right(r) {}
};

template<class T>
class AVLTree
{
public:
	AVLTree():root(nullptr){}
	AVLTree(AVLNode<T> *r) :root(r) {}
	~AVLTree() { clear(root); root = nullptr; }
	int height();                 // 获取树的高度
	AVLNode<T> * insert(T key);
	AVLNode<T> * remove(T key);

	// 删除AVL树
	void clear(AVLNode<T> *root);

	void preOrder();
	void midOrder();
	void postOrder();

private:
	AVLNode<T> *root;
	int height(AVLNode<T> *node); // 获取结点的高度
	int max(int a, int b);
	int getBalance(AVLNode<T> *node); // 获取平衡因子

	// 自平衡的四种方式
	AVLNode<T> *  LLrotation(AVLNode<T>* node);
	AVLNode<T> *  RRrotation(AVLNode<T>* node);
	AVLNode<T> *  LRrotation(AVLNode<T>* node);
	AVLNode<T> *  RLrotation(AVLNode<T>* node);

	// 插入结点
	AVLNode<T> * insert(AVLNode<T> * root, T key);
	// 删除节点
	AVLNode<T> * remove(AVLNode<T> *root, T key);

	void preOrder(AVLNode<T> *node);
	void midOrder(AVLNode<T> *node);
	void postOrder(AVLNode<T> *node);
};

/**********************获取AVL树的高度*******************/
template<class T> int AVLTree<T>::height(AVLNode<T> *node){
	if (node == nullptr) return 0;
	return node->height;
}
template<class T> int AVLTree<T>::height() {
	return height(root);
}
/**********************获取AVL结点的平衡因子*******************/
template<class T> int AVLTree<T>::getBalance(AVLNode<T>*node) {
	if (node == nullptr) return 0;
	return height(node->left) - height(node->right);
}
/**********************更新结点比较大小*******************/
template<class T> int AVLTree<T>::max(int a, int b) {
	return (a > b) ? a : b;
}

/************AVL树失衡的四种姿态对应的自平衡的方式***********/
template<class T> AVLNode<T> * AVLTree<T>::LLrotation(AVLNode<T>* g) {
	// 右旋祖父结点，让祖父结点下去
	// g:祖父结点(失衡结点) p:父结点 
	AVLNode<T> *p = g->left;
	g->left = p->right;
	p->right = g;
	// 更新结点高度
	g->height = 1+max(height(g->left), height(g->right));
	p->height = 1+max(height(p->left), height(p->right));

	return p;
}
template<class T> AVLNode<T> * AVLTree<T>::RRrotation(AVLNode<T> *g) {
	// 左旋祖父结点，让祖父结点下去
	// g:祖父结点(失衡结点) p:父结点 
	AVLNode<T> *p = g->right;
	g->right = p->left;
	p->left = g;
	// 更新结点高度
	g->height = 1 + max(height(g->left), height(g->right));
	p->height = 1 + max(height(p->left), height(p->right));

	return p;
}
template<class T> AVLNode<T> * AVLTree<T>::LRrotation(AVLNode<T> *g) {
	// 左旋p + 右旋g = RR + LL
	// g:祖父结点(失衡结点) p:父结点 
	g->left = RRrotation(g->left);
	return LLrotation(g);
}
template<class T> AVLNode<T> * AVLTree<T>::RLrotation(AVLNode<T> *g) {
	// 右旋p + 左旋g = LL + RR
	// g:祖父结点(失衡结点) p:父结点 
	g->right = LLrotation(g->right);
	return RRrotation(g);;
}

/**********************AVL 结点插入*******************/
template<class T> AVLNode<T> * AVLTree<T>::insert(AVLNode<T> * node, T key) {
	// 1. 执行标准BST插入操作
	if (node == nullptr)
		return new AVLNode<T>(key, nullptr, nullptr);
	else if (key < node->key)
		node->left = insert(node->left, key);
	else if (key > node->key)
		node->right = insert(node->right, key);
	else
		return node;// 不允许插入重复结点
	
	// 2. 更新该祖先节点的高度
	node->height = max(height(node->left), height(node->right)) + 1;

	// 3. 获取平衡因子，并判断是否失衡
	int balance = getBalance(node);
	// 4. 若失衡，则判断四种失衡情况
	if (balance > 1 && key < node->left->key) return LLrotation(node);
	if (balance > 1 && key > node->left->key) return LRrotation(node);
	if (balance < -1 && key > node->right->key) return RRrotation(node);
	if (balance < -1 && key < node->right->key) return RLrotation(node);

	return node;
}  

template<class T> AVLNode<T> * AVLTree<T>::insert(T key) {
	root =  insert(root, key);
	return root;
}

/**********************AVL 删除树*******************/
template<class T> void AVLTree<T>::clear(AVLNode<T> *node) {
	if (node != nullptr) {
		clear(node->left);
		clear(node->right);
		delete node;
		node = nullptr;
	}
}

/**********************AVL 结点删除*******************/
template <class T> AVLNode<T> * AVLTree<T>::remove(T key) {
	root = remove(root, key);
	return root;
}

// 删除结点 并 返回被修正子树的根结点
template<class T> AVLNode<T> * AVLTree<T>::remove(AVLNode<T> *node, T key) {
	// 1. 执行标准BST删除
	if (node == nullptr) {
		std::cout << "结点不存在，无法删除\n";
		return nullptr;
	}
	if (key < node->key){
		node->left = remove(node->left, key);
	}
	else if (key > node->key) {
		node->right = remove(node->right, key);
	}
	else{
		// 找到了要被删除的节点，开始分类讨论
		// 若删除结点的度为2
		if (node->left != nullptr && node->right != nullptr) {
			// 找到待删除结点的右子树的最小值来替换待删除结点
			AVLNode<T> * minright = node->right;
			while (minright->left){
				minright = minright->left;
			}
			node->key = minright->key;  // 替换待删除结点
			node->right = remove(node->right, minright->key); // 删除被替换的结点
		}
		else // 若待删除结点的度为0或1,删除待删除结点
		{   
			// 并将node设置成为非空子节点（度为1）
			// 或者设为空指针（度为0）
			AVLNode<T> * temp = node;
			node = node->left ? node->left : node->right;
			delete temp;
		}
	}

	// 2. 更新当前节点的高度
	if (node == nullptr) return nullptr; //对于空节点无法也无需更新高度
	node->height = 1 + max(height(node->left), height(node->right));

	// 3. 获取当前节点的平衡因子，验证是否失衡
	int balance = getBalance(node);
	// 4. 恢复平衡的四种情况
	if (balance > 1) {  //左侧失衡情况，可以分为LR,LL
		if (getBalance(node->left) >= 0) return LLrotation(node);
		else return LRrotation(node);
	}
	if (balance < -1) { // 右侧失衡情况，可以分为RL,RR
		if (getBalance(node->right) > 0) return RLrotation(node);
		else return RRrotation(node);
	}
	return node;
}

/**********************树的各种遍历*************************************/
template<class T> void AVLTree<T>::preOrder(AVLNode<T> *node) {
	if (node) {
		std::cout << node->key << " "; preOrder(node->left); preOrder(node->right);
	}
}

template<class T> void AVLTree<T>::midOrder(AVLNode<T> *node) {
	if (node){
		midOrder(node->left); std::cout << node->key << " "; midOrder(node->right);
	}	
}

template<class T> void AVLTree<T>::postOrder(AVLNode<T> *node) {
	if (node) {
		postOrder(node->left); postOrder(node->right); std::cout << node->key<<" ";
	}
}
template<class T> void AVLTree<T>::preOrder() { preOrder(root); }
template<class T> void AVLTree<T>::midOrder() { midOrder(root); }
template<class T> void AVLTree<T>::postOrder() { postOrder(root); }
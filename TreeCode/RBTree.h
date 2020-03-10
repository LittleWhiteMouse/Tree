#pragma once
#include<iostream>
enum COLOR {RED,BLACK};
template<class T> class RBTree;

template<class T>
class RBTNode {
private:
	T key;
	COLOR color; // 红色为0，黑色为1,db为2
	RBTNode<T> * left;
	RBTNode<T> * right;
	RBTNode<T> * parent;
public:
	RBTNode(T value):key(value),color(RED),left(nullptr),right(nullptr),parent(nullptr) {}
	friend  class RBTree<T>;
};

template<class T>
class RBTree{
private:
	RBTNode<T> * root;
public:
	RBTree() :root(nullptr) {}
	~RBTree() { clear(root); root = nullptr; }
	void clear(RBTNode<T>*node);
	void MidOrder();
	void insert(T key);
	void remove(T key);
private:
	// 遍历基操
	void MidOrder(RBTNode<T>* node);
	// 旋转基操
	void leftrotate(RBTNode<T>*node);
	void rightrotate(RBTNode<T>*node);
	// 颜色基操
	COLOR getcolor(RBTNode<T>*node);
	// 插入基操
	void LLinsert(RBTNode<T>*node);
	void RRinsert(RBTNode<T>*node);
	RBTNode<T>* BSTinsert(T key);
	void fixColor(RBTNode<T>* node);
	// 删除基操
	RBTNode<T>* getfar(RBTNode<T>*u);
	RBTNode<T>* getnear(RBTNode<T>*u);
	void farred(RBTNode<T>*u);
	void farblack(RBTNode<T>*u);
	void fixdb(RBTNode<T>*u);
	RBTNode<T>* getnodeBST(RBTNode<T>*node,T key);
	RBTNode<T>* getPred(RBTNode<T>* node);
	RBTNode<T>* getSucc(RBTNode<T>* node);
};

template<class T> RBTNode<T>* RBTree<T>::getfar(RBTNode<T>*u) {
	RBTNode<T> * p = u->parent;
	RBTNode<T> * s = nullptr;
	if (u == p->left) {
		s = p->right;
		return s->right;
	}
	else
	{
		s = p->left;
		return s->left;
	}
}
template<class T> RBTNode<T>* RBTree<T>::getnear(RBTNode<T>*u) {
	RBTNode<T> * p = u->parent;
	RBTNode<T> * s = nullptr;
	if (u == p->left) {
		s = p->right;
		return s->left;
	}
	else
	{
		s = p->left;
		return s->right;
	}
}
template<class T> void RBTree<T>::farred(RBTNode<T>*u) {
	RBTNode<T> * p = u->parent;
	RBTNode<T> * s = (u == p->left)?p->right:p->left;
	RBTNode<T>* far = getfar(u);
	if (far->color == RED)
	{
		s->color = p->color;
		p->color = BLACK;
		far->color = BLACK;
		if (s == p->right) leftrotate(p);
		else rightrotate(p);
	}
}
template<class T> void RBTree<T>::farblack(RBTNode<T>*u) {
	RBTNode<T> * p = u->parent;
	RBTNode<T> * s = nullptr;
	if (u == p->left) s = p->right;
	else s = p->left;
	RBTNode<T>* far = getfar(u);
	RBTNode<T>* near = getnear(u);

	if ((getcolor(far) == BLACK) && (getcolor(near) == RED))
	{
		s->color = RED;
		near->color = BLACK;
		if (s == p->right) rightrotate(s);
		else leftrotate(s);
		farred(u);
	}
}

template<class T> void RBTree<T>::fixdb(RBTNode<T>*u) {	// 当u是db的情况
	if (u == root) return;
	
	RBTNode<T> * p = u->parent;
	RBTNode<T> * s = ((u == p->left) ? p->right : p->left);
	if (getcolor(s) == RED) {
		s->color = BLACK;
		p->color = RED;
		if (s == p->right) leftrotate(p);
		else rightrotate(p);
		// 如果进入过s为红的情况，就一定要刷新sp,再进入到s为黑的情况
	}
	p = u->parent;
	s = ((u == p->left) ? p->right : p->left);

	if (getcolor(s) == BLACK){
		if ((getcolor(s->left) == BLACK) && (getcolor(s->right) == BLACK)){
			if (getcolor(p)==RED) {
				p->color = BLACK; s->color = RED;
				return;
			}
			else {
				s->color = RED; fixdb(p);
			}
		}
		else { //s为黑,xy至少有一红
			RBTNode<T>* far = getfar(u);
			if (getcolor(far) == RED)
				farred(u);
			else  farblack(u);
		}
	}
}
template<class T> void RBTree<T>::remove(T key) {
	RBTNode<T> * u = getnodeBST(root, key);  //找到待删除结点
	if (u == nullptr) {
		std::cout << "结点不存在" << std::endl;
		return;
	}
	if (u == root) { delete root; root = nullptr; return; }
	else if(u->color == RED)
	{
		if (u == u->parent->left) u->parent->left = nullptr;
		else u->parent->right = nullptr;
	}
	else {
			// 当u是db，调用db修正函数
		fixdb(u);
		if (u == u->parent->left) u->parent->left = nullptr;
		else u->parent->right = nullptr;
	}
	delete u; 
	u = nullptr; 
	return;
}
// 返回待被删除结点的位置
template<class T> RBTNode<T>* RBTree<T>::getnodeBST(RBTNode<T>*node, T key) {
	if (node == nullptr) return node;
	if (key < node->key) return getnodeBST(node->left, key);
	else if (key > node->key) return getnodeBST(node->right, key);
	else { // 找到了要删除的节点
		
		if (node->left != nullptr) {
			RBTNode<T> * maxleft = getPred(node);
			node->key = maxleft->key;
			return getnodeBST(node->left, maxleft->key);
		}
		else if(node->right!=nullptr) {
			RBTNode<T> *minright = getSucc(node);
			node->key = minright->key;
			return getnodeBST(node->right, minright->key);
		}
		else { //若为叶子节点，则
			return node;
		}
	}
}
// 返回node的最大前驱
template<class T> RBTNode<T>* RBTree<T>::getPred(RBTNode<T>* node) {
	RBTNode<T>* maxleft = node->left;
	while (maxleft->right != nullptr) {
		maxleft = maxleft -> right;
	}
	return maxleft;
}
// 返回node的最小后继
template<class T> RBTNode<T>* RBTree<T>::getSucc(RBTNode<T>* node) {
	RBTNode<T>* minright = node->right;
	while (minright->left != nullptr) {
		minright = minright->left;
	}
	return minright;
}

/********************************************************/

// 以x的孩子为圆心，对x进行左旋或右旋
template<class T> void RBTree<T>::leftrotate(RBTNode<T> *node) {
	RBTNode<T> *child = node->right;
	if (node == nullptr || child == nullptr) return;
	//先处理child->left和node的关系
	if(child->left != nullptr) child->left->parent = node;
	node->right = child->left;
	//再处理child和node->parent的关系,判断node是parent的左右孩子
	child->parent = node->parent;
	if (node->parent == nullptr) root = child;
	else if(node == node->parent->left) node->parent->left = child;
	else node->parent->right = child;
	//在处理child和 node的关系
	child->left = node;
	node->parent = child;
}
template<class T> void RBTree<T>::rightrotate(RBTNode<T> *node) {
	if (node == nullptr || node->left == nullptr) return;
	RBTNode<T>*child = node->left;
	// 先处理child->right和node的关系
	node->left = child->right;
	if (child->right != nullptr) child->right->parent = node;
	// 再处理child和node->parent的关系
	child->parent = node->parent;
	if (node->parent == nullptr) root = child;
	else if (node == node->parent->right) node->parent->right = child;
	else node->parent->left = child;
	// 最后处理node和child的关系
	node->parent = child;
	child->right = node;
}

// 颜色基操，红色为0，黑色为1
template<class T> COLOR RBTree<T>::getcolor(RBTNode<T>*node) {
	if (node == nullptr) return BLACK;
	return node->color;
}
// 插入基操
template<class T> void RBTree<T>::LLinsert(RBTNode<T>*node) {
	RBTNode<T> *p = node->parent;
	RBTNode<T> *g = p->parent;
	rightrotate(g);
	g->color = RED;
	p->color = BLACK;
}
template<class T> void RBTree<T>::RRinsert(RBTNode<T>*node) {
	RBTNode<T> *p = node->parent;
	RBTNode<T> *g = p->parent;
	leftrotate(g);
	g->color = RED;
	p->color = BLACK;
}

template<class T> RBTNode<T>* RBTree<T>::BSTinsert(T key) {
	RBTNode<T> * p = root;
	RBTNode<T> * q = nullptr; // p的父节点
	while (p)
	{
		q = p;
		if (key == p->key) return nullptr;
		else if (key > p->key) p = p->right;
		else p = p->left;
	}
	p = new RBTNode<T>(key);
	if (root == nullptr) {
		root = p;
		root->color = BLACK;
	}
	else if (key < q->key) {
		q->left = p;
		p->parent = q;
	}
	else {
		q->right = p;
		p->parent = q;
	}
	return p; // 返回新节点
}
template<class T> void RBTree<T>::fixColor(RBTNode<T>* node) {
	if (node == nullptr) return;
	// node是新插入的节点，需要对他们进行recolor
	RBTNode<T> *p = node->parent;
	if (node == nullptr || node == root || p->color == BLACK) return;
	//只剩下parent->color为RED,parent->parent->color为BLACK的情况
	RBTNode<T> *g = p->parent;
	RBTNode<T> *u = nullptr;
	if (p == g->right) u = g->left;
	else u = g->right;

	if (getcolor(u) == RED) {
		u->color = BLACK;
		p->color = BLACK;
		if (g != root) g->color = RED;
		fixColor(g);
	}
	else {
		if (g->left == p && p->left == node) {
			// LL情况
			LLinsert(node);
		}
		else if (g->right == p && p->right == node ) {
			// RR情况
			RRinsert(node);
		}
		else if(g->right ==p && p->left == node){
			// RL情况
			rightrotate(p);
			RRinsert(p);
		}
		else {
			//LR情况
			leftrotate(p);
			LLinsert(p);
		}
	}
}
template<class T> void RBTree<T>::insert(T key) {
	fixColor(BSTinsert(key));
}




template<class T>
void RBTree<T>::clear(RBTNode<T>* node) {
	if (node)
	{
		clear(node->left);
		clear(node->right);
		delete node;
		node = nullptr;
	}
}


template <class T>
void RBTree<T>::MidOrder() {
	if (root == nullptr) return;
	MidOrder(root);
}

template <class T>
void RBTree<T>::MidOrder(RBTNode<T>* node) {
	if (node==nullptr) return;
	MidOrder(node->left);
	std::cout << node->key << ":" << node->color << "   ,"<< "left:";
	if (node->left != nullptr) std::cout << node->left->key;
	std::cout << "," << "right:  ";
	if (node->right!=nullptr) std::cout  << node->right->key;
	std::cout << std::endl;
	MidOrder(node->right);
}
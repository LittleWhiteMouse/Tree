#pragma once
#include<iostream>
enum COLOR {RED,BLACK};
template<class T> class RBTree;

template<class T>
class RBTNode {
private:
	T key;
	COLOR color; // ��ɫΪ0����ɫΪ1,dbΪ2
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
	// ��������
	void MidOrder(RBTNode<T>* node);
	// ��ת����
	void leftrotate(RBTNode<T>*node);
	void rightrotate(RBTNode<T>*node);
	// ��ɫ����
	COLOR getcolor(RBTNode<T>*node);
	// �������
	void LLinsert(RBTNode<T>*node);
	void RRinsert(RBTNode<T>*node);
	RBTNode<T>* BSTinsert(T key);
	void fixColor(RBTNode<T>* node);
	// ɾ������
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

template<class T> void RBTree<T>::fixdb(RBTNode<T>*u) {	// ��u��db�����
	if (u == root) return;
	
	RBTNode<T> * p = u->parent;
	RBTNode<T> * s = ((u == p->left) ? p->right : p->left);
	if (getcolor(s) == RED) {
		s->color = BLACK;
		p->color = RED;
		if (s == p->right) leftrotate(p);
		else rightrotate(p);
		// ��������sΪ����������һ��Ҫˢ��sp,�ٽ��뵽sΪ�ڵ����
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
		else { //sΪ��,xy������һ��
			RBTNode<T>* far = getfar(u);
			if (getcolor(far) == RED)
				farred(u);
			else  farblack(u);
		}
	}
}
template<class T> void RBTree<T>::remove(T key) {
	RBTNode<T> * u = getnodeBST(root, key);  //�ҵ���ɾ�����
	if (u == nullptr) {
		std::cout << "��㲻����" << std::endl;
		return;
	}
	if (u == root) { delete root; root = nullptr; return; }
	else if(u->color == RED)
	{
		if (u == u->parent->left) u->parent->left = nullptr;
		else u->parent->right = nullptr;
	}
	else {
			// ��u��db������db��������
		fixdb(u);
		if (u == u->parent->left) u->parent->left = nullptr;
		else u->parent->right = nullptr;
	}
	delete u; 
	u = nullptr; 
	return;
}
// ���ش���ɾ������λ��
template<class T> RBTNode<T>* RBTree<T>::getnodeBST(RBTNode<T>*node, T key) {
	if (node == nullptr) return node;
	if (key < node->key) return getnodeBST(node->left, key);
	else if (key > node->key) return getnodeBST(node->right, key);
	else { // �ҵ���Ҫɾ���Ľڵ�
		
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
		else { //��ΪҶ�ӽڵ㣬��
			return node;
		}
	}
}
// ����node�����ǰ��
template<class T> RBTNode<T>* RBTree<T>::getPred(RBTNode<T>* node) {
	RBTNode<T>* maxleft = node->left;
	while (maxleft->right != nullptr) {
		maxleft = maxleft -> right;
	}
	return maxleft;
}
// ����node����С���
template<class T> RBTNode<T>* RBTree<T>::getSucc(RBTNode<T>* node) {
	RBTNode<T>* minright = node->right;
	while (minright->left != nullptr) {
		minright = minright->left;
	}
	return minright;
}

/********************************************************/

// ��x�ĺ���ΪԲ�ģ���x��������������
template<class T> void RBTree<T>::leftrotate(RBTNode<T> *node) {
	RBTNode<T> *child = node->right;
	if (node == nullptr || child == nullptr) return;
	//�ȴ���child->left��node�Ĺ�ϵ
	if(child->left != nullptr) child->left->parent = node;
	node->right = child->left;
	//�ٴ���child��node->parent�Ĺ�ϵ,�ж�node��parent�����Һ���
	child->parent = node->parent;
	if (node->parent == nullptr) root = child;
	else if(node == node->parent->left) node->parent->left = child;
	else node->parent->right = child;
	//�ڴ���child�� node�Ĺ�ϵ
	child->left = node;
	node->parent = child;
}
template<class T> void RBTree<T>::rightrotate(RBTNode<T> *node) {
	if (node == nullptr || node->left == nullptr) return;
	RBTNode<T>*child = node->left;
	// �ȴ���child->right��node�Ĺ�ϵ
	node->left = child->right;
	if (child->right != nullptr) child->right->parent = node;
	// �ٴ���child��node->parent�Ĺ�ϵ
	child->parent = node->parent;
	if (node->parent == nullptr) root = child;
	else if (node == node->parent->right) node->parent->right = child;
	else node->parent->left = child;
	// �����node��child�Ĺ�ϵ
	node->parent = child;
	child->right = node;
}

// ��ɫ���٣���ɫΪ0����ɫΪ1
template<class T> COLOR RBTree<T>::getcolor(RBTNode<T>*node) {
	if (node == nullptr) return BLACK;
	return node->color;
}
// �������
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
	RBTNode<T> * q = nullptr; // p�ĸ��ڵ�
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
	return p; // �����½ڵ�
}
template<class T> void RBTree<T>::fixColor(RBTNode<T>* node) {
	if (node == nullptr) return;
	// node���²���Ľڵ㣬��Ҫ�����ǽ���recolor
	RBTNode<T> *p = node->parent;
	if (node == nullptr || node == root || p->color == BLACK) return;
	//ֻʣ��parent->colorΪRED,parent->parent->colorΪBLACK�����
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
			// LL���
			LLinsert(node);
		}
		else if (g->right == p && p->right == node ) {
			// RR���
			RRinsert(node);
		}
		else if(g->right ==p && p->left == node){
			// RL���
			rightrotate(p);
			RRinsert(p);
		}
		else {
			//LR���
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
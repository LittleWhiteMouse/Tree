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
	int height();                 // ��ȡ���ĸ߶�
	AVLNode<T> * insert(T key);
	AVLNode<T> * remove(T key);

	// ɾ��AVL��
	void clear(AVLNode<T> *root);

	void preOrder();
	void midOrder();
	void postOrder();

private:
	AVLNode<T> *root;
	int height(AVLNode<T> *node); // ��ȡ���ĸ߶�
	int max(int a, int b);
	int getBalance(AVLNode<T> *node); // ��ȡƽ������

	// ��ƽ������ַ�ʽ
	AVLNode<T> *  LLrotation(AVLNode<T>* node);
	AVLNode<T> *  RRrotation(AVLNode<T>* node);
	AVLNode<T> *  LRrotation(AVLNode<T>* node);
	AVLNode<T> *  RLrotation(AVLNode<T>* node);

	// ������
	AVLNode<T> * insert(AVLNode<T> * root, T key);
	// ɾ���ڵ�
	AVLNode<T> * remove(AVLNode<T> *root, T key);

	void preOrder(AVLNode<T> *node);
	void midOrder(AVLNode<T> *node);
	void postOrder(AVLNode<T> *node);
};

/**********************��ȡAVL���ĸ߶�*******************/
template<class T> int AVLTree<T>::height(AVLNode<T> *node){
	if (node == nullptr) return 0;
	return node->height;
}
template<class T> int AVLTree<T>::height() {
	return height(root);
}
/**********************��ȡAVL����ƽ������*******************/
template<class T> int AVLTree<T>::getBalance(AVLNode<T>*node) {
	if (node == nullptr) return 0;
	return height(node->left) - height(node->right);
}
/**********************���½��Ƚϴ�С*******************/
template<class T> int AVLTree<T>::max(int a, int b) {
	return (a > b) ? a : b;
}

/************AVL��ʧ���������̬��Ӧ����ƽ��ķ�ʽ***********/
template<class T> AVLNode<T> * AVLTree<T>::LLrotation(AVLNode<T>* g) {
	// �����游��㣬���游�����ȥ
	// g:�游���(ʧ����) p:����� 
	AVLNode<T> *p = g->left;
	g->left = p->right;
	p->right = g;
	// ���½��߶�
	g->height = 1+max(height(g->left), height(g->right));
	p->height = 1+max(height(p->left), height(p->right));

	return p;
}
template<class T> AVLNode<T> * AVLTree<T>::RRrotation(AVLNode<T> *g) {
	// �����游��㣬���游�����ȥ
	// g:�游���(ʧ����) p:����� 
	AVLNode<T> *p = g->right;
	g->right = p->left;
	p->left = g;
	// ���½��߶�
	g->height = 1 + max(height(g->left), height(g->right));
	p->height = 1 + max(height(p->left), height(p->right));

	return p;
}
template<class T> AVLNode<T> * AVLTree<T>::LRrotation(AVLNode<T> *g) {
	// ����p + ����g = RR + LL
	// g:�游���(ʧ����) p:����� 
	g->left = RRrotation(g->left);
	return LLrotation(g);
}
template<class T> AVLNode<T> * AVLTree<T>::RLrotation(AVLNode<T> *g) {
	// ����p + ����g = LL + RR
	// g:�游���(ʧ����) p:����� 
	g->right = LLrotation(g->right);
	return RRrotation(g);;
}

/**********************AVL ������*******************/
template<class T> AVLNode<T> * AVLTree<T>::insert(AVLNode<T> * node, T key) {
	// 1. ִ�б�׼BST�������
	if (node == nullptr)
		return new AVLNode<T>(key, nullptr, nullptr);
	else if (key < node->key)
		node->left = insert(node->left, key);
	else if (key > node->key)
		node->right = insert(node->right, key);
	else
		return node;// ����������ظ����
	
	// 2. ���¸����Ƚڵ�ĸ߶�
	node->height = max(height(node->left), height(node->right)) + 1;

	// 3. ��ȡƽ�����ӣ����ж��Ƿ�ʧ��
	int balance = getBalance(node);
	// 4. ��ʧ�⣬���ж�����ʧ�����
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

/**********************AVL ɾ����*******************/
template<class T> void AVLTree<T>::clear(AVLNode<T> *node) {
	if (node != nullptr) {
		clear(node->left);
		clear(node->right);
		delete node;
		node = nullptr;
	}
}

/**********************AVL ���ɾ��*******************/
template <class T> AVLNode<T> * AVLTree<T>::remove(T key) {
	root = remove(root, key);
	return root;
}

// ɾ����� �� ���ر����������ĸ����
template<class T> AVLNode<T> * AVLTree<T>::remove(AVLNode<T> *node, T key) {
	// 1. ִ�б�׼BSTɾ��
	if (node == nullptr) {
		std::cout << "��㲻���ڣ��޷�ɾ��\n";
		return nullptr;
	}
	if (key < node->key){
		node->left = remove(node->left, key);
	}
	else if (key > node->key) {
		node->right = remove(node->right, key);
	}
	else{
		// �ҵ���Ҫ��ɾ���Ľڵ㣬��ʼ��������
		// ��ɾ�����Ķ�Ϊ2
		if (node->left != nullptr && node->right != nullptr) {
			// �ҵ���ɾ����������������Сֵ���滻��ɾ�����
			AVLNode<T> * minright = node->right;
			while (minright->left){
				minright = minright->left;
			}
			node->key = minright->key;  // �滻��ɾ�����
			node->right = remove(node->right, minright->key); // ɾ�����滻�Ľ��
		}
		else // ����ɾ�����Ķ�Ϊ0��1,ɾ����ɾ�����
		{   
			// ����node���ó�Ϊ�ǿ��ӽڵ㣨��Ϊ1��
			// ������Ϊ��ָ�루��Ϊ0��
			AVLNode<T> * temp = node;
			node = node->left ? node->left : node->right;
			delete temp;
		}
	}

	// 2. ���µ�ǰ�ڵ�ĸ߶�
	if (node == nullptr) return nullptr; //���ڿսڵ��޷�Ҳ������¸߶�
	node->height = 1 + max(height(node->left), height(node->right));

	// 3. ��ȡ��ǰ�ڵ��ƽ�����ӣ���֤�Ƿ�ʧ��
	int balance = getBalance(node);
	// 4. �ָ�ƽ����������
	if (balance > 1) {  //���ʧ����������Է�ΪLR,LL
		if (getBalance(node->left) >= 0) return LLrotation(node);
		else return LRrotation(node);
	}
	if (balance < -1) { // �Ҳ�ʧ����������Է�ΪRL,RR
		if (getBalance(node->right) > 0) return RLrotation(node);
		else return RRrotation(node);
	}
	return node;
}

/**********************���ĸ��ֱ���*************************************/
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
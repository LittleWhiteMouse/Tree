#pragma once
#include<iostream>
enum Boolean { FALSE, TRUE }; //�Զ��岼����

template<class T>
class Element {  //������Ӹ�������ݣ�������
public:
	T key;
};

template <class T> class BSTree; //����BSTree����������ֹ��������Ԫʱ����

template<class T>
class BSNode {
	friend class BSTree<T>; //��BSTree��Ϊ��Ԫʹ֮���Է���˽�г�Ա
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
	std::cout << "Position: " << i; // i�ǽ���λ�ã����ղ������������
	std::cout << "   DataKey: " << data.key << std::endl;  // ��ʾ����
	if (left) left->display(2 * i);
	if (right) right->display(2 * i + 1);
}

template<class T>
Boolean BSTree<T>::Insert(const Element<T> &data) {
	BSNode<T> *p = root;       // ��ǰ�ڵ�
	BSNode<T> *q = nullptr;    // ����ָ��ǰ�ڵ�ĸ��ڵ�
	//insert֮ǰ��Ҫ�Ȳ���
	while (p) {
		q = p; //ÿ�θı�p֮ǰ����q��¼
		if (data.key == p->data.key) return FALSE;//����ʧ��
		else if (data.key < p->data.key) { p = p->left; }//����
		else p = p->right; //����
	}
	//��ѭ���������ҵ���λ��Ϊq
	p = new BSNode<T>;
	p->left = p->right = nullptr;
	p->data = data;
	if (!root) root = p;
	else if (data.key < q->data.key) q->left = p;
	else q->right = p;
	return TRUE;//��ʾ����ɹ�
}

template <class T>
BSNode<T> * BSTree<T>::remove(Element<T> &data) {
	root = remove(root, data);
	return root;
}

// ɾ����� �� ���ر�ɾ���Ľڵ�
template<class T>
BSNode<T> * BSTree<T>::remove(BSNode<T> *node, Element<T> &data) {
	if (node == nullptr) return node;
	if (data.key < node->data.key)
		node->left = remove(node->left, data);
	else if (data.key > node->data.key)
		node->right = remove(node->right, data);
	else
	{ //�ҵ��˴�ɾ�����
		if (node->left != nullptr &&  node->right != nullptr) //����ɾ������Ϊ2
		{
			BSNode<T> * maxleft = node->left; //ȥ�������������ֵ�滻��ɾ�����
			while (maxleft->right != nullptr)
			{
				maxleft = maxleft->right;
			}
			node->data = maxleft->data; // �滻
			node->left = remove(node->left, data); // ɾ�������
		}
		else // ��ɾ�����Ķ���0����1
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

// �ݹ�Ĳ���
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

//�����Ĳ���
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

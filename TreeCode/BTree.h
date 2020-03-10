#pragma once
#include<iostream>
// �涨B������С��Ϊm
template <class T,int m = 3>
class BNode {
public:
	int n;       // ��ǰ�洢��key����
	T keys[2*m-1];  // ��keys�����ָ��
	BNode<T>* childnodes[2 * m]; // ��childpointers
	bool isleaf;    // �����ΪҶ�ӽڵ���Ϊ�棬����Ϊ��

	BNode(int _n=0,bool _isleaf=true):n(_n),isleaf(_isleaf){
		for (int i = 0; i < 2*m; i++)
		{
			childnodes[i] = nullptr;
		}
	}	
};

template <class T,int m=3>
class BTree
{
public:
	BNode<T,m> * root;
	void traverse(BNode<T,m>* node); // ����
public:
	BTree():root(nullptr) {}
	BNode<T, m>* search(BNode<T, m>* node, T key);
	// ���뺯��
	void splitchild(BNode<T, m>* parentnode, int index);
	void insert(T key);
	void insert_nonfull(BNode<T, m>*node, T key);

	void traverse(); // ����

	// ɾ������
	void remove(const T& key);
	void remove(BNode<T,m>*node, const T & key);
	int findKey(BNode<T, m>*node, const T & key);
	bool leakfill(BNode<T, m>*node, int id);
	void removeFromLeaf(BNode<T, m>*node,int id);
	void removeFromNonLeaf(BNode<T, m>*node, int id);
	BNode<T, m>* getPred(BNode<T,m>*node,int id); // ���ǰ��
	BNode<T, m>* getSucc(BNode<T, m>*node, int id); // ��ú��
	void borrowFromPrev(BNode<T, m>*node, int id);
	void borrowFromNext(BNode<T, m>*node, int id);
	void merge(BNode<T, m>*node, int id);
};



/*****************************B��������*********************************/
template<class T,int m>
BNode<T, m>* BTree<T,m>::search(BNode<T, m>* node, T key) {
	if (node == nullptr) return nullptr;
	// ȥ��key�ڵ�ǰ�ڵ��е��ĸ���Χ����ÿ���ڲ������n+1·�ķ�֧ѡ��
	int i = 0;  // ���±��0��ʼ
	while (i < node->n && key > node->keys[i]) { i++; } 

	//���������±�����
	if ( key == node->keys[i]) return node; // 1. �ҵ��ˣ����ص�ַ
	if (node->isleaf) return nullptr;       // 2. δ�ҵ�������Ҷ�ӣ�����û�ҵ������ؿ�
	return search(node->childnodes[i], key);// 3. δ�ҵ����ҷ�Ҷ������������
}

/*****************************B���Ĳ���*********************************/
template <class T,int m> 
void BTree<T, m>::splitchild(BNode<T, m>*node, int i) {
	// ���ܣ� ����node�����ӽڵ㣬�������м�key��node
	// node:δ���Ľڵ� i: node�����ӽڵ�L���±�
	// m����С��
	// L : node���±�Ϊi��ȫ��childnodes ȫ��Ҳ����n = 2*t-1,�±��0~2*t-2
	// L�й���2*t-1��keys�������t-1��keys��L�����ұ�t-1��keys��R���м�key��node
	// R : L���ѳ����½��,n������Ϊt-1���±��0~t-2
	BNode<T,m> * R = new BNode<T>();  
	BNode<T,m> * L = node->childnodes[i]; // ԭ�������ӽڵ�
	

	// 1. ��L�п���keys��nodes����R
	R->isleaf = L->isleaf;
	R->n = m - 1; 
	for (int j = 0; j < m-1; j++) 
		R->keys[j] = L->keys[m + j];
	if (L->isleaf==false){
		for (int j = 0; j < m; j++)
			R->childnodes[j] = L->childnodes[j + m];
	}
	L->n = m - 1; // �޸�L�е�keys����
	
	// 2.���м�������Ϊ���ڵ�
	for (int j = node->n ; j > i; j--)	{
		// ���±�Ϊx->n��β��㿪ʼ��i֮ǰ������ָ�붼������
		//i֮�������ָ�붼�����ƶ����ճ�i+1��ָ��λ�������µĽ��
		node->childnodes[j+1] = node->childnodes[j];
	}
	node->childnodes[i + 1] = R;

	for (int j = node->n;j>i;j--) { 
		//keyŲ���Ĵ�����childnodes�ƶ�������ͬ
		node->keys[j] = node->keys[j-1];
	}
	node->keys[i] = L->keys[m - 1]; //��L���м�ֵkey�ŵ����ڵ���Ӧ��λ��

	node->n++; // ����node��keys����
}

template<class T, int m>
void BTree<T, m>::insert_nonfull(BNode<T,m>*node,T key) {
	int i = node->n - 1; 
	if (node->isleaf)
	{  // ���뵽δ����Ҷ�ӽڵ㣬ֱ����λ�ò��뼴��
		while (i>=0 && key < node->keys[i])
		{
			node->keys[i+1] = node->keys[i];
			i--;
		}
		node->keys[i+1] = key;
		node->n++;
	}
	else
	{
		// ���뵽δ���ķ�Ҷ��㣬��Ҫ���뵽childnodes�ڲ����в���
		//�ҵ�key����keys[i]��keys[i+1]֮���childnodes[i+1]��
		while (i >= 0 && key < node->keys[i]) --i;
		if (node->childnodes[i+1]->n == 2*m-1)
		{	// ���������,node����keys[i+1]λ�ñ�����һ���µ�ֵ
			splitchild(node, i + 1);
			// ������ֵ�Ĵ�С���ж�key����split������node[i+1]�����ұ�node[i+2]
			if (node->keys[i + 1] < key) i++;
		}
		insert_nonfull(node->childnodes[i+1], key);
		// ���뵽�ӽڵ��ڲ��������룬ʵ���ǵݹ鵽��Ҷ�ӽڵ�
	}
}

template <class T,int m>
void BTree<T, m>::insert(T key) {
	// �����Ĳ������
	if (root == nullptr) {
		root = new BNode<T, m>();
		root->keys[0] = key;
		root->n = 1;
	}
	// �����ǿ�
	else {
		if (root->n == 2*m-1) // ���������Ա����Ҫ�ֶ���������ĸ��ڵ�������splitchild
		{	BNode<T, m> *oldroot = root;
			BNode<T, m> *newroot = new BNode<T, m>();
			root = newroot;
			newroot->childnodes[0] = oldroot; 
			// 0����Ϊ�����ĸ��ڵ��ǿյ�
			splitchild(newroot, 0); //����oldroot,֮��newroot�ϻ��һ����
			root->isleaf = false;
			insert_nonfull(newroot, key); // ���¶Ը����������������в���
		}
		else insert_nonfull(root, key); // ȥ��Ҷ�ӽ��
	}
}

/*****************************B���ı���*********************************/
template<class T, int m> void BTree<T, m>::traverse() {
	traverse(root);
}

template<class T,int m>
void  BTree<T, m>::traverse(BNode<T,m>* node) {
	if (node == nullptr) return;
	int i = 0;
	for (; i < node->n; i++)
	{
		if (node->isleaf==false) // ����Ҷ�ӽڵ㣬���ڴ�ӡ֮ǰ��Ҫ�ȴ�ӡ��ǰkey��ߵ�childnodes
		{
			traverse(node->childnodes[i]);
		}
		std::cout << " " << node->keys[i]; //�ٴ�ӡi���ڵ�key
	}
	// �������ұߵ�childnodes���е�����ӡ
	if (node->childnodes[i] != nullptr) traverse(node->childnodes[i]);
}


/*****************************B����ɾ��*********************************/
template<class T, int m>
void BTree<T, m>::remove(const T&key) {
	remove(root, key);
}
// �ҵ� node->keys[i]>=key��i
template<class T, int m>
int BTree<T, m>::findKey(BNode<T, m>*node, const T &key) {
	int id = 0;
	while (id<node->n && key>node->keys[id]) id++;
	return id;
}

template<class T, int m>
void BTree<T, m>::remove(BNode<T, m>*node, const T &key) {
	if (node == nullptr) return;
	int id = findKey(node, key);
	if (id < node->n && node->keys[id] == key)
	{   // �����ҵ��������
		if (node->isleaf)
			removeFromLeaf(node, id);
		else
			removeFromNonLeaf(node, id);
	}
	else // ���ڻ�δ�ҵ��������
	{
		if (node->isleaf) { //��ΪҶ�������key��������
			std::cout << key << " does not exist\n";
			return;
		}
		// ����ȥ�Ľڵ���п��ܻ�ʧ�⣬����Ҫ����������һ����
		// ����Ҫע���������֮��id�ᷢ���仯�������Ҫ�Կ��ܳ��ֵ��������������
		// key��idҪ�����仯��һ������Ϊnode���±귢���˱仯��
		// ��leakfill��borrow����Ӱ��node���±�仯��ֻ��merge���ܻ�ı��±�
		// �����ұ߽��ϲ���ʱ��id���ᷢ���仯
		// ֻ�����ұߵĽڵ㣬�ں���߽�㷢���ϲ�����node�е��±�id���1

		bool flag = false; // Ĭ�ϲ������ϲ������
		if (node->childnodes[id]->n < m)
			flag = leakfill(node, id);//��������˺ϲ�
		if (flag)
			remove(node->childnodes[id - 1], key);
		else
			remove(node->childnodes[id], key);
	}
}

// ����øú���,����ֹ�������
// �����棬˵�������ҽ��������ֵܷ����˺ϲ�������id��-1
template<class T, int m>
bool BTree<T, m>::leakfill(BNode<T, m>*node, int id) {
	// ��һ�֣������ֵܽ���
	if (id != 0 && node->childnodes[id - 1]->n >= m)
		borrowFromPrev(node, id);
	// �ڶ��֣������ֵܽ���
	else if (id != node->n && node->childnodes[id + 1]->n >= m)
		borrowFromNext(node, id);
	// �����֣���childnodes[id]���ֵܽ��ϲ�
	else
	{
		if (id < node->n)
			merge(node, id); //�������ҽ�㣬����ҽڵ�ϲ�
		else {
			merge(node, id - 1); // �������ڵ�ϲ�
			return true;
		}
	}
	return false;
}

template<class T, int m>
void BTree<T, m>::removeFromLeaf(BNode<T, m>*node, int id) {
	for (int i = id + 1; i < node->n; i++)
		node->keys[i - 1] = node->keys[i];
	node->n--;
}

template<class T, int m>
void BTree<T, m>::removeFromNonLeaf(BNode<T, m>*node, int id) {
	// ��ǰΪnode->keys[id],��ǰ��Ϊnode->childnodes[id],���Ϊnode->childnodes[id+1]
	// �ж�ǰ���Ƿ�����n>t-1,�Ǿ��滻��Ȼ��ɾ��
	T key = node->keys[id];
	if (node->childnodes[id]->n > m - 1)
	{
		BNode<T, m> *pred = getPred(node, id);
		node->keys[id] = pred->keys[pred->n - 1];
		remove(pred, pred->keys[pred->n - 1]);
	}// ���������n>t-1,�Ǿ��滻Ȼ��ɾ��
	else if (node->childnodes[id + 1]->n > m - 1)
	{
		BNode<T, m> *succ = getSucc(node, id);
		node->keys[id] = succ->keys[0];
		remove(succ, succ->keys[0]);
	}
	else //��ǰ���ͺ�̶�����n = t-1,��ϲ�ǰ����̺͸�key
	{
		merge(node, id);
		remove(node->childnodes[id], key);
	}
}

// ���node->keys[id]��ǰ�����ֵ���ڵĽڵ�
template<class T, int m>
BNode<T, m>* BTree<T, m>::getPred(BNode<T, m>*node, int id) {
	BNode<T, m> *cur = node->childnodes[id];  // curΪnode->keys[id]��ǰ������
	//���������ҵ�Ҷ�ӽڵ��е����ֵ
	while (!cur->isleaf) {
		cur = cur->childnodes[cur->n];
	}
	return cur;
}
// ��ȡnode->keys[id]�ĺ����Сֵ���ڵĽڵ�
template<class T, int m>
BNode<T, m>* BTree<T, m>::getSucc(BNode<T, m>*node, int id) {
	BNode<T, m> *cur = node->childnodes[id + 1]; // curΪnode->keys[id]�ĺ�̺���
	// ���������ҵ�Ҷ�ӽڵ��е���Сֵ
	while (!cur->isleaf)
	{
		cur = cur->childnodes[0];
	}
	return cur;
}


/****node->childnodes[ id ]->n  < t-1  ���ֵܽ��裬���ߺ����Ǻϲ�******/
template<class T, int m>
void BTree<T, m>::borrowFromPrev(BNode<T, m>*node, int id) {
	// node->childnodes[ id ]->n  < t-1 
	// node->childnodes[id-1]->n  > t-1
	// [id]��node��һ����node��[id-1]��һ��
	BNode<T, m> *child_i = node->childnodes[id];
	BNode<T, m> *sibling = node->childnodes[id - 1];

	//��λ�ø�node��������key
	for (int i = child_i->n; i > 0; i--) {
		child_i->keys[i] = child_i->keys[i - 1];
	}
	//��Ҷ�ڵ㣬child��ָ��Ҳ��Ҫ��֮�ƶ�
	if (!child_i->isleaf) {
		for (int i = node->childnodes[id]->n; i >= 0; i--) {

			child_i->childnodes[i + 1] = child_i->childnodes[i];
		}
	}
	child_i->keys[0] = node->keys[id - 1];

	//��sibling�����ұ�childnodesΪchild_i�ĵ�һ��childnodes
	if (!child_i->isleaf) {
		child_i->childnodes[0] = sibling->childnodes[sibling->n];
	}

	node->keys[id - 1] = sibling->keys[sibling->n - 1];

	child_i->n++;
	sibling->n--;
}
template<class T, int m>
void BTree<T, m>::borrowFromNext(BNode<T, m>*node, int id) {
	// node->childnodes[ id ]->n  < t-1 
	// node->childnodes[id+1]->n  > t-1
	// [id]��node��һ����node��[id+1]��һ��
	BNode<T, m>*child_i = node->childnodes[id];
	BNode<T, m>*sibling = node->childnodes[id + 1];

	child_i->keys[child_i->n] = node->keys[id];
	if (!child_i->isleaf)
	{
		child_i->childnodes[child_i->n + 1] = sibling->childnodes[0];
	}
	node->keys[id] = sibling->keys[0];
	for (int i = 0; i < sibling->n - 1; i++)
	{
		sibling->keys[i] = sibling->keys[i + 1];
	}
	if (!sibling->isleaf)
	{
		for (int i = 0; i <= sibling->n - 1; i++)
		{
			sibling->childnodes[i] = sibling->childnodes[i + 1];
		}
	}
	child_i->n++;
	sibling->n--;
}

template<class T, int m>
void BTree<T, m>::merge(BNode<T, m>*node, int id) {
	// node->childnodes[id] n = t-1
	// node->childnodes[id+1] n = t-1
	//�ϲ�node->key[id],node->childnodes[id]��node->childnodes[id+1]
	BNode<T, m>* child = node->childnodes[id];
	BNode<T, m>* sibling = node->childnodes[id + 1];

	// �ϲ�child\sibling\node��key��child��
	child->keys[m - 1] = node->keys[id];
	for (int i = 0; i < sibling->n; i++)
	{
		child->keys[m + i] = sibling->keys[i];
	}
	//���ڷǺ��ӽڵ㣬����Ҫ�ϲ����ӽ��
	if (!child->isleaf) {
		for (int i = 0; i < sibling->n; i++) {
			//�ϲ����ߵ�nodes
			child->childnodes[m + i] = sibling->childnodes[i];
		}
	}
	// ��node->keys[id]֮���λ����ǰ��
	for (int i = id; i < node->n - 1; i++)
	{
		node->keys[i] = node->keys[i + 1];
		node->childnodes[i + 1] = node->childnodes[i + 2];
	}

	child->n += sibling->n + 1;
	node->n--;
	delete sibling;
	sibling = nullptr;
}
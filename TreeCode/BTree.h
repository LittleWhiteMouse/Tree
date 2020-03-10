#pragma once
#include<iostream>
// 规定B树的最小度为m
template <class T,int m = 3>
class BNode {
public:
	int n;       // 当前存储的key数量
	T keys[2*m-1];  // 存keys数组的指针
	BNode<T>* childnodes[2 * m]; // 存childpointers
	bool isleaf;    // 若结点为叶子节点则为真，否则为假

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
	void traverse(BNode<T,m>* node); // 遍历
public:
	BTree():root(nullptr) {}
	BNode<T, m>* search(BNode<T, m>* node, T key);
	// 插入函数
	void splitchild(BNode<T, m>* parentnode, int index);
	void insert(T key);
	void insert_nonfull(BNode<T, m>*node, T key);

	void traverse(); // 遍历

	// 删除函数
	void remove(const T& key);
	void remove(BNode<T,m>*node, const T & key);
	int findKey(BNode<T, m>*node, const T & key);
	bool leakfill(BNode<T, m>*node, int id);
	void removeFromLeaf(BNode<T, m>*node,int id);
	void removeFromNonLeaf(BNode<T, m>*node, int id);
	BNode<T, m>* getPred(BNode<T,m>*node,int id); // 获得前驱
	BNode<T, m>* getSucc(BNode<T, m>*node, int id); // 获得后继
	void borrowFromPrev(BNode<T, m>*node, int id);
	void borrowFromNext(BNode<T, m>*node, int id);
	void merge(BNode<T, m>*node, int id);
};



/*****************************B树的搜索*********************************/
template<class T,int m>
BNode<T, m>* BTree<T,m>::search(BNode<T, m>* node, T key) {
	if (node == nullptr) return nullptr;
	// 去找key在当前节点中的哪个范围，对每个内部结点做n+1路的分支选择
	int i = 0;  // 令下标从0开始
	while (i < node->n && key > node->keys[i]) { i++; } 

	//分类讨论下标的情况
	if ( key == node->keys[i]) return node; // 1. 找到了，返回地址
	if (node->isleaf) return nullptr;       // 2. 未找到，且是叶子，彻底没找到，返回空
	return search(node->childnodes[i], key);// 3. 未找到，且非叶，继续往下找
}

/*****************************B树的插入*********************************/
template <class T,int m> 
void BTree<T, m>::splitchild(BNode<T, m>*node, int i) {
	// 功能： 分裂node的满子节点，并调整中间key到node
	// node:未满的节点 i: node的满子节点L的下标
	// m：最小度
	// L : node中下标为i的全满childnodes 全满也就是n = 2*t-1,下标从0~2*t-2
	// L中共有2*t-1个keys，留左边t-1个keys给L，出右边t-1个keys给R，中间key给node
	// R : L分裂出的新结点,n将被设为t-1，下标从0~t-2
	BNode<T,m> * R = new BNode<T>();  
	BNode<T,m> * L = node->childnodes[i]; // 原来的满子节点
	

	// 1. 从L中拷贝keys和nodes构造R
	R->isleaf = L->isleaf;
	R->n = m - 1; 
	for (int j = 0; j < m-1; j++) 
		R->keys[j] = L->keys[m + j];
	if (L->isleaf==false){
		for (int j = 0; j < m; j++)
			R->childnodes[j] = L->childnodes[j + m];
	}
	L->n = m - 1; // 修改L中的keys数量
	
	// 2.将中间结点提升为父节点
	for (int j = node->n ; j > i; j--)	{
		// 从下标为x->n的尾结点开始，i之前的所有指针都不动，
		//i之后的所有指针都往后移动，空出i+1的指针位置链接新的结点
		node->childnodes[j+1] = node->childnodes[j];
	}
	node->childnodes[i + 1] = R;

	for (int j = node->n;j>i;j--) { 
		//key挪动的次数和childnodes移动次数相同
		node->keys[j] = node->keys[j-1];
	}
	node->keys[i] = L->keys[m - 1]; //将L的中间值key放到父节点相应空位上

	node->n++; // 更新node内keys数量
}

template<class T, int m>
void BTree<T, m>::insert_nonfull(BNode<T,m>*node,T key) {
	int i = node->n - 1; 
	if (node->isleaf)
	{  // 插入到未满的叶子节点，直接腾位置插入即可
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
		// 插入到未满的非叶结点，需要进入到childnodes内部进行插入
		//找到key处在keys[i]和keys[i+1]之间的childnodes[i+1]中
		while (i >= 0 && key < node->keys[i]) --i;
		if (node->childnodes[i+1]->n == 2*m-1)
		{	// 若满则分裂,node会在keys[i+1]位置被插入一个新的值
			splitchild(node, i + 1);
			// 根据新值的大小来判断key处在split后的左边node[i+1]还是右边node[i+2]
			if (node->keys[i + 1] < key) i++;
		}
		insert_nonfull(node->childnodes[i+1], key);
		// 进入到子节点内部继续插入，实质是递归到了叶子节点
	}
}

template <class T,int m>
void BTree<T, m>::insert(T key) {
	// 空树的插入情况
	if (root == nullptr) {
		root = new BNode<T, m>();
		root->keys[0] = key;
		root->n = 1;
	}
	// 若树非空
	else {
		if (root->n == 2*m-1) // 若根结点满员，需要手动创造根结点的父节点来调用splitchild
		{	BNode<T, m> *oldroot = root;
			BNode<T, m> *newroot = new BNode<T, m>();
			root = newroot;
			newroot->childnodes[0] = oldroot; 
			// 0是因为根结点的父节点是空的
			splitchild(newroot, 0); //分裂oldroot,之后newroot上会多一个数
			root->isleaf = false;
			insert_nonfull(newroot, key); // 重新对根结点非满的新树进行插入
		}
		else insert_nonfull(root, key); // 去找叶子结点
	}
}

/*****************************B树的遍历*********************************/
template<class T, int m> void BTree<T, m>::traverse() {
	traverse(root);
}

template<class T,int m>
void  BTree<T, m>::traverse(BNode<T,m>* node) {
	if (node == nullptr) return;
	int i = 0;
	for (; i < node->n; i++)
	{
		if (node->isleaf==false) // 若非叶子节点，则在打印之前需要先打印当前key左边的childnodes
		{
			traverse(node->childnodes[i]);
		}
		std::cout << " " << node->keys[i]; //再打印i所在的key
	}
	// 对于最右边的childnodes进行单独打印
	if (node->childnodes[i] != nullptr) traverse(node->childnodes[i]);
}


/*****************************B树的删除*********************************/
template<class T, int m>
void BTree<T, m>::remove(const T&key) {
	remove(root, key);
}
// 找到 node->keys[i]>=key的i
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
	{   // 对于找到结点的情况
		if (node->isleaf)
			removeFromLeaf(node, id);
		else
			removeFromNonLeaf(node, id);
	}
	else // 对于还未找到结点的情况
	{
		if (node->isleaf) { //若为叶子则这个key不在树中
			std::cout << key << " does not exist\n";
			return;
		}
		// 走下去的节点很有可能会失衡，所以要先让它膨胀一波，
		// 但是要注意的是膨胀之后，id会发生变化，因此需要对可能出现的膨胀情况做分析
		// key的id要发生变化，一定是因为node中下标发生了变化，
		// 在leakfill中borrow不会影响node的下标变化，只有merge可能会改变下标
		// 结点和右边结点合并的时候，id不会发生变化
		// 只有最右边的节点，在和左边结点发生合并后，在node中的下标id会减1

		bool flag = false; // 默认不发生合并的情况
		if (node->childnodes[id]->n < m)
			flag = leakfill(node, id);//如果发生了合并
		if (flag)
			remove(node->childnodes[id - 1], key);
		else
			remove(node->childnodes[id], key);
	}
}

// 则调用该函数,来防止结点下溢
// 返回真，说明是最右结点和其左兄弟发生了合并，检索id会-1
template<class T, int m>
bool BTree<T, m>::leakfill(BNode<T, m>*node, int id) {
	// 第一种：找左兄弟结点借
	if (id != 0 && node->childnodes[id - 1]->n >= m)
		borrowFromPrev(node, id);
	// 第二种：找右兄弟结点借
	else if (id != node->n && node->childnodes[id + 1]->n >= m)
		borrowFromNext(node, id);
	// 第三种，该childnodes[id]和兄弟结点合并
	else
	{
		if (id < node->n)
			merge(node, id); //若非最右结点，则和右节点合并
		else {
			merge(node, id - 1); // 否则和左节点合并
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
	// 当前为node->keys[id],则前驱为node->childnodes[id],后继为node->childnodes[id+1]
	// 判断前驱是否满足n>t-1,是就替换换然后删除
	T key = node->keys[id];
	if (node->childnodes[id]->n > m - 1)
	{
		BNode<T, m> *pred = getPred(node, id);
		node->keys[id] = pred->keys[pred->n - 1];
		remove(pred, pred->keys[pred->n - 1]);
	}// 若后继满足n>t-1,是就替换然后删除
	else if (node->childnodes[id + 1]->n > m - 1)
	{
		BNode<T, m> *succ = getSucc(node, id);
		node->keys[id] = succ->keys[0];
		remove(succ, succ->keys[0]);
	}
	else //若前驱和后继都满足n = t-1,则合并前驱后继和父key
	{
		merge(node, id);
		remove(node->childnodes[id], key);
	}
}

// 获得node->keys[id]的前驱最大值所在的节点
template<class T, int m>
BNode<T, m>* BTree<T, m>::getPred(BNode<T, m>*node, int id) {
	BNode<T, m> *cur = node->childnodes[id];  // cur为node->keys[id]的前驱孩子
	//不断往右找到叶子节点中的最大值
	while (!cur->isleaf) {
		cur = cur->childnodes[cur->n];
	}
	return cur;
}
// 获取node->keys[id]的后继最小值所在的节点
template<class T, int m>
BNode<T, m>* BTree<T, m>::getSucc(BNode<T, m>*node, int id) {
	BNode<T, m> *cur = node->childnodes[id + 1]; // cur为node->keys[id]的后继孩子
	// 不断往左找到叶子节点中的最小值
	while (!cur->isleaf)
	{
		cur = cur->childnodes[0];
	}
	return cur;
}


/****node->childnodes[ id ]->n  < t-1  向兄弟结点借，或者和他们合并******/
template<class T, int m>
void BTree<T, m>::borrowFromPrev(BNode<T, m>*node, int id) {
	// node->childnodes[ id ]->n  < t-1 
	// node->childnodes[id-1]->n  > t-1
	// [id]向node拿一个，node向[id-1]拿一个
	BNode<T, m> *child_i = node->childnodes[id];
	BNode<T, m> *sibling = node->childnodes[id - 1];

	//腾位置给node中拿来的key
	for (int i = child_i->n; i > 0; i--) {
		child_i->keys[i] = child_i->keys[i - 1];
	}
	//非叶节点，child的指针也需要随之移动
	if (!child_i->isleaf) {
		for (int i = node->childnodes[id]->n; i >= 0; i--) {

			child_i->childnodes[i + 1] = child_i->childnodes[i];
		}
	}
	child_i->keys[0] = node->keys[id - 1];

	//设sibling的最右边childnodes为child_i的第一个childnodes
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
	// [id]向node拿一个，node向[id+1]拿一个
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
	//合并node->key[id],node->childnodes[id]和node->childnodes[id+1]
	BNode<T, m>* child = node->childnodes[id];
	BNode<T, m>* sibling = node->childnodes[id + 1];

	// 合并child\sibling\node的key到child中
	child->keys[m - 1] = node->keys[id];
	for (int i = 0; i < sibling->n; i++)
	{
		child->keys[m + i] = sibling->keys[i];
	}
	//对于非孩子节点，还需要合并孩子结点
	if (!child->isleaf) {
		for (int i = 0; i < sibling->n; i++) {
			//合并三者的nodes
			child->childnodes[m + i] = sibling->childnodes[i];
		}
	}
	// 把node->keys[id]之后的位置往前移
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
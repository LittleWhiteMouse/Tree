#include<iostream>
#include<time.h>
#include<vector>
#include "AVLTree.h"
using namespace std;

int main(int argc, char**argv) {
	srand((int)time(0));
	AVLTree<int> tree;
	int times = rand()%20; //随机生成树的节点数量
	int copytimes = times;
	vector<int> arr;
	while (copytimes--)
	{
		int val = rand() % 100;
		arr.push_back(val);
		tree.insert(val); // 随机插入任意节点的大小，范围定在0~99之间
	}
	tree.midOrder();
	cout << "**************" << endl;
	while (times--)
	{
		int todel = rand() % arr.size();
		cout << "\n try to del " << arr[todel] << endl;
		tree.remove(arr[todel]);
		cout << " the result is ";
		tree.midOrder();
	}
	cout << "\n***********" << endl;
	system("pause");
	return 0;
} 
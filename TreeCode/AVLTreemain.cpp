#include<iostream>
#include<time.h>
#include<vector>
#include "AVLTree.h"
using namespace std;

int main(int argc, char**argv) {
	srand((int)time(0));
	AVLTree<int> tree;
	int times = rand()%20; //����������Ľڵ�����
	int copytimes = times;
	vector<int> arr;
	while (copytimes--)
	{
		int val = rand() % 100;
		arr.push_back(val);
		tree.insert(val); // �����������ڵ�Ĵ�С����Χ����0~99֮��
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
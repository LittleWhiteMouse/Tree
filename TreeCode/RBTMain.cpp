#include<iostream>
#include<time.h>
#include<vector>
#include "RBTree.h"
using namespace std;

int main(int argc, char** argv) {
	srand((int)time(0));
	RBTree<int> tree;
	int times = rand() % 20;
	int copytimes = times;
	vector<int>arr;
	while (copytimes--)
	{
		int val = rand() % 100;
		arr.push_back(val);
		tree.insert(val);
	}
	tree.MidOrder();
	cout << "-------------------------------------" << endl;
	times = 50;
	while (times--)
	{
		int todel = rand() % arr.size();
		cout << "\n try to del " << arr[todel] << endl;
		tree.remove(arr[todel]);
		tree.MidOrder();
		cout << "-------------------------------------" << endl;
	}

	system("pause");
	return 0;
}
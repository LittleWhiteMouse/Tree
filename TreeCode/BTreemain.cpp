#include <iostream>
#include "BTree.h"
using namespace std;

int main(int argc, char ** argv) {
	const int m = 3;
	BTree<int,m> t;
	t.insert(1);	t.insert(3);	t.insert(7);	t.insert(10);	
	t.insert(11);	t.insert(13);	t.insert(14);	t.insert(15);	
	t.insert(18);	t.insert(16);	t.insert(19);	t.insert(24);
	t.insert(25);	t.insert(26);	t.insert(21);	t.insert(4);
	t.insert(5);	t.insert(20);	t.insert(22);	t.insert(2);
	t.insert(17);	t.insert(12);	t.insert(6);
	cout << "遍历结果：  ";  t.traverse();  cout << endl;
	t.remove(16);
	cout << "遍历结果：  ";  t.traverse();  cout << endl;
	t.remove(13);
	cout << "遍历结果：  ";  t.traverse();  cout << endl;
	system("pause");

}
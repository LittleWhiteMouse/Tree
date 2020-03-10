#include<iostream>
#include "BSTree.h"
using namespace std;

int main(int argc, char**argv) {
	BSTree<int> tree;
	Element<int> a, b, c, d, e, f, g, h;
	a.key = 5; b.key = 3; c.key = 11;
	d.key = 3; e.key = 15; f.key = 2;
	g.key = 8; h.key = 22;

	tree.Insert(a); tree.Insert(b); tree.Insert(c); tree.Insert(d);
	tree.Insert(e); tree.Insert(f); tree.Insert(g); tree.Insert(h);

	tree.display();
	BSNode<int> *p = tree.Search(g);
	cout << "finding result = " << p->data.key << endl;
	BSNode<int> *p2 = tree.IterSearch(g);
	cout << "finding result = " << p2->data.key << endl;
	cout << "----------------------" << endl;
	tree.MidOrder();
	cout << "delete b=3 " << endl;
	tree.remove(b);
	tree.MidOrder();
	cout << "delete f=2" << endl;
	tree.remove(f);
	tree.MidOrder();
	cout << "delete e=15 " << endl;
	tree.remove(e);
	tree.MidOrder();
	system("pause");
	return 0;
}
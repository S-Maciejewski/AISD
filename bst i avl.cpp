#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <random>
#include <fstream>

#define DLUGOSC_LISTY 150000
#define PROBY 5

int MAX_HEIGHT = 0;
int tab_avl[DLUGOSC_LISTY];
int counter = 0;

using namespace std;

struct node {
	node * left;
	node * right;
	int data;
};

void t_push_in_root(node * & root, int v) {

	node * p;
	p = new node;
	p->data = v;
	p->left = NULL;
	p->right = NULL;
	root = p;
}

void insert_as_left_son(node * current, int v) {
	node *leaf;
	leaf = new node;
	current->left = leaf;
	leaf->data = v;
	leaf->left = leaf->right = NULL;
}

void insert_as_right_son(node * current, int v) {
	node * leaf;
	leaf = new node;
	current->right = leaf;
	leaf->data = v;
	leaf->left = leaf->right = NULL;
}

void t_insert_elem(node * &root, int v) {
	if (!root) t_push_in_root(root, v);
	else {
		node * e;
		e = root;
		bool inserted = false;
		while (!inserted)
		{
			if (v < e->data)
			{
				if (e->left) e = e->left;
				else
				{
					insert_as_left_son(e, v);
					inserted = true;
				}
			}
			else
			{
				if (e->right) e = e->right;
				else
				{
					insert_as_right_son(e, v);
					inserted = true;
				}
			}
		}
	}
}

void make_in_order_list(node * node) {
	if (node->left)
	{
		make_in_order_list(node->left);
		/*tab_avl[counter] = node->data;
		counter++;*/
	}
	tab_avl[counter] = node->data;
	counter++;
	if (node->right) make_in_order_list(node->right);
}

void check_height(node * node, int current_height) {
	if (current_height > MAX_HEIGHT) MAX_HEIGHT = current_height;
	current_height++;
	if (node->left) check_height(node->left, current_height);
	if (node->right) check_height(node->right, current_height);
}

void delete_tree(node * &node)
{
	if (node->left)
	{
		delete_tree(node->left);
		node->left = NULL;
	}
	if (node->right) {
		delete_tree(node->right);
		node->right = NULL;
	}
	delete node;
}

void print_tree(node * &root) {
	if (root) {
		node *p;
		p = root;
		cout << " Node : " << p->data << " ";
		if (p->left) cout << "l : " << p->left->data << " ";
		if (p->right) cout << "r : " << p->right->data;
		cout << endl;
		if (p->left) print_tree(p->left);
		if (p->right) print_tree(p->right);
	}
}

void build_avl(node * &root, int begin, int end) {
	int middle = (begin + end) / 2;
	if (end >= begin) {
		t_insert_elem(root, tab_avl[middle]);
		if (end - begin > 0) {
			build_avl(root, begin, middle - 1);
			build_avl(root, middle + 1, end);
		}
	}
}

int main()
{
	random_device random;
	mt19937 gen(random());
	ofstream out("wyniki.txt");
	srand(time(NULL));
	clock_t start;
	double t_build_tree = 0, t_search_tree = 0, t_del_tree = 0;
	double avlh = 0, bsth = 0;

	//out << setw(2) << " n," << setw(7) << "bst," << setw(7) << "avl" << endl;
	cout << DLUGOSC_LISTY << " elementow" << endl;

	for (int k = 0; k < PROBY; k++)
	{
		MAX_HEIGHT = 0;
		counter = 0;
#pragma region Tworzenie listy unikatowych elementów
	uniform_int_distribution<> distribution(1, DLUGOSC_LISTY * 100);
	int *tab = new int[DLUGOSC_LISTY];
	tab[0] = rand() % (DLUGOSC_LISTY * 10);
	for (int i = 0; i < DLUGOSC_LISTY;)
	{
		int r = distribution(gen);
		bool unique = true;
		for (int j = i; j >= 0; j--)
		{
			if (r == tab[j])
				unique = false;
		}
		if (unique)
			tab[++i] = r;
	}


#pragma endregion

#pragma region Budowanie drzewa
	start = clock();
	node * root = NULL;
	for (int i = 0; i<DLUGOSC_LISTY; i++)
	{
		t_insert_elem(root, tab[i]);
	}
	t_build_tree = (clock() - start) / (double)CLOCKS_PER_SEC;
	//cout << "Czas budowania drzewa : " << t_build_tree << endl;
#pragma endregion

#pragma region Szukanie elementów
	start = clock();
	for (int i = 1; i<DLUGOSC_LISTY; i++)
	{
		node * find;
		find = root;
		while (find->data != tab[i])
		{
			if (find->data > tab[i]) find = find->left;
			else find = find->right;
		}
	}
	t_search_tree = (clock() - start) / (double)CLOCKS_PER_SEC;
	//cout << "Czas szukania elementow " << t_search_tree << endl;
#pragma endregion

#pragma region Badanie wysokoœci drzew BST i AVL
	check_height(root, 1);
	//cout << "Wysokosc drzewa BST : " << MAX_HEIGHT << endl;
	//out << "BST_height, " << MAX_HEIGHT;
	bsth += MAX_HEIGHT;
	//Tworzenie tablicy in-order
	make_in_order_list(root);

	//Budowanie drzewa AVL
	node * root_avl = NULL;
	build_avl(root_avl, 0, DLUGOSC_LISTY - 1);

	MAX_HEIGHT = 1;
	check_height(root_avl, 1);

	//cout << "Wysokosc drzewa AVL : " << MAX_HEIGHT << endl;
	//out << "AVL_height, " << MAX_HEIGHT << ", ilosc elementow, " << DLUGOSC_LISTY;
	avlh += MAX_HEIGHT;
#pragma endregion

#pragma region Usuwanie drzewa
	start = clock();
	delete_tree(root);
	root = NULL;
	t_del_tree = (clock() - start) / (double)CLOCKS_PER_SEC;
	//cout << "Czas usuwania drzewa " << t_del_tree << endl;
#pragma endregion
	}

	cout << "Wysokosc bst: " << bsth / PROBY << endl;;
	cout << "Wysokosc avl: " << avlh / PROBY << endl;

	return 0;
}
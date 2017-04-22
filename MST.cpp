#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <fstream>

using namespace std;

#define NODES 100
#define SAT 0.3
#define MAX_WEIGHT 1000
#define KROKI 15
#define PROBY 10

void table_generator(int **tab, int size)
{
	for (int i = 0; i < size; i++)
		tab[i] = new int[size] {0};
	int r_i, r_j, r, j, counter = (size*(size - 1) / 2) * SAT + 1;
	for (j = 0; j < size; j++)
	{
		do
			r_j = rand() % size;
		while (r_j == j);
		r = rand() % MAX_WEIGHT + 1;
		tab[j][r_j] = r;
		tab[r_j][j] = r;
	}
	counter -= j;
	while(counter>0)
	{
		do
			r_i = rand() % size, r_j = rand() % size;
		while (r_i == r_j || tab[r_i][r_j] != 0);
		r = rand() % MAX_WEIGHT + 1;
		tab[r_i][r_j] = tab[r_j][r_i] = r;
		counter--;
	}
}

int minKey(int key[], bool mstSet[], int size)
{
	int min = MAX_WEIGHT, min_index=0;

	for (int v = 0; v < size; v++)
		if (mstSet[v] == false && key[v] < min && key[v] != 0)
			min = key[v], min_index = v;

	return min_index;
}

void printMST(int parent[], int size, int **tab)
{
	cout << ("Edge   Weight") << endl;
	for (int i = 1; i < size; i++)
		cout << parent[i] << " - " << i << "   " << tab[i][parent[i]] << endl;
}

void table_MST(int **tab, int size)
{
	int *parent = new int[size]; 
	int *key = new int[size];   
	bool *mstSet = new bool[size];  
	for (int i = 0; i < size; i++)
	{
		key[i] = MAX_WEIGHT;
		mstSet[i] = false;
	}

	key[0] = 0;     
	parent[0] = -1;

	for (int count = 0; count < size - 1; count++)
	{
		int u = minKey(key, mstSet, size);
		mstSet[u] = true;

		for (int v = 0; v < size; v++)
			if (tab[u][v] && mstSet[v] == false && tab[u][v] <  key[v])
				parent[v] = u, key[v] = tab[u][v];
	}

	delete key;
	delete parent;
	delete mstSet;
	//printMST(parent, size, tab);
}

int main()
{
	srand(time(NULL));
	clock_t start;
	ofstream out("wyniki.txt");
	out << "size, t_table, sat = " << SAT << endl;

	for (int k = 1; k <= KROKI; k++)
	{
		double t = 0;
		int size = NODES * k;

		for (int p = 0; p < PROBY; p++)
		{	
		int **tab = new int*[size];

		table_generator(tab, size);

		start = clock();
		table_MST(tab, size);
		t += (clock() - start) / (double)CLOCKS_PER_SEC;
		delete tab;
		}

		t /= PROBY;
		cout << size << " : " << t << endl;
		out << size << ", " << t << ", " << endl;
	}

	return 0;
}
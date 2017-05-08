#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <random>

#define nodes 20
#define sat 0.7

using namespace std;

int m = floor(nodes*(nodes - 1)*sat / 2), sptr = 0;
int ** tab, * stack;	//stos dla funkcji znajdowania cyklu
	

void findEuler(int v)
{
	for (int i = 0; i < nodes; i++)
		while (tab[v][i])
		{
			tab[v][i]--, tab[i][v]--;
			findEuler(i);
		}
	stack[sptr++] = v;
}

void generate() {
	random_device random;
	mt19937 gen(random());
	uniform_int_distribution<> distribution(0, nodes - 1);

	int arcCounter = 0;
	for (int i = 0; i < nodes - 1; i++)
	{
		tab[i][i + 1]++, tab[i + 1][i]++;
		arcCounter++;
	}

	tab[0][nodes - 1]++, tab[nodes - 1][0]++;
	arcCounter++;

	while (arcCounter < m - 3)
	{
		int a = 0, b = 0, c = 0;

		do
			do
			{
				a = distribution(gen), b = distribution(gen);
				cout << "a, b generated" << endl;
			} while (a == b);
		while (tab[a][b] == 1);
		do
			do
			{
				c = distribution(gen);
				cout << "c generated" << endl;
			}
			while (c == a || c == b);
		while (tab[b][c] == 1 || tab[c][a] == 1);

		tab[a][b]++, tab[b][a]++;
		tab[b][c]++, tab[c][b]++;
		tab[c][a]++, tab[a][c]++;

		arcCounter += 3;
	}
	cout << endl << "Graph generated successfully" << endl;
	cout << "Edge amound: " << arcCounter << endl;
}

void findHamilton(int v)
{
	stack[sptr++] = v;
	for (int i = 0; i < nodes; i++)
		if (tab[v][i])
			findHamilton(i);
	if (sptr == nodes && tab[v][0])	//zakladam, ze punktem poczatkowym jest 0
	{
		cout << "Cykl Hamiltona znaleziony" << endl;
		return;
	}
	else
		stack[--sptr] = -1;
}

int main()
{
	time_t start;
	double eulerTime = 0, hamiltonTime = 0;

	tab = new int *[nodes];            
	stack = new int[m];

	for (int i = 0; i < nodes; i++)
	{
		tab[i] = new int[nodes];
		for (int j = 0; j < nodes; j++) 
			tab[i][j] = 0;
	}
		     
	generate();

	//wypisywanie tabeli
	for (int i = 0; i < nodes; i++)
	{
		for (int j = 0; j < nodes; j++)
		{
			cout << tab[i][j] << ", ";
		}
		cout << endl;
	}

	start = clock();
	findEuler(0);
	eulerTime = (clock() - start) / (double)CLOCKS_PER_SEC;


	//wypisywanie cyklu
	cout << "Cykl Eulera :" << endl;
	for (int i = 0; i < sptr; i++)
		cout << stack[i] << ", ";

	cout << endl << "Czas dzialania: " << eulerTime << endl;

	delete[] stack;
	stack = new int[nodes];
	sptr = 0;
	findHamilton(0);


	delete[] tab;
	delete[] stack;


	return 0;
}
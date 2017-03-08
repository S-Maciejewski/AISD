// C++ Algorytmika.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<cstdlib>
#include<iostream>
#include<ctime>
#include<algorithm>

using namespace std;


void s_sort(int *tab, int size)
{
	int a;
	for (int i = 0; i < size; i++)
	{
		a = i;
		for (int j = i + 1; j < size; j++)
			if (tab[j] < tab[a])
				a = j;

		swap(tab[a], tab[i]);
	}
}

int main() 
{
	int k = 1000, n = 1;
	double t_ss = 0, t_is = 0, t_hs = 0, t_ms = 0;
	srand(time(NULL));
	while (n < 16)
	{

	

	for (int proby = 0; proby < 10; proby++)
	{
		int *dane = new int[k*n];
		for (int i = 0; i < k*n; i++)
		{
			dane[i] = rand() % (k*n * 10);	//generujê dane z zagresu 0 - n*k*10, zamiast 0 - n*k. Lepsza próbka
		}

		//zrobiæ dla wszystkich losowañ
		const clock_t start = clock();	//inicjalizacja timera
		s_sort(dane, k*n);
		t_ss += (clock() - start) / (double)CLOCKS_PER_SEC;
		//cout << (clock() - start) / (double)CLOCKS_PER_SEC << endl;

		delete[] dane;
	}
	t_ss /= 10;
	cout << "n = " << n << ", t= " << t_ss << endl;
	
	n++;
	}

    return 0;
}


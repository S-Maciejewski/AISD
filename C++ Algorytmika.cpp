#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <math.h>
#include <fstream>
#include <random>

#define DLUGOSC_LISTY 50000

using namespace std;

struct lista
{
	lista * next;
	int data;
};

//Liczenie elementow
int l_size(lista * p)
{
	int c = 0;
	while (p)
	{
		c++;    
		p = p->next;
	}
	return c;
}

//Wyswietlanie elementow
void l_printl(lista * p)
{
	cout << "Ilosc elementow : " << l_size(p) << endl;

	for (int i = 0; p; p = p->next)
		cout << "Element nr. " << i++ << ",  data = " << p->data << endl;
	cout << endl;
}

//Wstawianie na poczatek listy
void l_push_front(lista * & head, int v)
{
	lista * p;
	p = new lista;  
	p->data = v;      
	p->next = head;
	head = p;
}

//Wstawianie na koniec listy
void l_push_back(lista * & head, int v)
{
	lista * p, *e;
	e = new lista;  
	e->next = NULL;   
	e->data = v;
	p = head;
	if (p)
	{
		while (p->next) p = p->next;
		p->next = e;
	}
	else head = e;
}

//Wstawianie przed e
void l_insert_before(lista * & head, lista * e, int v)
{
	lista * p = head;
	if (p == e) l_push_front(head, v);
	else
	{
		while (p->next != e) p = p->next;
		p->next = new lista;
		p->next->next = e;
		p->next->data = v;
	}
}

//Wstawianie za e
void l_insert_after(lista * e, int v)
{
	lista * p = new lista;
	p->next = e->next;
	p->data = v;
	e->next = p;
}

//Usuwanie pierwszego elementu
void l_pop_front(lista * & head)
{
	lista * p = head; 
	if (p)
	{
		head = p->next;   
		delete p;         
	}
}

//Usuwanie ostatniego elementu
void l_pop_back(lista * & head)
{
	lista * p = head; 
	if (p)
	{
		if (p->next)
		{
			while (p->next->next) p = p->next;
			delete p->next;
			p->next = NULL;
		}
		else
		{
			delete p;
			head = NULL;
		}
	}
}

//Usuwanie wybranego elementu
void l_remove(lista * & head, lista * e)
{
	lista * p;
	if (head == e) l_pop_front(head);
	else
	{
		p = head;
		while (p->next != e) p = p->next;
		p->next = e->next;
		delete e;
	}
}


int main()
{
	random_device random;
	mt19937 gen(random());

	ofstream out("wyniki.txt");
	lista * L = NULL;
	clock_t start;
	double t_build_list = 0, t_search_list = 0, t_del_list = 0;

	out << "Wyniki dla " << DLUGOSC_LISTY << " elementow" << endl;

	//Generowanie tablicy losowych, unikatowych elementow

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


	//Wstawianie elementów do listy (z sortowaniem)

	start = clock();
	l_push_back(L, tab[0]);
	if (tab[1] > tab[0])
		l_push_back(L, tab[1]);
	else
		l_push_front(L, tab[1]);
	for (int i = 2; i < DLUGOSC_LISTY; i++)
	{
		lista *e = L;
		bool inserted = false;
		while (e->next)
		{
			if (tab[i] > e->data && tab[i] < e->next->data)
			{
				l_insert_after(e, tab[i]);
				inserted = true;
				break;
			}
			e = e->next;
		}
		if (!inserted)
			if (tab[i] < L->data)
				l_push_front(L, tab[i]);
			else
				l_push_back(L, tab[i]);
	}
	t_build_list = (clock() - start) / (double)CLOCKS_PER_SEC;
	//l_printl(L);

	cout << "Zbudowanie posortowanej listy o dlugosci " << DLUGOSC_LISTY << " zajelo " << t_build_list << " sekund" << endl;
	out << "t_build_list, " << t_build_list << endl;
	

	//Wyszukiwanie elementow listy

	start = clock();
	for (int i = 0; i < DLUGOSC_LISTY; i++)
	{
		lista *e = L;
		for (int j = 0; j < i; j++)
		{
			e = e->next;
		}
	}
	t_search_list = (clock() - start) / (double)CLOCKS_PER_SEC;

	cout << "Przeszukanie listy o dlugosci " << DLUGOSC_LISTY << " zajelo " << t_search_list << " sekund" << endl;
	out << "t_search_list, " << t_search_list << endl;


	//Usuwanie listy

	start = clock();
	while (L->next)
	{
		l_pop_front(L);
	}
	t_del_list = (clock() - start) / (double)CLOCKS_PER_SEC;

	cout << "Usuniecie listy o dlugosci " << DLUGOSC_LISTY << " zajelo " << t_del_list << " sekund" << endl;
	out << "t_del_list, " << t_del_list << endl;


	return 0;
}

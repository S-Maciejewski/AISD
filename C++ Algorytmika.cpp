#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <math.h>

#define DLUGOSC_LISTY 100

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
	srand(time(NULL));
	lista * L = NULL;

	//Generowanie tablicy losowych, unikatowych elementow
	int *tab = new int[DLUGOSC_LISTY];
	tab[0] = rand() % (DLUGOSC_LISTY * 10);
	
	for (int i = 0; i < DLUGOSC_LISTY;)
	{
		int r = rand() % (DLUGOSC_LISTY * 10);
		bool unique = true;
		for (int j = 0; j < i; j++)
		{
			if (tab[j] == tab[i])
				unique = false;
		}
		if (unique)
		{
			tab[i++] = r;
		}
	}

	//Wstawianie elementów do listy (z sortowaniem)


	return 0;
}

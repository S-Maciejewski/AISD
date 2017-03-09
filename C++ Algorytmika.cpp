#include<cstdlib>
#include<iostream>
#include<ctime>
#include<algorithm>

using namespace std;

void selection_sort(int *tab, int size)
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

void insertion_sort(int *tab, int size)
{
    int i;
    for(int j = 1; j < size; j++)
    {
        int key = tab[j];
        i = j - 1;
        while (i >= 0 && key < tab[i])
        {
            tab[i + 1] = tab[i];
            tab[i] = key;
            i--;
        }
    }
}

int main()
{
    int k = 1000, n = 1;
    double t_ss = 0, t_is = 0, t_hs = 0, t_ms = 0;
    srand(time(NULL));
    clock_t start;
    while (n < 16)
    {
        for (int proba = 0; proba < 10; proba++)
        {
            int *dane = new int[k*n];
            for (int i = 0; i < k*n; i++)
            {
                dane[i] = rand() % (k*n*10);	//generujê dane z zagresu 0 - n*k*10, zamiast 0 - n*k. Lepsza próbka
            }
            //INSERTION SORT
            start = clock();	//inicjalizacja timera
            insertion_sort(dane, k*n);
            t_is += (clock() - start) / (double)CLOCKS_PER_SEC;
            //cout << (clock() - start) / (double)CLOCKS_PER_SEC << " ";

            //SELECTION SORT
            start = clock();	//inicjalizacja timera
            selection_sort(dane, k*n);
            t_ss += (clock() - start) / (double)CLOCKS_PER_SEC;
            //cout << (clock() - start) / (double)CLOCKS_PER_SEC << " || ";

            delete[] dane;
        }
        t_ss /= 10;
        t_is /= 10;
        cout << endl << "Dla " << n*k << " liczb : " << "t_is= " << t_is << ", t_ss= " << t_ss << endl;
        n++;
    }

    return 0;
}

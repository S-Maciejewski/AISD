#include<cstdlib>
#include<iostream>
#include<ctime>
#include<algorithm>

#define LICZBA_PROB 1000
#define KROK 1000
#define LICZBA_KROKOW 15

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

void merge_sort(int *a, int start, int size, int *b)
{
    int middle = ((start + size) / 2) - 1;
    if((middle - start) > 0) merge_sort(a, start, middle+1, b);
    if((size - middle) > 2) merge_sort(a, middle+1, size, b);
    int i = start;
    int j = middle + 1;
    for(int k = start; k < size; k++)
    {
        if(((i <= middle) && (j >= size)) || (((i <= middle) && (j < size)) && (a[i] <= a[j])))
        {
            b[k] = a[i];
            i = i + 1;
        }
        else
        {
            b[k] = a[j];
            j = j + 1;
        }
    }
    for(int k = start; k < size; k++)
        a[k] = b[k];

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
    int k = KROK, n = LICZBA_KROKOW;
    double t_ss = 0, t_is = 0, t_hs = 0, t_ms = 0;
    srand(time(NULL));
    clock_t start;
    while (n <= 15)
    {
        for (int proba = 0; proba < LICZBA_PROB; proba++)
        {
            int *dane_is = new int[k*n], *dane_ss = new int[k*n];
            int *dane_hs = new int[k*n], *dane_ms = new int[k*n];
            for (int i = 0; i < k*n; i++)
            {
                dane_is[i] = dane_ss[i] = dane_hs[i] = dane_ms[i] = rand() % (k*n*10);
            }

            //INSERTION SORT
            start = clock();	//inicjalizacja timera
            insertion_sort(dane_is, k*n);
            t_is += (clock() - start) / (double)CLOCKS_PER_SEC;
            //cout << (clock() - start) / (double)CLOCKS_PER_SEC << " ";

            //SELECTION SORT
            start = clock();	//inicjalizacja timera
            selection_sort(dane_ss, k*n);
            t_ss += (clock() - start) / (double)CLOCKS_PER_SEC;
            //cout << (clock() - start) / (double)CLOCKS_PER_SEC << " ";

            //MERGE SORT
            int *b = new int[k*n];
            start = clock();	//inicjalizacja timera
            merge_sort(dane_ms, 0, k*n, b);
            t_ms += (clock() - start) / (double)CLOCKS_PER_SEC;
            //cout << (clock() - start) / (double)CLOCKS_PER_SEC << " || ";

            delete[] dane_is;
            delete[] dane_ss;
            delete[] dane_hs;
            delete[] dane_ms;
        }
        cout<<endl;
        t_ss /= LICZBA_PROB;
        t_is /= LICZBA_PROB;
        t_ms /= LICZBA_PROB;
        cout << endl << "Dla " << n*k << " liczb : " << "t_is= " << t_is << ", t_ss= " << t_ss << ", t_ms= " << t_ms << endl;
        n++;
        t_ss = t_is = t_ms = t_hs = 0;   //konieczne zerowanie liczników czasu - inaczej pomiary są zafałszowane
    }

    return 0;
}

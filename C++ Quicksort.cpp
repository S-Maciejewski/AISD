#include <iostream>
#include <ctime>
#include<cstdlib>
#include<fstream>
#include<algorithm>
#include<iomanip>

#define LICZBA_PROB 100
#define KROK 10000
#define LICZBA_KROKOW 15

using namespace std;

void printtab(int *tab, int size)
{
    for(int i = 0; i<size; i++)
    {
        cout<<tab[i]<<" ";
    }
    cout<<endl;
}

int partition(int *tab, int left, int right, int version)
{
    int piwot;
    if(version == 0) piwot = tab[(left + right) / 2];   // środek
    if(version == 1) piwot = tab[right];                // prawo
    if(version == 2)                                    // losowo
    {
        srand(time(NULL));
        piwot = tab[rand()%(right+1)];
    }

    int l = left, r = right;
    while (true)
    {
        while (tab[l] < piwot) l++;
        while (tab[r] > piwot) r--;
        if (l < r) {
            if (tab[l] == tab[r]) l++;
            else swap(tab[l], tab[r]);
        }
        else return r;
    }
}

void quicksort_recursion(int *tab, int left, int right, int version)
{
    int middle;
    if(left < right)
    {
        middle = partition(tab, left, right, version); // checking whether there is more than one element in array
        quicksort_recursion(tab, left, middle-1, version);
        quicksort_recursion(tab, middle + 1, right, version);
    }
}

void quicksort_iteration (int *tab, int left, int right, int version)
{
    int stack[right];
    int stack_top = -1;

    stack[++stack_top] = left;
    stack[++stack_top] = right;

    // Keep popping from stack while is not empty
    while (stack_top >= 0 )
    {
        right = stack[stack_top--];
        left = stack[stack_top--];

        int piwot = partition(tab, left, right, version);

        if ( piwot-1 > left )
        {
            stack[ ++stack_top ] = left;
            stack[ ++stack_top ] = piwot - 1;
        }
        if ( piwot+1 < right )
        {
            stack[ ++stack_top ] = piwot+1;
            stack[ ++stack_top ] = right;
        }
    }
}

int main()
{
    int k = KROK, n = 1;
    double t_qs_iter = 0, t_qs_recur = 0;
    srand(time(NULL));
    ofstream out("wyniki_quicksort.txt");
    out << setw(2) << " n," << setw(8) << "t_rec," << setw(8) << "t_iter" <<endl;
    clock_t start;
    while (n <= LICZBA_KROKOW)
    {
        for (int proba = 0; proba < LICZBA_PROB; proba++)
        {
            int *dane_qs_recur = new int[k * n], *dane_qs_iter = new int[k * n];

            //DODAĆ A-KSZTAŁTNY CIĄG WEJŚCIOWY
            for (int i = 0; i < k * n; i++)
            {
                dane_qs_recur[i] = dane_qs_iter[i] = rand() % (k * n * 10);
            }

            //WYBRAĆ RODZAJ DANYCH DO POSORTOWANIA

            //RECURSIVE VERSION
            start = clock();    //inicjalizacja timera
            quicksort_recursion(dane_qs_recur, 0, k*n-1, 0);
            t_qs_recur += (clock() - start) / (double) CLOCKS_PER_SEC;

            //ITERATIVE VERSION
            start = clock();    //inicjalizacja timera
            quicksort_iteration(dane_qs_iter, 0, k*n-1, 0);
            t_qs_iter += (clock() - start) / (double) CLOCKS_PER_SEC;

            delete[] dane_qs_iter;
            delete[] dane_qs_recur;
        }

        t_qs_iter /= LICZBA_PROB;
        t_qs_recur /= LICZBA_PROB;

        out << setw(2) << n << "," << setw(8) << t_qs_recur << "," << setw(8) << t_qs_iter << endl;
          n++;
        t_qs_iter = t_qs_recur = 0;   //konieczne zerowanie licznikow czasu - inaczej pomiary sa zafalszowane
    }
    return 0;
}

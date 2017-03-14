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

int draw(int min, int max)
{
    int result;
    srand(time(NULL));
    result = (rand() % (max - min + 1)) + min;
    return result;
}

int partition(int *tab, int left, int right, int version)
{
    int piwot;
    if(version == 0) piwot = tab[(left + right) / 2];   // środek
    if(version == 1) piwot = tab[right];                // prawo
    if(version == 2) piwot = tab[draw(left, right)];

    int l = left, r = right;
    while (true)
    {
        while (tab[l] < piwot) l++;
        while (tab[r] > piwot) r--;
        if (l < r) {
            if (tab[l] == tab[r]) l++;
            else swap(tab[l], tab[r]);
            //printtab(tab, right+1);
        }
        else
        {
//            if(version == 2) printtab(tab, right+1);
            return r;
        }
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
    double time0 = 0, time1 = 0, time2 = 0;
    srand(time(NULL));
    ofstream out("wyniki_quicksort.txt");
    out << setw(2) << " n," << setw(8) << "środek," << setw(8) << "prawo," << setw(8) << "losowo" <<endl;
    clock_t start;
    while (n <= LICZBA_KROKOW)
    {
        for (int proba = 0; proba < LICZBA_PROB; proba++)
        {
            int *dane_qs_recur = new int[k * n], *dane_qs_iter = new int[k * n];
            int *dane_qs_iter0 = new int[k * n], *dane_qs_iter1 = new int[k * n], *dane_qs_iter2 = new int[k * n];


            //DODAĆ A-KSZTAŁTNY CIĄG WEJŚCIOWY

            for (int i = 0; i < k*n; i++)
            {
                dane_qs_iter[i] = rand() % (k*n * 10);
            }
            //printtab(dane_qs_iter, n*k);
            int *temp = new int[k*n];
            quicksort_iteration(dane_qs_iter, 0, k*n-1, 0);
            for (int j = 0; j < n*k; j+=2)
            {
                temp[j/2] = dane_qs_iter[j];
                temp[n*k -1 - j/2] = dane_qs_iter[j + 1];
                if(k%2 == 1) temp[(n*k-1)/2] = dane_qs_iter[n*k-1];
            }
            for (int i = 0; i < k*n; i++)
            {
                dane_qs_iter0[i] = dane_qs_iter1[i] = dane_qs_iter2[i] = temp[i];
            }
            delete[] temp;

//            for (int i = 0; i < k * n; i++)
//            {
//                dane_qs_recur[i] = dane_qs_iter[i] = rand() % (k * n * 10);
//            }


            //WYBRAĆ RODZAJ DANYCH DO POSORTOWANIA

//            //RECURSIVE VERSION
//            start = clock();    //inicjalizacja timera
//            quicksort_recursion(dane_qs_recur, 0, k*n-1, 0);
//            t_qs_recur += (clock() - start) / (double) CLOCKS_PER_SEC;

            //ITERATIVE VERSION
            //cout<<"środek"<<endl;
            start = clock();    //inicjalizacja timera
            quicksort_recursion(dane_qs_iter0, 0, k*n-1, 0);
            time0 += (clock() - start) / (double) CLOCKS_PER_SEC;


            //cout<<"prawo"<<endl;
            start = clock();    //inicjalizacja timera
            quicksort_recursion(dane_qs_iter1, 0, k*n-1, 1);
            time1 += (clock() - start) / (double) CLOCKS_PER_SEC;


            start = clock();    //inicjalizacja timera
            quicksort_recursion(dane_qs_iter2, 0, k*n-1, 2);
            time2 += (clock() - start) / (double) CLOCKS_PER_SEC;


            delete[] dane_qs_iter;
//            delete[] dane_qs_recur;
            delete[] dane_qs_iter0;
            delete[] dane_qs_iter1;
            delete[] dane_qs_iter2;
        }

        time0 /= LICZBA_PROB;
        time1 /= LICZBA_PROB;
        time2 /= LICZBA_PROB;

        out << setw(2) << n << "," << setw(8) << time0 << "," << setw(8) << time1 << "," << setw(8) << time2 << endl;
          n++;
        //t_qs_iter = t_qs_recur = 0;   //konieczne zerowanie licznikow czasu - inaczej pomiary sa zafalszowane
        time0 = time1= time2 = 0;
    }
    return 0;
}



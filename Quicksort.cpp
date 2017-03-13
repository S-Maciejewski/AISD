#include <iostream>
#include <ctime>
#include<cstdlib>

using namespace std;

void printtab(int *tab, int size)
{
    for(int i = 0; i<size; i++)
    {
        cout<<tab[i]<<" ";
    }
    cout<<endl;
}

int partition(int *tab, int left, int right)
{
    //method of finding piwot
    int piwot = tab[(left+right)/2];
    int l = left, r = right;

    while(true)
    {
        while(tab[l] < piwot) l++;
        while(tab[r] > piwot) r--;
        if(l<r)
        {
            if(tab[l] == tab[r]) r--;
            else swap(tab[l], tab[r]);
        }
        else return r;
    }
}

void quicksort_recursion(int *tab, int left, int right)
{
    int middle;
    if(left < right)
    {
        middle = partition(tab, left, right); // checking whether there is more than one element in array
        quicksort_recursion(tab, left, middle);
        quicksort_recursion(tab, middle + 1, right);
    }
}
int main()
{
    int k = 150000, n = 1;
    clock_t start;
    double t_qs = 0;
    
    //data initialization
    int *dane_qs = new int[k * n];
    for (int i = 0; i < k * n; i++)
    {
        dane_qs[i] = rand() % (k * n * 10);
    }
    
    srand(time(NULL));
    start = clock();
    quicksort_recursion(dane_qs, 0, k * n - 1);
    t_qs += (clock() - start) / (double) CLOCKS_PER_SEC;

    cout<<t_qs<<endl;
    return 0;
}

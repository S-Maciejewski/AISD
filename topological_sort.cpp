#include <iostream>
#include <cstdlib>
#include <ctime>
#include<algorithm>
#define NODE_NUMBER 1000

using namespace std;

struct node{
    int value;
    node* next;
};
node * root = NULL;

void insert_element(int value) {
    node * element = new node, *head = root;
    element->value = value;
    element->next = head;
    root = element;
}

void print_table(int **table, int n) {
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            cout<<table[i][j]<<" ";
        }
        cout<<endl;
    }
}

void switch_columns(int **DAG_table, int dimension, int node1, int node2) {
    for(int i=0;i<dimension;i++) swap(DAG_table[node1][i], DAG_table[node2][i]);
    for(int i=0;i<dimension;i++) swap(DAG_table[i][node1], DAG_table[i][node2]);
}

void DAG_table_generator(int **DAG_table, int dimension)
{
    int saturation = int(0.6 * (dimension*(dimension-1))/2) + 1;
    int counter = saturation;

    //umieszczanie połączeń w każdym wierszu tabeli
    for(int i=0;i<dimension-1;i++)
    {
        int rand_j;
        do
        {
            rand_j = rand() % dimension;
        }
        while(rand_j<=i);
        DAG_table[i][rand_j] = 1;
    }

    //umieszczenie reszty połączeń
    counter-=(dimension-1);
    while(counter)
    {
        int rand_i, rand_j;
        rand_i = rand()%(dimension-1);
        rand_j = rand()%(dimension-1)+1;
        if(rand_i < rand_j && DAG_table[rand_i][rand_j]!=1)
        {
            DAG_table[rand_i][rand_j] = 1;
            counter--;
        }
    }
    //zamiana kolumn i wierszy zachowując acykliczność grafu
    for(counter=dimension*2; counter>0;counter--)
    {
        int rand_node1 = rand()%dimension;
        int rand_node2 = rand()%dimension;
        switch_columns(DAG_table, dimension, rand_node1, rand_node2);
    }
}

void DFS(int **DAG_table, int n, bool *visited, int current_node) {
    if(!visited[current_node])
    {
        visited[current_node] = true;
        for(int i=0;i<n;i++)
        {
            if(DAG_table[current_node][i]==1 && !visited[i])
                DFS(DAG_table, n, visited, i);
        }
        insert_element(current_node);
    }
}

void DAG_table_Topological_Sort(int **DAG_table, int n, bool *visited)
{
    for(int i=0;i<n;i++)
    {
        //wywoływanie rekurecyjne po wszystkich wierzchołkach
        DFS(DAG_table, n, visited, i);
    }
}

int main() {
    srand(time(NULL));
    clock_t start;
    int n = NODE_NUMBER;
    bool DAG_visiting_table[n] {false};

    //tworzenie tablicy grafu DAG + zerowanie całej tablicy
    int ** DAG_table = new int *[n];
    for(int i=0;i<n;i++)
    {
        DAG_table[i] = new int [n];
        for(int j=0;j<n;j++)
        {
            DAG_table[i][j] = 0;
        }
    }
    //generowanie tablicy grafu DAG
    cout<<"Generowanie..."<<endl;
    DAG_table_generator(DAG_table, n);
    cout<<"GOTOWE"<<endl<<endl;
//    print_table(DAG_table, n);
//    cout<<endl;
    double t_topological_sort_table = 0;
    start = clock();
    DAG_table_Topological_Sort(DAG_table, n, DAG_visiting_table);
    t_topological_sort_table = (clock() - start) / (double)CLOCKS_PER_SEC;
    cout<<t_topological_sort_table<<endl;

    //wyświetlanie listy posortowanych topologicznie wierzchołków
//    node * node = root;
//    while(node)
//    {
//        cout<<node->value<<" ";
//        node = node->next;
//    }
//    cout<<endl;

    return 0;
}

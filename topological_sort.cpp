#include <iostream>
#include <cstdlib>
#include <ctime>
#include<algorithm>
#define NODE_NUMBER 4000

using namespace std;
struct node1{
    int value;
    node1* next;
};
node1 * root1 = NULL;
void insert_element_list(int value) {
    node1 * element = new node1, *head = root1;
    element->value = value;
    element->next = head;
    root1 = element;
}

//incidence list
struct neighbour{
    int node_number;
    neighbour * next_neighbour;
};
void add_neighbour(neighbour** pointer, int node_number){
    neighbour* node = new neighbour;
    node->node_number = node_number;
    node->next_neighbour = NULL;
    if(!*pointer) *pointer = node;
    else
    {
        neighbour * pointer1;
        pointer1 = *pointer;
        while(pointer1->next_neighbour) pointer1 = pointer1->next_neighbour;
        pointer1->next_neighbour = node;
    }
}
void print_incidence_list(neighbour ** table_of_list, int dimension) {
    for(int i=0;i<dimension;i++)
    {
        cout<<i<<" : ";
        neighbour * first = table_of_list[i];
        while(first)
        {
            cout<<first->node_number<<" ";
            first = first->next_neighbour;
        }
        cout<<endl;
    }
}
void make_incidence_list(int **DAG_table, neighbour* table_of_list[], int dimension) {
    for(int i=0; i<dimension; i++)
    {
        table_of_list[i] = NULL;
        for(int j=0; j<dimension; j++)
        {
            if(DAG_table[i][j]) add_neighbour(&table_of_list[i], j);
        }
    }
}
void DFS_list(neighbour* table_of_list[], neighbour** pointer, int dimension, bool visited[], int node_number) {

    if(!visited[node_number])
    {
        visited[node_number] = true;
        if(*pointer) {
            neighbour *n = *pointer;
            while (n) {
                DFS_list(table_of_list, &table_of_list[n->node_number], dimension, visited, n->node_number);
                n = n->next_neighbour;
            }
        }
        insert_element_list(node_number);
    }
}
void DAG_incidence_list_Topological_Sort(neighbour* table_of_list[], int dimension, bool visited[]) {
    for(int i=0;i<dimension;i++)
            DFS_list(table_of_list, &table_of_list[i], dimension, visited, i);
}

//table
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
void DAG_table_generator(int **DAG_table, int dimension) {
    for(int i=0;i<dimension;i++) {
        DAG_table[i] = new int [dimension];
        for(int j=0;j<dimension;j++)
            DAG_table[i][j] = 0;

    }
    int saturation = int(0.6 * (dimension*(dimension-1))/2) + 1;
    int counter = saturation;

    //umieszczanie połączeń w każdym wierszu tabeli
    for(int i=0;i<dimension-1;i++) {
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
    while(counter) {
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
    for(counter=dimension*2; counter>0;counter--) {
        int rand_node1 = rand()%dimension;
        int rand_node2 = rand()%dimension;
        switch_columns(DAG_table, dimension, rand_node1, rand_node2);
    }
//    print_table(DAG_table, dimension);
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
void DAG_table_Topological_Sort(int **DAG_table, int n, bool *visited) {
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
    double t_topological_sort_table = 0, t_topological_sort_list = 0;
    bool DAG_visiting_table[n] {false};

    //generowanie tablicy grafu DAG
    int ** DAG_table = new int *[n];
    DAG_table_generator(DAG_table, n);

    //sortowanie topologiczne - macierz
    start = clock();

    DAG_table_Topological_Sort(DAG_table, n, DAG_visiting_table);

    t_topological_sort_table = (clock() - start) / (double)CLOCKS_PER_SEC;
    cout<<"Macierz dla "<<n<<" elementow : "<<t_topological_sort_table<<endl;

    //wyświetlanie listy posortowanych topologicznie wierzchołków - macierz
//    node * node = root;
//    while(node)
//    {
//        cout<<node->value<<" ";
//        node = node->next;
//    }
//    cout<<endl;

    //generowanie listy incydencji grafu DAG
    cout<<"Building list of incidence..."<<endl;
    neighbour* table_of_list[NODE_NUMBER];
    make_incidence_list(DAG_table,table_of_list, n);
    bool DAG_visiting_table_list[n] {false};

    cout<<"Sorting list of incidence..."<<endl;
    //sortowanie topologiczne - lista incydencji

    start = clock();

    DAG_incidence_list_Topological_Sort(table_of_list, n, DAG_visiting_table_list);

    t_topological_sort_list = (clock() - start) / (double)CLOCKS_PER_SEC;
    cout<<"Lista incydencji dla "<<n<<" elementow : "<<t_topological_sort_list<<endl;

    //wyświetlanie listy posortowanych topologicznie wierzchołków - lista incydencji
//    node1 * node1 = root1;
//    while(node1)
//    {
//        cout<<node1->value<<" ";
//        node1 = node1->next;
//    }
//    cout<<endl;

    return 0;
}

#include <iostream>
#include <cstdlib>
#include <ctime>
#include<algorithm>
#define NODE_NUMBER 20

using namespace std;

//incidence list
struct neighbour{
    int node_number;
    neighbour * next_neighbour;
};
struct main_node{
    int node_number;
    main_node * next_main;
    neighbour * first_neighbour;
};
main_node * incidence_list_root = NULL;

void insert_first(int number) {
    main_node * first = new main_node;
    first->node_number = number;
    first->first_neighbour = NULL;
    first->next_main = NULL;
    incidence_list_root = first;
}
void insert_next(int number){
    main_node * e = incidence_list_root, * next = new main_node;
    while(e->next_main) e = e->next_main;
    next->node_number = number;
    next->first_neighbour = NULL;
    next->next_main = NULL;
    e->next_main = next;
}
void add_main_node(int node_number) {
    if(!incidence_list_root) insert_first(node_number);
    else insert_next(node_number);
}
void add_neighbour(int node_number){
    main_node * last = incidence_list_root;
    while(last->next_main) last = last->next_main;

    neighbour *node = new neighbour;
    node->node_number = node_number;
    node->next_neighbour = NULL;
    if(!last->first_neighbour) last->first_neighbour = node;
    else{
        neighbour * last_neighbour = last->first_neighbour;
        while(last_neighbour->next_neighbour) last_neighbour = last_neighbour->next_neighbour;
        last_neighbour->next_neighbour = node;
    }
}
void print_incidence_list() {
    main_node * start = incidence_list_root;
    while(start)
    {
        cout<<start->node_number<<" : ";
        neighbour * first = start->first_neighbour;
        while(first)
        {
            cout<<first->node_number<<" ";
            first = first->next_neighbour;
        }
        cout<<endl;
        start = start->next_main;
    }
}

void make_incidence_list(int **DAG_table, int dimension) {
    for(int i=0; i<dimension; i++)
    {
        add_main_node(i);
        for(int j=0; j<dimension; j++)
        {
            if(DAG_table[i][j]) add_neighbour(j);
        }
    }
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
//    cout<<"Generowanie..."<<endl;
    DAG_table_generator(DAG_table, n);
//    cout<<"GOTOWE"<<endl<<endl;
    print_table(DAG_table, n);
    cout<<endl;

    //sortowanie z mierzeniem czasu
//    double t_topological_sort_table = 0;
//    start = clock();
//    DAG_table_Topological_Sort(DAG_table, n, DAG_visiting_table);
//    t_topological_sort_table = (clock() - start) / (double)CLOCKS_PER_SEC;
//    cout<<t_topological_sort_table<<endl;

    make_incidence_list(DAG_table, n);
    print_incidence_list();

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

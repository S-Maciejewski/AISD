#include <iostream>
#include <time.h>
#include<cstdlib>
#include <random>

#define DLUGOSC_LISTY 10000000

using namespace std;

struct node{
    node * left;
    node * right;
    int data;
};

void delete_tree(node * &node)
{
    if(node->left)
    {
        delete_tree(node->left);
        node->left = NULL;
    }
    if(node->right){
        delete_tree(node->right);
        node->right = NULL;
    }
    delete node;
}
void print_tree(node * &root) {
    if(root) {
        node *p;
        p = root;
        cout << " Node : " << p->data << endl;
        if (p->left) print_tree(p->left);
        if (p->right) print_tree(p->right);
        cout << "done" << endl;
    }
}

//Wstawianie korzenia
void t_push_in_root(node * & root, int v) {

    node * p;
    p = new node;
    p->data = v;
    p->left = NULL;
    p->right = NULL;
    root = p;
}

void insert_as_left_son(node * current, int v) {
    node *leaf;
    leaf = new node;
    current->left = leaf;
    leaf->data = v;
    leaf->left = leaf->right = NULL;
}

void insert_as_right_son(node * current, int v) {
    node * leaf;
    leaf = new node;
    current->right = leaf;
    leaf->data = v;
    leaf->left = leaf->right = NULL;
}

//Wstawianie elementu na właściwe miejsce
void t_insert_elem(node * &root, int v) {
    node * e;
    e = root;
    bool inserted = false;
    while(!inserted)
    {
        if(v < e->data)
        {
            if(e->left) e = e->left;
            else
            {
                insert_as_left_son(e, v);
                inserted = true;
            }
        }
        else
        {
            if(e->right) e = e->right;
            else
            {
                insert_as_right_son(e, v);
                inserted = true;
            }
        }
    }
}

int main()
{
    random_device random;
    mt19937 gen(random());
    srand(time(NULL));
    clock_t start;
    double t_build_tree = 0, t_search_tree = 0, t_del_tree = 0;

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
//    for (int i = 0; i < DLUGOSC_LISTY ; i++)
//    {
//        cout<<tab[i]<<" ";
//    }
//    cout<<endl;

    cout<<"GOTOWE"<<endl;
    //Budowanie drzewa
    start = clock();
    node * root = NULL;
    t_push_in_root(root, tab[0]);
    for(int i = 1; i<DLUGOSC_LISTY; i++)
    {
        t_insert_elem(root, tab[i]);
    }
    t_build_tree = (clock() - start) / (double)CLOCKS_PER_SEC;
    cout<<t_build_tree<<endl;


    //Szukanie elementów
    //print_tree(root);
    start = clock();
    for(int i = 1; i<DLUGOSC_LISTY; i++)
    {
        node * find;
        find = root;
        while(find->data != tab[i])
        {
            if(find->data > tab[i]) find = find->left;
            else find = find->right;
        }

    }
    t_search_tree = (clock() - start) / (double)CLOCKS_PER_SEC;
    cout<<t_search_tree<<endl;

    //Usuwanie drzewa
    start = clock();
    delete_tree(root);
    root = NULL;
    t_del_tree = (clock() - start) / (double)CLOCKS_PER_SEC;
    cout<<t_del_tree<<endl;
    //print_tree(root);
    return 0;
}

#include <iostream>
#include <ctime>
#include<cstdlib>
#include <fstream>

using namespace std;

#define NODES 100
#define SAT 0.3
#define MAX_WEIGHT 500
#define KROKI 15
#define PROBY 10

//wspólne funkcje
int minKey(int accessible_lines_values[], bool is_visited[], int size/*, int * min_length*/) {
	int min = MAX_WEIGHT+1, min_index=0;

	for (int v = 0; v < size; v++)
		if (!is_visited[v] && accessible_lines_values[v] < min && accessible_lines_values[v] != 0)
			min = accessible_lines_values[v], min_index = v;
//	if(min!=MAX_WEIGHT+1) *min_length += min;
	return min_index;
}
void printMST(int visited_nodes[], int size, int version) {
	if(version == 1) cout<<"matrix : ";
	else cout<<"list   : ";
	for (int i = 0; i < size; i++)
		cout << visited_nodes[i] << " ";
	cout<<endl;
}

//lista incydencji
struct neighbour{
	int index;
	int connection_prize;
	neighbour * next;
};
void show_incidence_list(neighbour ** main_list_tab, int size) {
	for(int i=0;i<size;i++)
	{
		cout<<i<<" : ";
		neighbour * first = main_list_tab[i];
		while(first)
		{
			cout<<first->index<<"("<<first->connection_prize<<") ";
			first = first->next;
		}
		cout<<endl;
	}
}
void add_neighbour(neighbour** pointer, int index, int prize){
	neighbour* node = new neighbour;
	node->index = index;
	node->connection_prize = prize;
	node->next = NULL;
	if(!*pointer) *pointer = node;
	else
	{
		neighbour * pointer1;
		pointer1 = *pointer;
		while(pointer1->next) pointer1 = pointer1->next;
		pointer1->next = node;
	}
}
void incidence_list_generator(int **tab, neighbour* main_list_tab[], int size) {
	for(int i=0; i<size; i++)
	{
		main_list_tab[i] = NULL;
		for(int node_index=0; node_index<size; node_index++)
			if(tab[i][node_index])
				add_neighbour(&main_list_tab[i], node_index, tab[i][node_index]);
	}
}
void incidence_list_MST(neighbour **tab, int size) {
//	int min_length = 0;
	int *visited_nodes = new int[size];  //odwiedzone wierzchołki
	int *accessible_lines_values = new int[size];  //wartości krawędzi
	bool *is_visited = new bool[size]; //czy już został policzony
	for (int i = 0; i < size; i++)
	{
		accessible_lines_values[i] = MAX_WEIGHT + 1;
		is_visited[i] = false;
	}

	accessible_lines_values[0] = 0;
	visited_nodes[0] = -1;

	for (int count = 0; count < size; count++)
	{
		//znajdowanie najtańszego dostępnego połączenia
		int u;
		if(count==0) u = 0;
		else u = minKey(accessible_lines_values, is_visited, size/*, &min_length*/);
		is_visited[u] = true;
		visited_nodes[count] = u;

		neighbour * node = tab[u];
		while(node) {  //jadę po wszystkich wierzchołkach
			if (!is_visited[node->index] && node->connection_prize < accessible_lines_values[node->index])
				accessible_lines_values[node->index] = node->connection_prize;
			node = node->next;
		}
	}
//	printMST(visited_nodes, size, 2);
//	cout<<min_length<<endl;
	delete accessible_lines_values;
	delete visited_nodes;
	delete is_visited;
}

//macierz sąsiedztwa
void show_table(int **tab, int size){
	for(int i=0;i<size;i++) {
		for (int j = 0; j < size; j++)
			cout << tab[i][j] << " ";
		cout<<endl;
	}
	cout<<endl;
}
void table_generator(int **tab, int size){
	for (int i = 0; i < size; i++)
		tab[i] = new int[size] {0};

    //generowanie grafu nieskierowanego
	int r_i, r_j, r, j, counter = int((size*(size - 1) / 2) * SAT + 1);
	for (j = 0; j < size; j++)
	{
		do
			r_j = rand() % size;
		while (r_j == j);
		r = rand() % MAX_WEIGHT + 1 ;
		tab[j][r_j] = r;
		tab[r_j][j] = r;
	}
	counter -= j;
	while(counter>0)
	{
		do
			r_i = rand() % size, r_j = rand() % size;
		while (r_i == r_j || tab[r_i][r_j] != 0);
		r = rand() % MAX_WEIGHT + 1;
		tab[r_i][r_j] = tab[r_j][r_i] = r;
		counter--;
	}
}
void matrix_MST(int **tab, int size) {
//	int min_length = 0;
	int *visited_nodes = new int[size];  //odwiedzone wierzchołki
	int *accessible_lines_values = new int[size];  //wartości krawędzi
	bool *is_visited = new bool[size]; //czy już został policzony
	for (int i = 0; i < size; i++)
	{
		accessible_lines_values[i] = MAX_WEIGHT + 1;
		is_visited[i] = false;
	}

	accessible_lines_values[0] = 0;
	visited_nodes[0] = -1;

	for (int count = 0; count < size; count++)
	{
		//znajdowanie najtańszego dostępnego połączenia
		int u;
		if(count==0) u = 0;
		else u = minKey(accessible_lines_values, is_visited, size/*, &min_length*/);
		is_visited[u] = true;
		visited_nodes[count] = u;

		for (int v = 0; v < size; v++)  //jadę po wszystkich wierzchołkach
			if (tab[u][v] && !is_visited[v] && tab[u][v] <  accessible_lines_values[v])
				accessible_lines_values[v] = tab[u][v];
	}
//	printMST(visited_nodes, size, 1);
//	cout<<min_length<<endl;
	delete accessible_lines_values;
	delete visited_nodes;
	delete is_visited;
}

int main()
{
	srand(time(NULL));
	clock_t start;
	ofstream out("wyniki1.txt");
	out << "size\tmatrix\ti.list\tsat = " << SAT << endl;

	for (int k = 1; k <= KROKI; k++)
	{
		double t_matrix = 0, t_incidence_list = 0;
        int size = NODES * k;

		for (int p = 0; p < PROBY; p++) {

			//macierz sąsiedztwa
            int **tab = new int*[size];
            table_generator(tab, size);
//            show_table(tab, size);

            start = clock();
			matrix_MST(tab, size);
            t_matrix += (clock() - start) / (double)CLOCKS_PER_SEC;


//			//lista incydencji
			neighbour *main_list_tab[size];
			incidence_list_generator(tab, main_list_tab, size);
//			show_incidence_list(main_list_tab, size);

			start = clock();
			incidence_list_MST(main_list_tab, size);
			t_incidence_list += (clock() - start) / (double)CLOCKS_PER_SEC;
			delete tab;
			delete main_list_tab;
		}
		t_matrix /= PROBY;
		t_incidence_list /= PROBY;
		cout << endl << size << "\t" << t_matrix <<"\t"<<t_incidence_list<< endl;
		out << size << "\t" << t_matrix <<"\t"<<t_incidence_list<< endl;
	}

	return 0;
}

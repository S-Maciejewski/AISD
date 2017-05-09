#include <iostream>
#include <cmath>
#include <ctime>
#include <random>
#include <stack>

#define nodes 4000
#define sat 0.3

using namespace std;

stack<int> usedNodes;    // stacki znacznie fajniejsze od tablic ;)
stack<int> result;
int m = int(floor(nodes*(nodes - 1)*sat / 2));
int resultTableSize = 0;
int ** tab;	//macierz grafu

int nextPath(int node) {
    for(int i = 0; i < nodes; i++)
    {
        if(tab[node][i]) return i;
    }
    return -1;
}
void findEuler() {
    int currentNode = 0;
    int nextNode;
    usedNodes.push(currentNode);
    while(!usedNodes.empty())
    {
        if(nextPath(currentNode) != -1)
        {
            nextNode = nextPath(currentNode);
            usedNodes.push(nextNode);
            tab[currentNode][nextNode]--; tab[nextNode][currentNode]--;
            currentNode = nextNode;
        }
        else
        {
            result.push(currentNode);
            usedNodes.pop();
            if(!usedNodes.empty()) currentNode = usedNodes.top();
        }
    }
}

void generate() {

	int arcCounter = 0;
	for (int i = 0; i < nodes - 1; i++)
	{
		tab[i][i + 1]++, tab[i + 1][i]++;
		arcCounter++;
	}

	tab[0][nodes - 1]++, tab[nodes - 1][0]++;
	arcCounter++;

	while (arcCounter <= m - 3) {
        int a = 0, b = 0, c = 0;
        a = rand() % nodes, b = rand() % nodes;
        while (tab[a][b] == 1 || a == b){
                a = rand() % nodes, b = rand() % nodes;
        }

        c = rand()%nodes;
		while( (c==a || c==b) || (tab[b][c] == 1 || tab[c][a] == 1) ) {
                c = rand() % nodes;
        }

		tab[a][b]++, tab[b][a]++;
		tab[b][c]++, tab[c][b]++;
		tab[c][a]++, tab[a][c]++;

		arcCounter += 3;
	}

    //zamiana wierzchołków
	for (int i = 0; i < nodes*2; i++)
	{
		int randNode1 = rand()%nodes;
		int randNode2 = rand()%nodes;
        while(randNode1 == randNode2){
            randNode2 = rand()%nodes;
        }
		for (int k = 0; k<nodes; k++) swap(tab[randNode1][k], tab[randNode2][k]);
		for (int j = 0; j<nodes; j++) swap(tab[j][randNode1], tab[j][randNode2]);
	}

    resultTableSize = arcCounter+1;  //zmienna resultTableSize jest zawsze o 1 większa
    // od arcCounter, więc żeby bez sensu nie zliczac jest obliczona tutaj, trochę to skraca czas działania
	cout << endl << "Graph generated successfully" << endl;
	cout << "Edge amount: " << arcCounter <<" "<<m<< endl << endl;
}

//void findHamilton(int v)
//{
//	stack[sptr++] = v;
//	for (int i = 0; i < nodes; i++)
//		if (tab[v][i])
//			findHamilton(i);
//	if (sptr == nodes && tab[v][0])	//zakladam, ze punktem poczatkowym jest 0
//	{
//		cout << "Cykl Hamiltona znaleziony" << endl;
//		return;
//	}
//	else
//		stack[--sptr] = -1;
//}

int main()
{
	time_t start;
    srand(time(NULL));   //dałem zwykłego randa, bo i tak nie wychodzimy poza 10000
	double eulerTime = 0/*, hamiltonTime = 0*/;

	tab = new int *[nodes];
	for (int i = 0; i < nodes; i++)
	{
		tab[i] = new int[nodes];
		for (int j = 0; j < nodes; j++) 
			tab[i][j] = 0;
	}
		     
	generate();
//    wypisywanie tabeli
//	for (int i = 0; i < nodes; i++)
//	{
//		for (int j = 0; j < nodes; j++)
//		{
//			cout << tab[i][j] << ", ";
//		}
//		cout << endl;
//	}
//    cout<<endl;
//    wypisywanie krawędzi
//    for (int i = 0; i < nodes; i++)
//    {
//        cout<<i<<" : ";
//        for (int j = 0; j < nodes; j++)
//        {
//            if(tab[i][j]) cout<<j<<" ";
//        }
//        cout << endl;
//    }

	start = clock();
	findEuler();
	eulerTime = (clock() - start) / (double)CLOCKS_PER_SEC;

// 	  wypisywanie cyklu (w kolejności odwrotnej jakby co, ale cykl to cykl)
//    cout << "Cykl Eulera : ";
//    while(!result.empty())
//    {
//        cout<<result.top()<<", ";
//        result.pop();
//    }
    cout << endl << "Czas dzialania eulera: " << eulerTime << endl;

//    findHamilton(0);
    
	delete[] tab;


	return 0;
}

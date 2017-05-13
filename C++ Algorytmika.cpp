#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <random>
#include <stack>
#include <fstream>

#define nodes 50
#define sat 0.3
#define steps 15
#define step 10
#define repeats 5

using namespace std;

stack<int> usedNodes;
stack<int> resultEuler;
stack<int> resultHamilton;
int edges;
int *visited;
bool hamiltonFound = false;

int ** eulerGraph;
int ** hamiltonGraph;

void setGlobal(int nodeAmount)
{
	edges = (nodeAmount*(nodeAmount - 1)*sat / 2);
	//visited = new int[nodeAmount]{ false };
	visited = new int[nodeAmount];
	for (int i = 0; i < nodeAmount; i++)
		visited[i] = false;
	cout << endl << "Globals set on : " << nodeAmount << " nodes and : " << edges << " edges." << endl;
}

void generate() {
	cout << "Generating graph... ";

	int arcCounter = 0;
	for (int i = 0; i < nodes - 1; i++)
	{
		eulerGraph[i][i + 1]++, eulerGraph[i + 1][i]++;
		arcCounter++;
	}

	eulerGraph[0][nodes - 1]++, eulerGraph[nodes - 1][0]++;
	arcCounter++;

	cout << "... ";

	while (arcCounter <= edges - 3)
	{
		int a, b, c, loops, loopBreaker;
		bool generationError = true;
		while (generationError)
		{
			a = 0, b = 0, c = 0, loops = 0, loopBreaker = 0;
			generationError = false;

			a = rand() % nodes, b = rand() % nodes;

			while (eulerGraph[a][b] == 1 || a == b) {
				if (loops > nodes)
				{
					a = loopBreaker++;
					for (int j = 0; j < nodes; j++)
					{
						b = j;
						if (eulerGraph[a][b] != 1 && a != b) break;
					}
				}
				else
				{
					a = rand() % nodes, b = rand() % nodes;
					loops++;
				}
				//cout << "a, b generated" << endl;
			}

			c = rand() % nodes;
			while ((c == a || c == b) || (eulerGraph[b][c] == 1 || eulerGraph[c][a] == 1))
			{
				if (loops > nodes)
				{
					c = loopBreaker++;
					if (loopBreaker >= nodes)
					{
						generationError = true;
						//cout << "				GENERATION ERROR" << endl;
						break;
					}
				}
				else
				{
					c = rand() % nodes;
					loops++;
				}
				//cout << "c generated" << endl;
			}
		}

		eulerGraph[a][b]++, eulerGraph[b][a]++;
		eulerGraph[b][c]++, eulerGraph[c][b]++;
		eulerGraph[c][a]++, eulerGraph[a][c]++;

		arcCounter += 3;
	}
	cout << "... ";

	for (int i = 0; i < nodes * nodes; i++)
	{
		int randNode1 = rand() % nodes;
		int randNode2 = rand() % nodes;
		while (randNode1 == randNode2) {
			randNode2 = rand() % nodes;
		}
		for (int k = 0; k<nodes; k++) swap(eulerGraph[randNode1][k], eulerGraph[randNode2][k]);
		for (int j = 0; j<nodes; j++) swap(eulerGraph[j][randNode1], eulerGraph[j][randNode2]);
	}

	cout << "...graph generated successfully" << endl;
	//cout << "Edge amount: " << arcCounter << " " << edges << endl << endl;
}

int nextPath(int node) {
	for (int i = 0; i < nodes; i++)
	{
		if (eulerGraph[node][i]) return i;
	}
	return -1;
}

void findEuler() {
	int currentNode = 0;
	int nextNode;
	usedNodes.push(currentNode);
	while (!usedNodes.empty())
	{
		if (nextPath(currentNode) != -1)
		{
			nextNode = nextPath(currentNode);
			usedNodes.push(nextNode);
			eulerGraph[currentNode][nextNode]--; eulerGraph[nextNode][currentNode]--;
			currentNode = nextNode;
		}
		else
		{
			resultEuler.push(currentNode);
			usedNodes.pop();
			if (!usedNodes.empty()) currentNode = usedNodes.top();
		}
	}
}

void findHamilton(int v) {
	visited[v] = true;
	resultHamilton.push(v);
	for (int i = 0; i < nodes; i++)
		if (hamiltonGraph[v][i] && !visited[i]) {
			hamiltonGraph[v][i]--; hamiltonGraph[i][v]--;
			findHamilton(i);
			if (hamiltonFound) return;
			hamiltonGraph[v][i]++; hamiltonGraph[i][v]++;
		}

	if (resultHamilton.size() == nodes && hamiltonGraph[v][0]) {
		resultHamilton.push(0);
		hamiltonFound = true;
		return;
	}
	else {
		resultHamilton.pop();
		visited[v] = false;
	}
}

void printGraph(int nodeAmount) {
	for (int i = 0; i < nodeAmount; i++)
	{
		for (int j = 0; j < nodeAmount; j++)
			cout << eulerGraph[i][j] << ", ";
		cout << endl;
	}
}

int main()
{
	time_t start;
	srand(time(NULL));
	double eulerTime = 0, hamiltonTime = 0;
	ofstream out("wyniki.txt");
	out << "nodes, eulerTime, hamiltonTime" << endl;

	for (int stepNumber = 0; stepNumber <= steps; stepNumber++)
	{
		int nodesActually = nodes + stepNumber*step;

		setGlobal(nodesActually);
		for (int repeatNumber = 0; repeatNumber < repeats; repeatNumber++)
		{
			eulerGraph = new int *[nodesActually];
			for (int i = 0; i < nodesActually; i++)
			{
				eulerGraph[i] = new int[nodesActually];
				for (int j = 0; j < nodesActually; j++)
					eulerGraph[i][j] = 0;
			}

			generate();

			//printGraph(nodesActually);

			hamiltonGraph = new int *[nodesActually];
			for (int i = 0; i < nodesActually; i++)
			{
				hamiltonGraph[i] = new int[nodesActually];
				for (int j = 0; j < nodesActually; j++)
					hamiltonGraph[i][j] = eulerGraph[i][j];
			}


			cout << "Searching for Euler" << endl;
			start = clock();
			findEuler();
			eulerTime = ((clock() - start) / (double)CLOCKS_PER_SEC);

			cout << "Searching for Hamilton"<<endl;
			start = clock();
			findHamilton(0);
			hamiltonTime = (clock() - start) / (double)CLOCKS_PER_SEC;

			delete eulerGraph;
			delete hamiltonGraph;
		}

		eulerTime /= (double)repeats;
		hamiltonTime /= (double)repeats;
		cout << endl << "Euler time for " << nodesActually << " nodes : " << eulerTime << endl;
		cout << "Hamilton time for " << nodesActually << " nodes : " << hamiltonTime << endl;

		out << nodesActually << ", " << eulerTime << ", " << hamiltonTime << endl;
	}

	return 0;
}
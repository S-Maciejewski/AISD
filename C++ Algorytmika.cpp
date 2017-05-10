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

void setGlobal(int stp)
{
	edges = int(floor(nodes + step*stp*(nodes + step*stp - 1)*sat / 2));
	visited = new int[nodes + step*stp]{ false };
}

void generate() {

	int arcCounter = 0;
	for (int i = 0; i < nodes - 1; i++)
	{
		eulerGraph[i][i + 1]++, eulerGraph[i + 1][i]++;
		arcCounter++;
	}

	eulerGraph[0][nodes - 1]++, eulerGraph[nodes - 1][0]++;
	arcCounter++;

	while (arcCounter <= edges - 3) {
		int a = 0, b = 0, c = 0;
		a = rand() % nodes, b = rand() % nodes;
		while (eulerGraph[a][b] == 1 || a == b) {
			a = rand() % nodes, b = rand() % nodes;
		}

		c = rand() % nodes;
		while ((c == a || c == b) || (eulerGraph[b][c] == 1 || eulerGraph[c][a] == 1)) {
			c = rand() % nodes;
		}

		eulerGraph[a][b]++, eulerGraph[b][a]++;
		eulerGraph[b][c]++, eulerGraph[c][b]++;
		eulerGraph[c][a]++, eulerGraph[a][c]++;

		arcCounter += 3;
	}


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

	//cout << endl << "Graph generated successfully" << endl;
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

int main()
{
	time_t start;
	srand(time(NULL));
	double eulerTime = 0, hamiltonTime = 0;
	short int eulerMultiplier = 10, hamiltonMultiplier = 2;
	ofstream out ("wyniki.txt");
	out << "nodes, eulerTime, hamiltonTime" << endl;

	for (int stepNumber = 0; stepNumber < steps; stepNumber++)
	{
		setGlobal(stepNumber);
		for (int repeatNumber = 0; repeatNumber < repeats; repeatNumber++)
		{
			eulerGraph = new int *[nodes*+stepNumber*step];
			for (int i = 0; i < nodes*+stepNumber*step; i++)
			{
				eulerGraph[i] = new int[nodes*+stepNumber*step];
				for (int j = 0; j < nodes*+stepNumber*step; j++)
					eulerGraph[i][j] = 0;
			}

			generate();

			hamiltonGraph = new int *[nodes*+stepNumber*step];
			for (int i = 0; i < nodes*+stepNumber*step; i++)
			{
				hamiltonGraph[i] = new int[nodes*+stepNumber*step];
				for (int j = 0; j < nodes*+stepNumber*step; j++)
					hamiltonGraph[i][j] = eulerGraph[i][j];
			}


			start = clock();
			for(short int i=0;i<eulerMultiplier;i++)
				findEuler();
			eulerTime = ((clock() - start) / (double)CLOCKS_PER_SEC) / (double)eulerMultiplier;

			start = clock();
			for (short int i = 0; i<hamiltonMultiplier; i++)
				findHamilton(0);
			hamiltonTime = (clock() - start) / (double)CLOCKS_PER_SEC / (double)hamiltonMultiplier;

			delete[] eulerGraph;
			delete[] hamiltonGraph;
		}

		eulerTime /= repeats;
		hamiltonTime /= repeats;
		cout << endl << "Euler time for " << nodes*+stepNumber*step << " nodes : " << eulerTime << endl;
		cout << endl << "Hamilton time for " << nodes*+stepNumber*step << " nodes : " << hamiltonTime << endl;

		out << nodes*+stepNumber*step << ", " << eulerTime << ", " << hamiltonTime << endl;
	}

	return 0;
}
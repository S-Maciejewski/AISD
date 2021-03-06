﻿#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <stack>
#include <fstream>

#define nodes 33
#define sat 0.3
#define steps 1
#define step 1
#define repeats 1

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
	hamiltonFound = false;
}

void generate(int nodesActually) {
	int arcCounter = 0;
	for (int i = 0; i < nodesActually - 1; i++)
	{
		eulerGraph[i][i + 1]++, eulerGraph[i + 1][i]++;
		arcCounter++;
	}

	eulerGraph[0][nodesActually - 1]++, eulerGraph[nodesActually - 1][0]++;
	arcCounter++;


	while (arcCounter <= edges - 3)
	{
		int a, b, c, loops, loopBreaker;
		bool generationError = true;
		while (generationError)
		{
			a = 0, b = 0, c = 0, loops = 0, loopBreaker = 0;
			generationError = false;

			a = rand() % nodesActually, b = rand() % nodesActually;

			while (eulerGraph[a][b] == 1 || a == b) {
				if (loops > nodesActually)
				{
					a = loopBreaker++;
					for (int j = 0; j < nodesActually; j++)
					{
						b = j;
						if (eulerGraph[a][b] != 1 && a != b) break;
					}
				}
				else
				{
					a = rand() % nodesActually, b = rand() % nodesActually;
					loops++;
				}
			}

			c = rand() % nodesActually;
			while ((c == a || c == b) || (eulerGraph[b][c] == 1 || eulerGraph[c][a] == 1))
			{
				if (loops > nodesActually)
				{
					c = loopBreaker++;
					if (loopBreaker >= nodesActually)
					{
						generationError = true;
						break;
					}
				}
				else
				{
					c = rand() % nodesActually;
					loops++;
				}
			}
		}

		eulerGraph[a][b]++, eulerGraph[b][a]++;
		eulerGraph[b][c]++, eulerGraph[c][b]++;
		eulerGraph[c][a]++, eulerGraph[a][c]++;

		arcCounter += 3;
	}

	for (int i = 0; i < nodesActually; i++)
	{
		int randNode1 = rand() % nodesActually;
		int randNode2 = rand() % nodesActually;
		while (randNode1 == randNode2)
			randNode2 = rand() % nodesActually;
		for (int k = 0; k<nodesActually; k++) swap(eulerGraph[randNode1][k], eulerGraph[randNode2][k]);
		for (int j = 0; j<nodesActually; j++) swap(eulerGraph[j][randNode1], eulerGraph[j][randNode2]);
	}
	cout << "Graph generated" << endl;
}

int nextPath(int node, int nodesActually) {
	for (int i = 0; i < nodesActually; i++)
	{
		if (eulerGraph[node][i]) return i;
	}
	return -1;
}

void findEuler(int nodesActually) {
	int currentNode = 0;
	int nextNode;
	usedNodes.push(currentNode);
	while (!usedNodes.empty())
	{
		if (nextPath(currentNode, nodesActually) != -1)
		{
			nextNode = nextPath(currentNode, nodesActually);
			usedNodes.push(nextNode);
			//eulerGraph[currentNode][nextNode]--; eulerGraph[nextNode][currentNode]--;
			eulerGraph[currentNode][nextNode]=0; eulerGraph[nextNode][currentNode]=0;
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

void findHamilton(int v, int nodesActually) {
	visited[v] = true;
	resultHamilton.push(v);
	if (resultHamilton.size()<nodesActually)
		for (int i = 0; i < nodesActually; i++)
			if (hamiltonGraph[v][i] && !visited[i])
			{
				hamiltonGraph[v][i]--; hamiltonGraph[i][v]--;
				findHamilton(i, nodesActually);

				if (hamiltonFound) return;
				hamiltonGraph[v][i]++; hamiltonGraph[i][v]++;
			}

	if (resultHamilton.size() == nodesActually && hamiltonGraph[v][0]) {
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

void readGraph(int nodeAmount)
{
	ifstream in("euler.txt");
	for (int i = 0; i < nodeAmount; i++)
	{
		for (int j = 0; j < nodeAmount; j++)
		{
			in>>eulerGraph[i][j];
		}
	}

}


int main()
{
	//time_t start;
	srand(time(NULL));
	ofstream out("wyniki.txt");
	//out << "nodes, eulerTime, hamiltonTime" << endl;
	
	int nodesActually = 6;
	//double eulerTime = 0, hamiltonTime = 0;

	srand(time(NULL));
	setGlobal(nodesActually);
	//eulerTime = 0, hamiltonTime = 0;
	eulerGraph = new int *[nodesActually];
	for (int i = 0; i < nodesActually; i++)
	{
		eulerGraph[i] = new int[nodesActually];
		for (int j = 0; j < nodesActually; j++)
			eulerGraph[i][j] = 0;
	}

	//generate(nodesActually);
	readGraph(nodesActually);

	printGraph(nodesActually);

	hamiltonGraph = new int *[nodesActually];
	for (int i = 0; i < nodesActually; i++)
	{
		visited[i] = false;
		hamiltonGraph[i] = new int[nodesActually];
		for (int j = 0; j < nodesActually; j++)
			hamiltonGraph[i][j] = eulerGraph[i][j];
	}


	//cout << "Searching for Euler" << endl;
	//start = clock();
	findEuler(nodesActually);
	/*while (!resultEuler.empty())
	{
		cout << resultEuler.top() + 1 << " ";
		resultEuler.pop();
	}*/
	//12 krawędzi
	cout << "Cykl Eulera: " << endl;
	int result[12];
	for (int i = 0; !resultEuler.empty(); i++)
	{
		cout << resultEuler.top() + 1 << " ";
		result[i] = resultEuler.top() + 1;
		resultEuler.pop();
	}
	cout << endl;
	for (int i = 0; i < 11; i++)
	{
		cout << "(" << result[i] << ", " << result[i + 1] << "), ";
	}
	cout << endl;

	//eulerTime += (clock() - start) / (double)CLOCKS_PER_SEC;

	hamiltonFound = false;
	//cout << "Searching for Hamilton" << endl;
	//start = clock();
	findHamilton(0, nodesActually);
	//hamiltonTime += (clock() - start) / (double)CLOCKS_PER_SEC;

	for (int i = 0; i < nodesActually; i++)
	{
		delete eulerGraph[i];
		delete hamiltonGraph[i];
	}

	while (!usedNodes.empty()) usedNodes.pop();
	while (!resultEuler.empty()) resultEuler.pop();
	while (!resultHamilton.empty()) resultHamilton.pop();
	for (int i = 0; i < nodesActually; i++) visited[i] = false;


	delete eulerGraph;
	delete hamiltonGraph;



	return 0;
}
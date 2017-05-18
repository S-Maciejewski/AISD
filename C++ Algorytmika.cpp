#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <random>
#include <stack>
#include <fstream>

#define CAPACITY 6000
#define ITEM_NUMBER 5000
#define PROBA 20

using namespace std;
stack <int> greedy_results;
stack <int> dynamic_results;
struct item {
	int value;
	int weight;
	double ratio;
};

void merge_sort(item * in_tab, int start, int size, item *help_tab) {
	int middle = ((start + size) / 2) - 1;
	if ((middle - start) > 0) merge_sort(in_tab, start, middle + 1, help_tab);
	if ((size - middle) > 2) merge_sort(in_tab, middle + 1, size, help_tab);
	int i = start;
	int j = middle + 1;

	for (int k = start; k < size; k++)
	{
		if (((i <= middle) && (j >= size)) || (((i <= middle) && (j < size)) && (in_tab[i].ratio >= in_tab[j].ratio)))
			help_tab[k] = in_tab[i++];

		else help_tab[k] = in_tab[j++];
	}

	for (int k = start; k < size; k++)
		in_tab[k] = help_tab[k];
}

void print_tab(item * tab, int item_number) {
	for (int i = 0; i<item_number; i++)
	{
		cout << tab[i].value << " " << tab[i].weight << " " << tab[i].ratio << endl;
	}
}


void sort(item * tab, item * sorted_tab, int item_number) {
	for (int i = 0; i<item_number; i++) {
		sorted_tab[i].weight = tab[i].weight;
		sorted_tab[i].value = tab[i].value;
		sorted_tab[i].ratio = tab[i].ratio;
	}
	item * help_tab = new item[item_number];
	merge_sort(sorted_tab, 0, item_number, help_tab);
	////wyświetlanie elementów posortowanych względem stosunku wartości do wagi
	//        cout<<endl<<"Posortowana : "<<endl;
	//        print_tab(sorted_tab, item_number);
	delete[] help_tab;
}

void print_items(item * tab, int item_number) {
	for (int i = 0; i<item_number; i++)
	{
		cout << "Item " << i << " : " << tab[i].value << " " << tab[i].weight << endl;
	}
}


void print_proper_greedy_items(item * sorted_tab, int max_value) {

	//odwracanie stosu wynikowego
	int stack_size = greedy_results.size();
	int * reverse_tab = new int[stack_size];
	int counter = stack_size - 1;
	while (!greedy_results.empty()) {
		reverse_tab[counter] = greedy_results.top();
		greedy_results.pop();
		counter--;
	}

	//wyświetlanie spakowanych elementów i maksymalnej spakowanej wartości
	cout << endl << "Items by greedy algorithm" << endl;
	for (int i = 0; i<stack_size; i++)
		cout << "(" << sorted_tab[reverse_tab[i]].value << " " << sorted_tab[reverse_tab[i]].weight << ")" << endl;
	cout << "Max value by greedy algorithm : " << max_value << endl;

	delete[] reverse_tab;
}

void print_proper_dynamic_items(item * tab, int max_value) {

	//odwracanie stosu wynikowego
	int stack_size = dynamic_results.size();
	int * reverse_tab = new int[stack_size];
	int counter = stack_size - 1;
	while (!dynamic_results.empty()) {
		reverse_tab[counter] = dynamic_results.top();
		dynamic_results.pop();
		counter--;
	}

	//wyświetlanie spakowanych elementów i maksymalnej spakowanej wartości
	cout << endl << "Items by dynamic algorithm" << endl;
	for (int k = 0; k<stack_size; k++)
		cout << "(" << tab[reverse_tab[k]].value << " " << tab[reverse_tab[k]].weight << ")" << endl;
	cout << "Max value by dynamic algorithm : " << max_value << endl;

	delete[] reverse_tab;
}

void generate(item * tab, int item_number, int capacity) {

	//super chytry algos na dopasowywanie wielkości wag elementów
	int weight_range;
	if (item_number < 0.5*capacity) weight_range = (capacity / item_number) * 5;
	else weight_range = 10;

	//mało sprytny algos na dopasowywanie wielkości wartości elementów
	int value_range = item_number;

	//losowanie wartości
	int weight_sum = 0;
	double value, weight;
	for (int i = 0; i<item_number; i++)
	{
		tab[i].value = (rand() % value_range) + 1;
		tab[i].weight = (rand() % weight_range) + 1;
		value = tab[i].value;
		weight = tab[i].weight;
		tab[i].ratio = value / weight;
		weight_sum += tab[i].weight;
	}

	//jak suma wag elementów jest mniejsza niż pojemnośc plecaka, to losuje od nowa
	if (weight_sum <= capacity) generate(tab, item_number, capacity);
}

int greedy_algorithm(item * sorted_tab, int item_number, int capacity) {

	//algorytm zachłanny
	int max_value = 0;
	int weight_left = capacity;
	for (int i = 0; i<item_number; i++) {
		if (sorted_tab[i].weight <= weight_left) {
			weight_left -= sorted_tab[i].weight;
			greedy_results.push(i);
			max_value += sorted_tab[i].value;
		}
		if (weight_left <= 0) return max_value;
	}

	return max_value;
}

int dynamic_algorithm(item * tab, int item_number, int capacity) {

	//tworzenie dynamicznej macierzy
	int ** matrix = new int *[item_number + 1];
	for (int i = 0; i <= item_number; i++)
		matrix[i] = new int[capacity + 1];

	//zerowanie zerowej kolumny i zerowego wiersza
	for (int i = 0; i<capacity + 1; i++) matrix[0][i] = 0;
	for (int i = 0; i<item_number + 1; i++) matrix[i][0] = 0;

	//dynamiczny algorytm plecakowy
	for (int i = 1; i <= item_number; i++) {
		for (int j = 1; j <= capacity; j++) {
			if (tab[i - 1].weight > j) matrix[i][j] = matrix[i - 1][j];
			else if (tab[i - 1].value + matrix[i - 1][j - tab[i - 1].weight] > matrix[i - 1][j]) matrix[i][j] = tab[i - 1].value + matrix[i - 1][j - tab[i - 1].weight];
			else matrix[i][j] = matrix[i - 1][j];
		}
	}

	//wyświetlanie macierzy dynamicznej
	//    cout<<endl<<"Dynamic matrix"<<endl;
	//    for(int i=0; i<item_number+1; i++){
	//        for(int j=0; j<capacity+1; j++){
	//            cout<<matrix[i][j]<<" ";
	//        }
	//        cout<<endl;
	//    }

	//odczytywanie właściwych itemów z macierzy
	int i = item_number, j = capacity;
	while (matrix[i][j] != 0)
	{
		while (matrix[i][j] == matrix[i - 1][j]) i--;
		while (matrix[i][j] == matrix[i][j - 1]) j--;
		dynamic_results.push(i - 1);
		j -= tab[i - 1].weight;
		i--;
	}

	int max_value = matrix[item_number][capacity];

	//usuwanie macierzy
	for (int k = item_number; k>0; k--)
		delete[] matrix[k];
	delete[] matrix;

	return max_value;
}

int main() {
	srand(time(NULL));
	time_t start;
	ofstream out("wyniki.txt");
	out << "capacity, greedyTime, dynamicTime, error, items : 5000"<<endl;

	for (int i = 0; i < 15; i++)
	{
		int capacity = CAPACITY + i*1000;
		int item_number = ITEM_NUMBER;
		int proba = PROBA;

		int max_value_greedy, max_value_dynamic;
		double errors_sum = 0;
		double dynamic_time = 0, greedy_time = 0;

		for (int i = 0; i<proba; i++) {

			item * tab = new item[item_number];
			generate(tab, item_number, capacity);


			//mierzenie czasu dla algorytmu zachłannego
			start = clock();
			item * sorted_tab = new item[item_number];
			sort(tab, sorted_tab, item_number);
			max_value_greedy = greedy_algorithm(sorted_tab, item_number, capacity);
			greedy_time += (clock() - start) / (double)CLOCKS_PER_SEC;


			//mierzenie czasu dla algorytmu dynamicznego
			start = clock();
			max_value_dynamic = dynamic_algorithm(tab, item_number, capacity);
			dynamic_time += (clock() - start) / (double)CLOCKS_PER_SEC;


			double relative_error = (max_value_dynamic - max_value_greedy) / double(max_value_dynamic);
			errors_sum += relative_error;

			delete[] sorted_tab;
			delete[] tab;
		}
		greedy_time /= (double)proba;
		dynamic_time /= (double)proba;
		errors_sum /= (double)proba;

		//cout << "Item number : " << item_number;
		cout << "Capacity : " << capacity;
		cout << endl << "Greedy time : " << greedy_time << endl;
		cout << endl << "Dynamic time : " << dynamic_time << endl;
		cout << endl << "Relative error : " << errors_sum << endl;
		

		out << capacity << ", " << greedy_time << ", " << dynamic_time << ", " << errors_sum << endl;
	}
	
	return 0;
}
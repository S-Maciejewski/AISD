#include <iostream>
#include <ctime>
#include <random>
#include <stack>

#define CAPACITY 1000
#define ITEM_NUMBER 2000

using namespace std;
stack <int> greedy_results;
stack <int> dynamic_results;
struct item{
    int value;
    int weight;
};
void print_items(item ** tab, int item_number){
    for(int i=0; i<item_number; i++)
    {
        cout<<"Item "<<i<<" : "<<tab[i]->value<<" "<<tab[i]->weight<<endl;
    }
}

void generate(item ** tab, int item_number, int capacity){
    int weight_range = 20
    /*(capacity/item_number)*3*/;

    int value_range = item_number;
    int weight_sum = 0;
    for(int i=0; i<item_number; i++)
    {
        tab[i]->value = (rand()%value_range) + 1;
        tab[i]->weight = (rand()%weight_range) + 1;
        weight_sum += tab[i]->weight;
    }

    cout<<"Draw successfully ended"<<endl<<"Weight sum = "<<weight_sum<<endl;
    if(weight_sum <= capacity) generate(tab, item_number, capacity);
}

int greedy_algorithm(item ** tab, int item_number, int capacity){
    int max_value = 0;
    int weight_left = capacity;
    for(int i=0; i<item_number; i++){
        if(tab[i]->weight <= weight_left){
            weight_left -= tab[i]->weight;
            greedy_results.push(i);
            max_value += tab[i]->value;
        }
        if(weight_left <= 0) return max_value;
    }
    return max_value;
}

int dynamic_algorithm(item ** tab, int item_number, int capacity){
    int ** matrix = new int *[item_number+1];
    for(int i=0; i<=item_number; i++)
    {
        matrix[i] = new int[capacity+1];
    }

    //zerowanie zerowej kolumny i zerowego wiersza
    for(int i=0; i<capacity+1; i++) matrix[0][i] = 0;
    for(int i=0; i<item_number+1; i++) matrix[i][0] = 0;

    //dynamiczny algorytm plecakowy
    for(int i=1; i<=item_number; i++){
        for(int j=1; j<=capacity; j++){
            if(tab[i-1]->weight > j) matrix[i][j] = matrix[i-1][j];
            else if(tab[i-1]->value + matrix[i-1][j-tab[i-1]->weight] > matrix[i-1][j]) matrix[i][j] = tab[i-1]->value + matrix[i-1][j-tab[i-1]->weight];
            else matrix[i][j] = matrix[i-1][j];
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

    //odczytywanie właściwych itemów
    int i=item_number, j=capacity;
    while(matrix[i][j]!=0)
    {
        while(matrix[i][j] == matrix[i-1][j]) i--;
        while(matrix[i][j] == matrix[i][j-1]) j--;
        dynamic_results.push(i-1);
        j-= tab[i-1]->weight;
        i--;
    }

    int max = matrix[item_number][capacity];
    for(int i=item_number; i>0; i--)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
    return max;
}

int main() {
    srand(time(NULL));
    int capacity = CAPACITY;
    int item_number = ITEM_NUMBER;
    int proba = 1000;
    int max_value_greedy, max_value_dynamic;
    struct item ** tab = new item* [item_number];
    for(int i=0; i<item_number; i++)
        tab[i] = new item;
    double errors_sum = 0;
    for(int i=0; i<proba; i++) {
        generate(tab, item_number, capacity);

//    print_items(tab, item_number);

        max_value_greedy = greedy_algorithm(tab, item_number, capacity);
//    cout<<"Items by greedy algorithm"<<endl;
//    while(!greedy_results.empty())
//    {
//        cout<<greedy_results.top()<<" ";
//        greedy_results.pop();
//    }
//    cout<<endl;
        //cout<<"Max value by greedy algorithm : "<<max_value_greedy<<endl;

        max_value_dynamic = dynamic_algorithm(tab, item_number, capacity);
//    cout<<endl<<"Items by dynamic algorithm"<<endl;
//    while(!dynamic_results.empty())
//    {
//        cout<<dynamic_results.top()<<" ";
//        dynamic_results.pop();
//    }
//    cout<<endl;
        //cout<<"Max value by dynamic algorithm : "<<max_value_dynamic<<endl;

        double relative_error = (max_value_dynamic - max_value_greedy) / double(max_value_dynamic);
        errors_sum += relative_error;
    }
    cout<<endl<<"Relative error : "<<errors_sum / 1000<<endl;
    return 0;
}

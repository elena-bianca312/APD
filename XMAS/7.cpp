#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <sstream>
#include <algorithm>

using namespace std;

int main() {

    string text;
    vector<int> numbers;
    int average1 = 0, average2;

    ifstream readFile("7.txt");

    getline(readFile, text);
    stringstream ss(text);
    for (int i; ss >> i;) {
        numbers.push_back(i); 
        average1 += i;  
        if (ss.peek() == ',')
            ss.ignore();
    }

    int size;
    int cost1 = 0, cost2 = 0;

    auto middle = numbers.begin() + (numbers.end() - numbers.begin())/2;
    nth_element(numbers.begin(), middle, numbers.end());                    // INTERESANT :D, nu stiam de fct asta
    int median = *middle;

    average1 /= numbers.size();
    average2 = average1 + 1;

    for (int i = 0; i < numbers.size(); i++) {

        // Part 1
        // int cost += abs(numbers[i] - median);

        // Part 2
        int dif1 = abs(numbers[i] - average1);
        int dif2 = abs(numbers[i] - average2);
        cost1 += dif1 * (dif1 + 1) / 2;
        cost2 += dif2 * (dif2 + 1) / 2;
    }

    // Part 2
    int cost    = cost1 < cost2 ? cost1 : cost2;
    int average = cost1 < cost2 ? average1 : average2;

    // cout << median << " " << average << endl;
    cout << cost << endl;

    readFile.close();

    return 0;
}

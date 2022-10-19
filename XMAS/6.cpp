#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <sstream>

#define DAYS 256

using namespace std;

int main() {

    // Explicit version

    string text;
    vector<int> numbers;

    ifstream readFile("6.txt");

    getline(readFile, text);

    stringstream ss(text);

    for (int i; ss >> i;) {
        numbers.push_back(i);   
        if (ss.peek() == ',')
            ss.ignore();
    }

    int days = DAYS;
    int size;

    for (int i = 0; i < days; i++) {

        size = numbers.size();
        for (int i = 0; i < size; i++) {
            if (numbers[i] == 0) {
                numbers[i] = 6 + 1;
                numbers.push_back(8);
            }
            numbers[i]--;
        }
    }


    cout << numbers.size() << endl;

    readFile.close();

    return 0;
}

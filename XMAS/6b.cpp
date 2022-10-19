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

    string text;
    vector<long> numbers;   // vector de aparitii

    ifstream readFile("6.txt");

    getline(readFile, text);

    for (int i = 0; i < 9; i++) {
        numbers.push_back(0);
    }

    stringstream ss(text);

    for (int i; ss >> i;) {
        numbers[i]++;   
        if (ss.peek() == ',')
            ss.ignore();
    }

    int days = DAYS;

    for (int i = 0; i < days; i++) {

        long newFish = numbers[0];  // all the fish with remaining life = 0 will produce new fish
        for (int j = 0; j < 8; j++) {
            numbers[j] = numbers[j + 1];
        }
        numbers[6] += newFish;      // the fish that had lifespan = 0 now have a lifespan of 6
        numbers[8] = newFish;       // all the new fish added will have lifespan = 8
    }

    long res = 0;
    for (int i = 0; i < 9; i++) {
        res += numbers[i];
    }
    cout << res << endl;

    readFile.close();

    return 0;
}

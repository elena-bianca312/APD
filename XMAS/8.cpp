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
    int count = 0;

    for (int i = 0; i < 7 + 1; i++) {
        numbers.push_back(0);
    }

    ifstream readFile("8.txt");

    while(getline(readFile, text)) {
        string line = text.substr(text.find("|") + 2, text.length());
        // cout << line << endl;
        stringstream ss(line);
        for (string i; ss >> i;) {
            numbers[i.length()]++;  
            if (ss.peek() == ' ')
                ss.ignore();
        }
    }

    for (int i = 0; i < 7 + 1; i++) {
        if (i == 2 || i == 3 || i == 4 || i == 7) {
            count += numbers[i];
        }
        // cout << numbers[i] << " ";
    }
    // cout << endl;

    cout << count << endl;

    readFile.close();

    return 0;
}

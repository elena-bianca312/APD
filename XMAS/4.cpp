#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <math.h>
#include <sstream>

using namespace std;

int main() {

    string text;
    int contor = 0;
    vector<int> numbers;
    vector<vector<vector<int>>> matrices;

    ifstream readFile("4.txt");

    getline(readFile, text);

    stringstream ss(text);

    for (int i; ss >> i;) {
        numbers.push_back(i);    
        if (ss.peek() == ',')
            ss.ignore();
    }

    for (auto i : numbers)
        cout << i << " ";
    cout << endl;

    vector<int> line;
    vector<vector<int>> matrix;

    // Compute input
    while(getline(readFile, text)) {

        stringstream ss(text);

        for (int i; ss >> i;) {
            line.push_back(i);    
            if (ss.peek() == ' ')
                ss.ignore();
        }
        
        if (contor < 6) {
            matrix.push_back(line);
        }
       

        if (contor == 5) {
            matrices.push_back(matrix);
        }

        if (contor == 6) {
            matrix.clear();
            contor = 0;
        }

        contor++;

        line.clear();
    }


    for (auto i : matrices) {
        for (auto j : i) {
            for (auto k : j) {
                cout << k << " ";
            }
            cout << endl;
        }
        cout << endl;
    }


    // Bingo logic
    

    readFile.close();

    return 0;
}

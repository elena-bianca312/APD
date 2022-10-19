#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <math.h>

using namespace std;

vector<vector<int>> keepNumbers (vector<vector<int>> array, int position, int x, int length) {

    vector<vector<int>> newArray;

    for (int i = 0; i < length; i++) {
        if (array[i][position] == x) {
            newArray.push_back(array[i]);
        }
    }

    return newArray;
}

int computeMajority (vector<vector<int>> array, int position, int length) {

    int zeros = 0;
    for (int i = 0; i < length; i++) {
        if (array[i][position] == 0) {
            zeros++;
        }
    }
    if (zeros > length - zeros) {
        return 0;
    } else {
        return 1;
    }
} 

int main() {

    string text;
    int contor = 0;
    vector<vector<int>> array;
    vector<int> values;
    int wordLength;
    int number1 = 0, number2 = 0;

    ifstream readFile("3.txt");

    while(getline(readFile, text)) {

        wordLength = text.length();
        contor++;
        for (int i = 0; i < text.length(); i++) {
            if (text[i] == '0') {
                values.push_back(0);
            } else {
                values.push_back(1);
            }
        }
        array.push_back(values);
        values.clear();
    }


    // COMPUTE OXYGEN VALUE
    vector<vector<int>> oxygen = array;
    vector<vector<int>> oxygenCopy;
    int contorCopy = contor;

    for (int i = 0; i < wordLength && oxygen.size() > 1; i++) {

        // More 1s
        if (computeMajority(oxygen, i, contorCopy) == 1) {
            oxygenCopy = keepNumbers(oxygen, i, 1, contorCopy);
        } 
        // Mores 0s
        else {
            oxygenCopy = keepNumbers(oxygen, i, 0, contorCopy);
        }
        contorCopy = oxygenCopy.size();

        oxygen = oxygenCopy;
    }

    for (int i = 0; i < wordLength; i++) {
        // cout << oxygen[0][i];
        if (oxygen[0][i] == 1)
            number1 += pow(2, wordLength - i - 1);
    }
    // cout << endl;
    // cout << number1 << endl;


    // COMPUTE CO2 VALUE
    vector<vector<int>> co2 = array;
    vector<vector<int>> co2Copy;
    contorCopy = contor;

    for (int i = 0; i < wordLength && co2.size() > 1; i++) {

        // More 1s
        if (computeMajority(co2, i, contorCopy) == 1) {
            co2Copy = keepNumbers(co2, i, 0, contorCopy);
        } 
        // Mores 0s
        else {
            co2Copy = keepNumbers(co2, i, 1, contorCopy);
        }
        contorCopy = co2Copy.size();

        co2 = co2Copy;
    }

    for (int i = 0; i < wordLength; i++) {
        // cout << co2[0][i];
        if (co2[0][i] == 1)
            number2 += pow(2, wordLength - i - 1);
    }
    // cout << endl;
    // cout << number2 << endl;

    cout << number1 * number2 << endl;

    readFile.close();

    return 0;
}

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <math.h>

using namespace std;

int main() {

    string text;
    int number1 = 0, number2 = 0, contor = 1;
    vector<int> countZeros;

    ifstream readFile("3.txt");

    getline(readFile, text);

    for (int i = 0; i < text.length(); i++) {
        if (text[i] == '0') {
            countZeros.push_back(1);
        }
        else {
            countZeros.push_back(0);
        }
    }

    while(getline(readFile, text)) {

        contor++;
        for (int i = 0; i < text.length(); i++) {
            if (text[i] == '0') {
                countZeros[i]++;;
            }
        }
    }

    for (int i = 0; i < countZeros.size(); i++) {

        // cout << countZeros[i] << " " << contor - countZeros[i] <<  endl;

        // Compute gamma rate (more 1s)
        if (countZeros[i] < contor - countZeros[i]) {
            number1 += pow(2, countZeros.size() - i - 1);
        } 
        // Compute epsilon rate 
        else {
            number2 += pow(2, countZeros.size() - i - 1);
        }
    }

    // cout << number1 << " " << number2 << endl;
    cout << number1 * number2 << endl;

    readFile.close();

    return 0;
}

#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

int main() {

    string text;
    int number, counter = 0;
    int sumA = 0, sumB = 0, sumC = 0;
    int A, B, C;
    int i = 0;

    ifstream readFile("1.txt");


    while(getline(readFile, text)) {

        switch (i % 3)
        {
        case 0:
            A = sumA;
            sumA = 0;
            if (A > C) {
                counter++;
            }
            break;
        
        case 1:
            B = sumB;
            sumB = 0;
            if (B > A) {
                counter++;
            }
            break;
        
        case 2:
            C = sumC;
            sumC = 0;
            if (C > B) {
                counter++;
            }
            break;

        default:
            break;
        }

        int number = stoi(text);
        sumA += number;
        if (i > 0)
            sumB += number;
        if (i > 1)
            sumC += number;

        i++;
    }

    cout << counter << endl;

    readFile.close();

    return 0;
}

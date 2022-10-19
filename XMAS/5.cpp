#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <math.h>
#include <cstdlib>

#define SIZE 1000

using namespace std;

int main() {

    string text;
    int contor = 0;
    int x1, x2, y1, y2;

    int **matrix = (int **) calloc(SIZE, sizeof(int *));
    for (int i = 0; i < SIZE; i++) {
        matrix[i] = (int *) calloc(SIZE, sizeof(int));
    }

    int min, max;

    ifstream readFile("5.txt");

    while(getline(readFile, text)) {

        x1 = stoi(text.substr(0, text.find(",")));
        y1 = stoi(text.substr(text.find(",") + 1, text.find("-")));
        x2 = stoi(text.substr(text.find(">") + 2, text.find_last_of(",")));
        y2 = stoi(text.substr(text.find_last_of(",") + 1, text.length()));

        // Horizontal and vertical
        if (x1 == x2) {
            if (y1 > y2) {
                max = y1;
                min = y2;
            } else {
                max = y2;
                min = y1;
            }
            for (int i = min; i <= max; i++) {
                matrix[i][x1]++;
            }
        } else if (y1 == y2) {
            if (x1 > x2) {
                max = x1;
                min = x2;
            } else {
                max = x2;
                min = x1;
            }
            for (int i = min; i <= max; i++) {
                matrix[y1][i]++;
            }
        }

        // Diagonals
        else if (x1 > x2 && y1 > y2) {
            while (x1 >= x2) {
                matrix[y1][x1] += 1;
                x1--;
                y1--;
            }
        } else if (x1 > x2 && y1 < y2) {
            while (x1 >= x2) {
                matrix[y1][x1] += 1;
                x1--;
                y1++;
            }
        } else if (x1 < x2 && y1 > y2) {
            while (x1 <= x2) {
                matrix[y1][x1] += 1;
                x1++;
                y1--;
            }
        } else if (x1 < x2 && y1 < y2) {
            while (x1 <= x2) {
                matrix[y1][x1] += 1;
                x1++;
                y1++;
            }
        }
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (matrix[i][j] >= 2) {
                contor++;
            }
        }
    }

    cout << contor << endl;

    readFile.close();

    return 0;
}

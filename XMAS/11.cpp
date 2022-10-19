#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <stack>

#define len     10
#define height  10

// 1683
//  788

using namespace std;

void print_matrix(int **matrix) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < len; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int increment(int i, int j, int **matrix, bool flash) {

    if (i == -1 || j == -1 || i == height || j == len) {
        return 0;
    }

    if (flash && matrix[i][j] == 0)
        return 0;

    if (matrix[i][j] == 9) {
        matrix[i][j] = 0;
        return 1;
    } else {
        matrix[i][j]++;
    }

    return 0;
}

int flash (int **matrix, int **set_flash) {
    
    int count = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < len; j++) {
            count += increment(i, j, matrix, false);
        }
    }

    int countCopy = -1;
    do {
        countCopy = count;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < len; j++) {
                if (matrix[i][j] == 0 && set_flash[i][j] == 0) {
                    set_flash[i][j] = 1;
                    // increment neighbours
                    count += increment(i - 1, j - 1, matrix, true);
                    count += increment(i - 1, j,     matrix, true);
                    count += increment(i - 1, j + 1, matrix, true);
                    count += increment(i    , j - 1, matrix, true);
                    count += increment(i    , j + 1, matrix, true);
                    count += increment(i + 1, j - 1, matrix, true);
                    count += increment(i + 1, j    , matrix, true);
                    count += increment(i + 1, j + 1, matrix, true);
                }
            }
        }
    } while (countCopy != count);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < len; j++) {
            set_flash[i][j] = 0;
        }
    }

    return count;
    
}

int main() {

    string text;
    int count = 0;
    int line = 0;

    int **matrix     = (int **) calloc(height, sizeof(int *));
    int **set_flash  = (int **) calloc(height, sizeof(int *));
    for (int i = 0; i < height; i++) {
        matrix[i]     = (int *) calloc(len, sizeof(int));
        set_flash[i]  = (int *) calloc(len, sizeof(int));
    }

    ifstream readFile("11.txt");

    bool ok = 1;
    while(getline(readFile, text)) {
        for (int i = 0; i < text.length(); i++) {
            matrix[line][i] = text[i] - '0';
        }
        line++;
    }

    int part = 2;

    // Part 1
    if (part == 1) {
        for (int i = 0; i < 100; i++) {
            count += flash(matrix, set_flash);
            // print_matrix(matrix);
        }

        cout << count << endl;
    }
    
    // Part 2
    if (part == 2) {
        int step = 1;
        while (flash(matrix, set_flash) != len * height) {
            step++;
        }
        cout << step << endl;
    }
   
    readFile.close();

    return 0;
}

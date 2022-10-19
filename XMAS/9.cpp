#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <sstream>
#include <algorithm>

#define len     100
#define height  100

// 508

using namespace std;

int recursive_bfs(int i, int j, int **matrix, int **basins) {

    // Past the edge of the matrix => stop
    if (i== -1 || j == -1 || i == height || j == len) {
        return 0;
    }

    // If matrix value = 9 or value is already assigned to a basin => stop
    if (matrix[i][j] == 9 || basins[i][j] == 1) {
        return 0;
    }

    // Mark element as visited
    basins[i][j] = 1;

    // Recursive search for all 4 neighbours in all directions
    // Will eventually return the size of all connected matrix elements
    // aka the size of the basin
    return 1 + recursive_bfs(i + 1, j, matrix, basins) +
               recursive_bfs(i - 1, j, matrix, basins) +
               recursive_bfs(i, j - 1, matrix, basins) +
               recursive_bfs(i, j + 1, matrix, basins);
}

int main() {

    string text;
    vector<pair<int, int>> numbers;
    int count = 0;

    int **matrix     = (int **) malloc(height * sizeof(int *));
    int **set_basins = (int **) malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++) {
        matrix[i]     = (int *) malloc(len * sizeof(int));
        set_basins[i] = (int *) malloc(len * sizeof(int));
    }

    ifstream readFile("9.txt");

    int line = 0;
    while(getline(readFile, text)) {
       
        for (int i = 0; i < text.length(); i++) {
            matrix[line][i] = text[i] - '0';
        }
        line++;
    }

    // Find low points in map
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < len; j++) {

            bool okUp = 1, okDown = 1, okLeft = 1, okRight = 1;

            // Check up - down
            if (i == 0) {
                if (matrix[i + 1][j] <= matrix[i][j])
                    okDown = 0;
            } else if (i == height - 1) {
                if (matrix[i - 1][j] <= matrix[i][j])
                    okUp = 0;
            } else {
                if (matrix[i + 1][j] <= matrix[i][j])
                    okDown = 0;
                if (matrix[i - 1][j] <= matrix[i][j])
                    okUp = 0;    
            }

            // Check left - right
            if (j == 0) {
                if (matrix[i][j + 1] <= matrix[i][j])
                    okRight = 0;
            } else if (j == len - 1) {
                if (matrix[i][j - 1] <= matrix[i][j])
                    okLeft = 0;
            } else {
                if (matrix[i][j + 1] <= matrix[i][j])
                    okRight = 0;
                if (matrix[i][j - 1] <= matrix[i][j])
                    okLeft = 0;    
            }

            if (okUp && okDown && okLeft && okRight) {
                // cout << matrix[i][j] << " ";
                count += matrix[i][j] + 1;
                numbers.push_back(std::make_pair(i, j));
            }
        }
    }


    // Find basins
    vector<int> basins;
    for (auto i : numbers) {
        basins.push_back(recursive_bfs(i.first, i.second, matrix, set_basins));
    }

    sort(basins.begin(), basins.end(), std::greater<int>());
    for (auto i : basins) {
        // cout << i << endl;
    }

    // cout << count << endl;
    cout << basins[0] * basins[1] * basins[2] << endl;

    readFile.close();

    return 0;
}

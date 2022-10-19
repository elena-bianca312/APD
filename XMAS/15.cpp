#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <stack>
#include <map>

#define len     10
#define height  10

using namespace std;

bool comp(int a, int b)
{
    return (a < b);
}

int findPath(int startX, int startY, int endX, int endY, int **matrix, int **visited, int sum, vector<int> vec) {

    // Found the end of the road
    if (startX == endX && startY == endY) {
        int s = 0;
        cout << "bingo" << endl;
        for (auto i : vec) {
            cout << i << " ";
            s += i;
        }
        cout << "S = " << s << endl;
        cout << "end bingo" << endl;
        return 0;
    }

    // Exploration goes past matrix grid
    if (startX < 0 || startX > endX || startY < 0 || startY > endY) {
        return INT16_MAX;
    }

    // Can't go back
    if (visited[startX][startY] == 1) {
        return INT16_MAX;
    }
    vec.push_back(sum);

    visited[startX][startY] = 1;

    // cout << min({
    //        findPath(startX - 1, startY - 1, endX, endY, matrix, visited) +
    //        findPath(startX - 1, startY,     endX, endY, matrix, visited) +
    //        findPath(startX - 1, startY + 1, endX, endY, matrix, visited) +
    //        findPath(startX    , startY - 1, endX, endY, matrix, visited) +
    //        findPath(startX    , startY + 1, endX, endY, matrix, visited) +
    //        findPath(startX + 1, startY - 1, endX, endY, matrix, visited) +
    //        findPath(startX + 1, startY,     endX, endY, matrix, visited) +
    //        findPath(startX + 1, startY + 1, endX, endY, matrix, visited)}, comp) << " ";

    cout << matrix[startX][startY] << " ";

    return matrix[startX][startY] + min({
           findPath(startX - 1, startY - 1, endX, endY, matrix, visited, matrix[startX][startY], vec) +
           findPath(startX - 1, startY,     endX, endY, matrix, visited, matrix[startX][startY], vec) +
           findPath(startX - 1, startY + 1, endX, endY, matrix, visited, matrix[startX][startY], vec) +
           findPath(startX    , startY - 1, endX, endY, matrix, visited, matrix[startX][startY], vec) +
           findPath(startX    , startY + 1, endX, endY, matrix, visited, matrix[startX][startY], vec) +
           findPath(startX + 1, startY - 1, endX, endY, matrix, visited, matrix[startX][startY], vec) +
           findPath(startX + 1, startY,     endX, endY, matrix, visited, matrix[startX][startY], vec) +
           findPath(startX + 1, startY + 1, endX, endY, matrix, visited, matrix[startX][startY], vec)}, comp);
}

int main() {

    string text;
    int count = 0;
    int line = 0;

    int **matrix     = (int **) calloc(height, sizeof(int *));
    int **visited    = (int **) calloc(height, sizeof(int *));
    for (int i = 0; i < height; i++) {
        matrix[i]     = (int *) calloc(len, sizeof(int));
        visited[i]    = (int *) calloc(len, sizeof(int));
    }

    ifstream readFile("15.txt");

    while(getline(readFile, text)) {
        
        // cout << text << endl;
        for(int i = 0; i < text.length(); i++) {
            matrix[count][i] = text[i] - '0';
        }
        count++;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < len; j++) {
            cout << matrix[i][j];
        }
        cout << endl;
    }

    
    vector<int> vec;
    cout << findPath(0, 0, 9, 9, matrix, visited, 0, vec) << endl;


   
    readFile.close();

    return 0;
}

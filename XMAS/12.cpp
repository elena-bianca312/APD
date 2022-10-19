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

using namespace std;

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

    ifstream readFile("12.txt");

    bool ok = 1;
    while(getline(readFile, text)) {
        
        cout << text << endl;
    }

    
   
    readFile.close();

    return 0;
}

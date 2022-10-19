#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <stack>

using namespace std;

int main() {

    string text;
    vector<int> numbers;
    stack<char> st;
    int count = 0;

    ifstream readFile("10.txt");

    bool ok = 1;
    while(getline(readFile, text)) {

        ok = 1;
        for (int i = 0; i < text.length() && ok == 1; i++) {
            if (text[i] == '(') {
                st.push(text[i]);
            }
            if (text[i] == '[') {
                st.push(text[i]);
            }
            if (text[i] == '{') {
                st.push(text[i]);
            }
            if (text[i] == '<') {
                st.push(text[i]);
            }
            if (text[i] == ')') {
                if (st.top() != '(') {
                    ok = 0;
                    count += 3;
                }
                    
                st.pop();
            }
            if (text[i] == ']') {
                if (st.top() != '[') {
                    ok = 0;
                    count += 57;
                }
                    
                st.pop();
            }
            if (text[i] == '}') {
                if (st.top() != '{') {
                    ok = 0;
                    count += 1197;
                }
                    
                st.pop();
            }
            if (text[i] == '>') {
                if (st.top() != '<') {
                    ok = 0;
                    count += 25137;
                }
                    
                st.pop();
            }
        }
    }

    cout << count << endl;
    readFile.close();

    return 0;
}

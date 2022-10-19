#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <stack>

// 3969823589

using namespace std;

int main() {

    string text;
    vector<long> numbers;
    stack<char> st;
    long count = 0;

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
                }
                    
                st.pop();
            }
            if (text[i] == ']') {
                if (st.top() != '[') {
                    ok = 0;
                }
                    
                st.pop();
            }
            if (text[i] == '}') {
                if (st.top() != '{') {
                    ok = 0;
                }
                    
                st.pop();
            }
            if (text[i] == '>') {
                if (st.top() != '<') {
                    ok = 0;
                }
                st.pop();
            }
        }

        if (ok) {
            count = 0;
            while (!st.empty()) {
                count *= 5;
                if (st.top() == '(') {
                    count += 1;
                }
                if (st.top() == '[') {
                    count += 2;
                }
                if (st.top() == '{') {
                    count += 3;
                }
                if (st.top() == '<') {
                    count += 4;
                }
                st.pop();
            }
            numbers.push_back(count);
        }

        while (!st.empty()) {
            st.pop();
        }
    }

    sort(numbers.begin(), numbers.end());

    cout << numbers[numbers.size() / 2] << endl;
    
    readFile.close();

    return 0;
}

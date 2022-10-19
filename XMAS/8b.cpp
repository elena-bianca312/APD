#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <sstream>
#include <algorithm>

/*

Facem urmatoarea mapare a segmentelor:

                   0
            --------------
            -            -
          1 -            -  2
            -      3     -
            --------------
            -            -
          4 -            -  5
            -            -
            --------------
                   6

Daca urmarim de cate ori apare fiecare segment in scrierea tuturor
numerelor de la 0 la 9 o singura data, vom obtine urmatoarea mapare:

    -    0: 8 / 10
    -    1: 6 / 10  -> nr. unic
    -    2: 8 / 10
    -    3: 7 / 10
    -    4: 4 / 10  -> nr. unic
    -    5: 9 / 10  -> nr. unic
    -    6: 7 / 10

Astfel ordinea de deductie a simbolurilor va fi urmatoarea:
    5   ->   2   ->   0  ->  1  ->  4  ->  3  ->  6.

Avand in vedere cifrele usor identificabile:
    1 se identifica prin 2, 5
    4 se identifica prin 1, 2, 3, 5
    7 se identifica prin 0, 2, 5
    8 se identifica prin 0, 1, 2, 3, 4, 5

Maparea cifrelor se reconfigureaza odata cu fiecare linie.

Dupa mapare, trebuie sa vedem caror cifre se potrivesc configuratiile fiecarui output:
Le vom imparti pe categorii, in functie de dimensiune:
    2 (cifra = 1)
    3 (cifra = 7)
    4 (cifra = 4)
    5 (cifra = 2 / 3 / 5)
    6 (cifra = 0 / 6 / 9)
    7 (cifra = 8)

Si vom tine cont de simbolurile care trebuie sa se regaseasca in scrierea fiecarei cifre:
    0 : 0, 1, 2, 4, 5, 6 
    1 : 2, 5
    2 : 0, 2, 3, 4, 6
    3 : 0, 2, 3, 5, 6
    4 : 1, 2, 3, 5
    5 : 0, 1, 3, 5, 6
    6 : 0, 1, 3, 4, 5, 6
    7 : 0, 2, 5
    8 : 0, 1, 2, 3, 4, 5, 6

*/

using namespace std;

int main() {

    string text;
    vector<int> numbers;
    vector<char> mapping;
    vector<int> output;
    int count = 0;

    for (int i = 0; i < 7; i++) {
        numbers.push_back(0);
        mapping.push_back('0');
    }

    ifstream readFile("8.txt");

    // getline(readFile, text);

    while(getline(readFile, text)) {
        string line1 = text.substr(0, text.find("|"));
        string line2 = text.substr(text.find("|") + 2, text.length());

        // Numaram de cate ori apare fiecare segment
        // Cautam cifrele unice: 1, 4, 7, 8
        string one, four, seven, eight;
        stringstream ss(line1);
        for (string i; ss >> i;) {
            for (int j = 0; j < i.length(); j++) {
                numbers[i[j] - 'a']++;
            }  
            if (i.length() == 2) {
                one = i;
            }
            if (i.length() == 3) {
                seven = i;
            }
            if (i.length() == 4) {
                four = i;
            } 
            if (i.length() == 7) {
                eight = i;
            }       
            if (ss.peek() == ' ')
                ss.ignore();
        }

        // Vedem maparea pentru segmentele unice
        for (int i = 0; i < 7; i++) {
            if (numbers[i] == 4) 
                mapping[4] = i + 'a';
            if (numbers[i] == 6)
                mapping[1] = i + 'a';
            if (numbers[i] == 9)
                mapping[5] = i + 'a';
            // cout << numbers[i] << " ";
        }
        // cout << endl;
        // Finished mapping for 1, 4, 5

        if (one[0] == mapping[5]) {
            mapping[2] = one[1];
        } else {
            mapping[2] = one[0];
        }
        // Finished mapping for 1, 2, 4, 5

        for (int i = 0; i < 3; i++) {
            if (seven[i] != mapping[2] && seven[i] != mapping[5]) {
                mapping[0] = seven[i];
                break;
            }
        }
        // Finished mapping for 0, 1, 2, 4, 5

        for (int i = 0; i < 4; i++) {
            if (four[i] != mapping[1] && four[i] != mapping[2] && four[i] != mapping[5]) {
                mapping[3] = four[i];
                break;
            }
        } 
        // Finished mapping for 0, 1, 2, 3, 4, 5 

        for (int i = 0; i < 7; i++) {
            if (numbers[i] == 7 && ((i + 'a') != mapping[3])) {
                mapping[6] = i + 'a';
                break;
            }
        }
        // Finished mapping for 0, 1, 2, 3, 4, 5, 6

        // for (int i = 0; i < 7; i++) {
        //     cout << i << " " << mapping[i] << endl;
        // }

        // Interpret output, use mapping as reference
        stringstream ss1(line2);
        for (string i; ss1 >> i;) {
            if (i.length() == 2) {
                output.push_back(1);
            } else if (i.length() == 3) {
                output.push_back(7);
            } else if (i.length() == 4) {
                output.push_back(4);
            } else if (i.length() == 7) {
                output.push_back(8);
            } else if (i.length() == 5) {
                bool ok = 0;
                // Searching for 5
                for (int j = 0; j < i.length() && ok == 0; j++) { 
                    if (i[j] == mapping[1]) {
                        ok = 1;
                    }
                }
                if (ok)
                    output.push_back(5);
                else {
                    bool ok = 0;
                    // Searching for 3
                    for (int j = 0; j < i.length() && ok == 0; j++) { 
                        if (i[j] == mapping[5]) {
                            ok = 1;
                        }
                    }
                    if (ok)
                        output.push_back(3);
                    else
                        output.push_back(2);
                }
            } else if (i.length() == 6) {
                bool ok = 0;
                // Searching for 0
                for (int j = 0; j < i.length() && ok == 0; j++) { 
                    if (i[j] == mapping[3]) {
                        ok = 1;
                    }
                }
                if (!ok)
                    output.push_back(0);
                else {
                    bool ok = 0;
                    // Searching for 6
                    for (int j = 0; j < i.length() && ok == 0; j++) { 
                        if (i[j] == mapping[4]) {
                            ok = 1;
                        }
                    }
                    if (ok)
                        output.push_back(6);
                    else
                        output.push_back(9);
                }
            } 
            if (ss1.peek() == ' ')
                ss1.ignore();
        }

        for (int i = 0; i < output.size(); i++) {
            count += output[i] * pow(10, output.size() - i - 1);
            // cout << output[i];
        }
        for (int i = 0; i < 7; i++) {
            numbers[i] = 0;
        }
        output.clear();
        // cout << endl;
    }

    cout << count << endl;

    readFile.close();

    return 0;
}

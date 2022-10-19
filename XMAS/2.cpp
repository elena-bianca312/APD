#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

int main() {

    string text;
    int number, horizontal = 0, depth = 0, aim = 0;
    int i = 0;

    ifstream readFile("2.txt");

    string command;

    while(getline(readFile, text)) {

        command = text.substr(0, text.find(" "));
        number = stoi(text.substr(text.find(" ") + 1, text.length()));
        
        if (command.compare("forward") == 0) {
            horizontal += number;
            depth += aim * number;
        }
        if (command.compare("up") == 0) {
            aim -= number;
        }
        if (command.compare("down") == 0) {
            aim += number;
        }
    }

    cout << horizontal * depth << endl;

    readFile.close();

    return 0;
}

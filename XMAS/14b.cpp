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

using namespace std;

pair<int, vector<char>> mostFrequent(string text) {
    vector<char> characters;
    int max = 0;
    int count = 0;
    char maxCharacter;
    for (char q = ' '; q <= '~'; q++) {
        count = 0;
        for (int i = 0; i < text.length(); i++) {
            if(text[i] == q)
                count++;
        }
        
        if(count > max) {
            max = count;
            maxCharacter = q;
        }

        if (count > 0) {
            characters.push_back(q);
        }
    }
    
    return make_pair(max, characters);
}

pair<int, char> leastFrequent(string text, vector<char> chars) {
    int min = INT16_MAX;
    int count = 0;
    char minCharacter;
    for(auto q : chars) {

        count = 0;
        for (int i = 0; i < text.length(); i++) {
            if(text[i] == q)
                count++;
        }
        
        if (count < min) {
            min = count;
            minCharacter = q;
        }
    }
    
    return make_pair(min, minCharacter);
}

int main() {

    string text;
    int count = 0;
    int line = 0;

    vector<vector<char>> rules;
    map<char, int> mappings;

    ifstream readFile("14.txt");
    getline(readFile, text);
    string polymer = text;
    cout << polymer << endl;
    getline(readFile, text);

    bool ok = 1;
    while(getline(readFile, text)) {
        
        // cout << text << endl;
        vector<char> rule;
        for (int i = 0; i < text.length(); i++) {
            if(isalpha(text[i])) {
                rule.push_back(text[i]);
            }
        }
        rules.push_back(rule);
        rule.clear();
    }

    string newPolymer = "";

    int steps = 10;

    for (int j = 0; j < steps; j++) {
        for (int i = 0; i < polymer.length() - 1; i++) {
            string copy = polymer;
            // Substring at position i and of length 2
            string substr = copy.substr(i, 2);
            newPolymer.push_back(substr[0]);
            // if (mappings.find(substr[0]) != mappings.end()) {
            //     mappings[substr[0]]++;
            // } else {
            //     mappings[substr[0]] = 0;
            // }
            for (auto rule : rules) {
                if (rule.at(0) == substr[0] && rule.at(1) == substr[1]) {
                    newPolymer.push_back(rule.at(2));
                    // if (mappings.find(rule.at(2)) != mappings.end()) {
                    //     mappings[rule.at(2)]++;
                    // } else {
                    //     mappings[rule.at(2)] = 0;
                    // }
                } 
            }
            if (i == polymer.length() - 2) {
                newPolymer.push_back(substr[1]);
                // if (mappings.find(substr[1]) != mappings.end()) {
                //     mappings[substr[1]]++;
                // } else {
                //     mappings[substr[1]] = 0;
                // }                
            }
        }
        polymer = newPolymer;
        newPolymer = "";
    }
    
    pair<int, vector<char>> most  = mostFrequent(polymer);
    pair<int, char> least = leastFrequent(polymer, most.second);
    cout << most.first << " " << least.first << " " << most.first - least.first << endl;
    cout << polymer << endl;
   
    readFile.close();

    return 0;
}

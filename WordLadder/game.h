#pragma once
#include <iostream>
#include <fstream>
#include <exception>
#include <string>

#include "Graph.h"
#include <queue>
#include <set>
using std::vector;
using std::map;
using std::queue;
using std::set;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::cin;
using std::ostream;

class Game
{
private:
    bool alive = true;
    string command;
    map<size_t, vector<string>> wordsByLength;
    vector<string> dictionary;
    map<size_t, Graph<string>> graphs;
public:
    vector<string> readDictionary(const string& filename);
    Graph<string> createGraph(const vector<string>& dictionary);
    vector<string> ShortestPath(Graph<string>& graph, const string& start, const string& target);
    map<size_t, Graph<string>> initGraph(vector<string>& dictionary, map<size_t, vector<string>>& wordsByLength);

    void AutomaticGamemode();
    void PlayingGamemode();
    void AnalyticsMode();
    void DisplayGamemodes() {
        string a = "";
        a += "1) Automatic Mode.\n";
        a += "2) Plaing Mode.\n";
        a += "3) Analytics Mode.\n";
        cout << a;
    };
    void Start() {
        cout << "------Please Wait!!!------" << endl;
        dictionary = readDictionary("dictionary.txt");
        graphs = initGraph(dictionary, wordsByLength);
        cout << "------Done loading!!------" << endl;
        cout << endl;
        DisplayGamemodes();
        cout << endl;
        command = "";
        while (command != "-1") {
            cout << "Select gamemode: " << endl;
            cin >> command;
            if (command == "1") {
                AutomaticGamemode();
            }
            else if (command == "2") {
                PlayingGamemode();
            }
            else if (command == "3") {
                AnalyticsMode();
            }
            else if (command != "-1")
            {
                cout << "Invalid command.\n";
            }
        }
    };
};
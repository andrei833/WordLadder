#include <iostream>
#include <fstream>
#include <exception>
#include <string>

#include "game.h"
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

int main() {

    Game g;
    g.Start();
    /*map<size_t, vector<string>> wordsByLength;
    vector<string> dictionary = readDictionary("dictionary.txt");
    map<size_t, Graph<string>> graphs = initGraph(dictionary, wordsByLength) ;
    cout << "------Done loading!!!------" << std::endl;

    string alive = "ok";
    string start;
    string target;
    int max = 0;
    while (alive != "-1") {
        int nr;
        do {
            nr = rand() % 29 + 1;
            
        } while (nr < 4);
        auto words = wordsByLength[nr];
        if (!words.empty()) { 
            int randWordPos1 = rand() % words.size();
            start = words[randWordPos1];
            int randWordPos2 = rand() % words.size();
            target = words[randWordPos2];
            auto list = ShortestPath(graphs[nr], start, target);
            if (!list.empty()) {
                cout << "Start text:" << list.front()<<std::endl;
                cout << "End text:" << list.back() << std::endl;
                string current = list.front();
                string next;
                while (current != "-1") {
                    std::cin >> next;
                    auto neighbors = graphs[nr].getNeighbors(current);
                    if (next == "-1") {
                        current = "-1";
                    }
                    else if (next == list.front())
                    {
                        cout << "You won";
                        current = "-1";
                    }
                    else for (auto test : neighbors) {
                        if (next == test) {
                            current = next;
                        }
                        else
                        {
                            cout << "Invalid word" << std::endl;
                        }
                    }
                }
            }
        }
    }*/
    /*string start = "same";
    string target = "apor";

    if (start.size() != target.size()) {
        return 1;
    }

    auto it = graphs.find(start.size());
    if (it != graphs.end()) {
        Graph<string>& graph = it->second;
        vector<string> result = ShortestPath(graph,start,target);
        for (auto elems : result) { std::cout << elems << " "; }
    }*/

    

    return 0;
}

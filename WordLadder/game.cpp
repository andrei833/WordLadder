#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include <chrono>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <random>

bool isValidTransformation(const string& from, const string& to) {
    if (from.size() != to.size()) return false;
    int diffCount = 0;
    for (size_t i = 0; i < from.size(); ++i) {
        if (from[i] != to[i]) {
            diffCount++;
            if (diffCount > 1) return false;
        }
    }
    return diffCount == 1;
}

string getRandomWord(const vector<string>& words) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, words.size() - 1);
    return words[dis(gen)];
}


vector<string> Game::readDictionary(const string& filename) {
    ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Could not open dictionary file");
    }

    vector<string> dictionary;
    string word;
    while (file >> word) {
        dictionary.push_back(word);
    }
    return dictionary;
}

Graph<string> Game::createGraph(const vector<string>& dictionary) {
    Graph<string> graph;
    map<string, vector<string>> wildcardMap;

    for (const auto& word : dictionary) {
        graph.addNode(word);
        for (size_t i = 0; i < word.length(); ++i) {
            string wildcard = word;
            wildcard[i] = '*';
            wildcardMap[wildcard].push_back(word);
        }
    }

    for (const auto& entry : wildcardMap) {
        const auto& words = entry.second;
        for (size_t i = 0; i < words.size(); ++i)
            for (size_t j = i + 1; j < words.size(); ++j)
            {
                graph.addEdge(words[i], words[j]);
            }
    }
    return graph;
}

vector<string> Game::ShortestPath(Graph<string>& graph, const string& start, const string& target) {

    if (!graph.hasNode(start) || !graph.hasNode(target)) {
        throw std::invalid_argument("Start or target word not in dictionary");
    }
    queue<vector<string>> toVisit;
    set<string> visited;

    toVisit.push({ start });
    visited.insert(start);

    while (!toVisit.empty())
    {
        auto path = toVisit.front();
        toVisit.pop();
        string currentWord = path.back();

        for (auto& neightbor : graph.getNeighbors(currentWord)) {
            if (visited.find(neightbor) == visited.end()) {

                vector<string> newPath = path;
                newPath.push_back(neightbor);
                if (neightbor == target) {
                    return newPath;
                }
                toVisit.push(newPath);
                visited.insert(neightbor);
            }
        }
    }
    return {};
}

map<size_t, Graph<string>> Game::initGraph(vector<string>& dictionary, map<size_t, vector<string>>& wordsByLength) {
    for (const auto& word : dictionary) {
        wordsByLength[word.size()].push_back(word);
    }

    map<size_t, Graph<string>> graphs;
    for (auto& pair : wordsByLength) {
        size_t length = pair.first;
        const vector<string>& wordsOfLength = pair.second;
        graphs[length] = createGraph(wordsOfLength);
    }
    return graphs;
}

void Game::AutomaticGamemode() {
    string start;
    string target;
    
    string c = "Y";
    while (c == "Y") {
        cout << "Enter a starting word: ";
        cin >> start;
        cout << "Enter a target word: ";
        cin >> target;
        try
        {
            if (start.length() != target.length()) {
                throw std::invalid_argument("Words have different lenghts.");
            }
            auto list = ShortestPath(graphs[start.length()], start, target);
            for (auto it : list) {
                cout << it << " ";
            }
            cout << endl;
        }
        catch (std::exception& e)
        {
            cout << e.what();
        }
        cout << "Again (Y)? "<< endl;
        cin >> c;
    }
};

void Game::PlayingGamemode() {
    string userName;
    int nrOfLetters = 0;
    cout << "Enter your name: ";
    cin >> userName;
    cout << "Enter number of letters: ";
    while (nrOfLetters < 1 || nrOfLetters > 29) {
        cin >> nrOfLetters;
        if (nrOfLetters < 1 || nrOfLetters > 29) {
            cout << "Please choose a number between 1 and 29: ";
        }
    }

    const auto& wordsOfLength = wordsByLength[nrOfLetters];
    if (wordsOfLength.size() < 2) {
        cout << "Not enough words of the specified length in the dictionary." << endl;
        return;
    }

    string start = getRandomWord(wordsOfLength);
    string target = getRandomWord(wordsOfLength);
    while (target == start) {
        target = getRandomWord(wordsOfLength);
    }

    cout << "Starting word: " << start << endl;
    cout << "Target word: " << target << endl;

    Graph<string>& graph = graphs[nrOfLetters];
    vector<string> optimalPath = ShortestPath(graph, start, target);
    vector<string> userPath = { start };
    set<string> usedWords = { start };
    int hintsUsed = 0;
    string currentWord = start;

    while (currentWord != target) {
        string nextWord;
        cout << "Enter the next word (or type '!hint' for a suggestion): ";
        cin >> nextWord;

        if (nextWord == "!hint") {
            hintsUsed++;
            for (const auto& neighbor : graph.getNeighbors(currentWord)) {
                if (std::find(optimalPath.begin(), optimalPath.end(), neighbor) != optimalPath.end() && usedWords.find(neighbor) == usedWords.end()) {
                    cout << "Hint: ";
                    // Colorize the differing letter
                    for (size_t i = 0; i < currentWord.size(); ++i) {
                        if (currentWord[i] != neighbor[i]) {
                            cout << "\033[1;31m" << currentWord[i] << "\033[0m";
                        }
                        else {
                            cout << neighbor[i];
                        }
                    }
                    cout << endl;
                    break;
                }
            }
        }
        else if (usedWords.find(nextWord) == usedWords.end() && graph.hasNode(nextWord) && isValidTransformation(currentWord, nextWord)) {
            usedWords.insert(nextWord);
            userPath.push_back(nextWord);
            currentWord = nextWord;
        }
        else {
            cout << "Invalid word or word already used." << endl;
        }
    }
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::ofstream file(userName + ".csv", std::ios::app);
    file << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S") << ","
        << start << "," << target << "," << hintsUsed << "," << userPath.size() << "," << optimalPath.size() << ",";
    for (const auto& word : userPath) {
        file << word << " ";
    }
    file << "\n";
}

void Game::AnalyticsMode() {
    string userName;
    cout << "Enter your name: ";
    cin >> userName;

    ifstream file(userName + ".csv");
    if (!file) {
        cout << "No data available for user " << userName << endl;
        return;
    }

    set<string> uniqueWords;
    string line;
    int gamesPlayed = 0;
    int totalHints = 0;

    while (getline(file, line)) {
        gamesPlayed++;

        // Parse the CSV line
        std::istringstream ss(line);
        string date, start, target, hintsUsedStr, userPathSizeStr, optimalPathSizeStr, userPath;

        // Extract fields separated by commas
        getline(ss, date, ',');
        getline(ss, start, ',');
        getline(ss, target, ',');
        getline(ss, hintsUsedStr, ',');
        getline(ss, userPathSizeStr, ',');
        getline(ss, optimalPathSizeStr, ',');
        getline(ss, userPath, ',');

        // Convert hintsUsedStr to an integer
        int hintsUsed = std::stoi(hintsUsedStr);
        totalHints += hintsUsed;

        // Extract words from userPath
        std::istringstream pathStream(userPath);
        string word;
        while (pathStream >> word) {
            uniqueWords.insert(word);
        }
    }

    cout << "Games played: " << gamesPlayed << endl;
    cout << "Unique words used: " << uniqueWords.size() << endl;
    cout << "Total hints used: " << totalHints << endl;
}

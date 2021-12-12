#include <iostream>
#include <fstream>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <cctype>
#include <algorithm>

struct Cave {
    std::string name;
    bool big;
    int visited;
    std::vector<Cave*> nodes;
};

typedef std::unordered_map<std::string, Cave> CaveMap;

bool allUppers(std::string& s) {
    return std::all_of(std::begin(s), std::end(s), [](unsigned char c){ return std::isupper(c); });
}

int path(CaveMap& caves, Cave& cave, bool twice = false) {
    int paths{};
    ++cave.visited;

    for (auto& node : cave.nodes) {
        if (node->name == "end") {
            ++paths;
        } else if (node->big || node->visited < 1) {
            paths += path(caves, *node, twice);
        } else if (!twice && node->visited < 2) {
            paths += path(caves, *node, true);
        }
    }
    
    --cave.visited;
    return paths;
}


int solve(std::istream& input) {
    CaveMap caves{};
    std::string line;
    while (std::getline(input, line)) {
        auto found = line.find('-');
        auto firstName = line.substr(0, found);
        auto secondName = line.substr(found + 1);

        auto& firstCave = caves[firstName];
        auto& secondCave = caves[secondName];

        firstCave.name = firstName;
        secondCave.name = secondName;
        firstCave.big = allUppers(firstName);
        secondCave.big = allUppers(secondName);
        if (secondName != "start") firstCave.nodes.push_back(&secondCave);
        if (firstName != "start") secondCave.nodes.push_back(&firstCave);
    }
    
    return path(caves, caves["start"]);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "[ERROR] missing argument [input]\n";
    } else {    
        std::ifstream input(argv[1]);
        if (input.fail()) std::cerr << "[ERROR] " << strerror(errno) << '\n';

        std::cout << solve(input) << '\n';
    }
}

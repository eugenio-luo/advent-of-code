#include <iostream>
#include <fstream>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <algorithm>

long long solve(std::istream& input, int steps) {
    std::unordered_map<std::string, char> insertions;
    std::unordered_map<std::string, long long> counts;
    std::unordered_map<char, long long> charCounts;

    std::string line;
    std::getline(input, line);
    for (int i{}; i < line.size() - 1; ++i) {
        ++charCounts[line[i]];
        std::string count = std::string() + line[i] + line[i+1];
        ++counts[count];
    }
    ++charCounts[line.back()];
    
    std::getline(input, line); // empty line

    constexpr int single = 6;
    while (std::getline(input, line)) {
        std::string insertion = std::string() + line[0] + line[1];
        insertions[insertion] = line[single];
    }

    for (int step{}; step < steps; ++step) {
        auto newCounts = counts;
        
        for (auto& pair : counts) {
            long long n = pair.second;
            newCounts[pair.first] -= n;
            
            char c = insertions[pair.first];
            charCounts[c] += n;

            std::string count = std::string() + c + pair.first[1];
            newCounts[count] += n;
            count = std::string() + pair.first[0] + c;
            newCounts[count] += n;
        }

        counts = newCounts;
    }

    typedef std::pair<char, long long> Count;
    
    std::vector<Count> vecCounts(std::begin(charCounts), std::end(charCounts));
    auto greater = [](Count a, Count b){ return a.second > b.second; };
    std::sort(std::begin(vecCounts), std::end(vecCounts), greater);

    return vecCounts.front().second - vecCounts.back().second;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "[ERROR] missing argument [input] optional=[steps]\n";
    } else {
        std::ifstream input(argv[1]);
        if (input.fail()) std::cerr << "[ERROR] " << strerror(errno) << '\n';

        int steps = (argc == 3) ? std::stoi(argv[2]) : 10;
        std::cout << solve(input, steps) << '\n';
    }
}

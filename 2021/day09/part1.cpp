#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

int solve(std::ifstream& input) {
    std::vector<int> points{};
    std::string line;
    int size{};
    bool oneTime{true};
    while (std::getline(input, line)) {
        if (oneTime) {
            size = line.size();
            oneTime = false;
        }

        for (char letter : line) {
            if (letter == '\n') break;

            points.push_back(letter - '0');
        }
    }

    int riskLvl{};
    for (int i{}; i < points.size(); ++i) {
        if (i % size != 0) {
            if (points[i] >= points[i - 1]) continue;
        }

        if (i % size != size - 1) {
            if (points[i] >= points[i + 1]) continue;
        }

        if (i >= size) {
            if (points[i] >= points[i - size]) continue;
        }

        if (i < points.size() - size) {
            if (points[i] >= points[i + size]) continue;
        }

        riskLvl += points[i] + 1;
    }

    return riskLvl;
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

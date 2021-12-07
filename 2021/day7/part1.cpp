#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <vector>

int solve(std::istream& input) {
    std::vector<int> crabs{};

    int index{};
    while (input >> index) {
        if (index >= crabs.size()) { crabs.resize(index+1); }

        ++crabs[index];
        input.ignore(256, ',');
    }

    int min{};
    for (int i{}; i < crabs.size(); ++i) {
        int fuel{};

        for (int j{}; j < crabs.size(); ++j) {
            fuel += crabs[j] * std::abs(j - i);
        }

        if (min == 0 || fuel < min) { min = fuel; }
    }

    return min;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "[ERROR] missing argument [input]\n";
    } else {
        std::fstream input(argv[1]);
        if (input.fail()) std::cout << "[ERROR] " << strerror(errno) << '\n';
        
        std::cout << solve(input) << '\n';
    }
}

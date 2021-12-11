#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <array>
#include <algorithm>

long long solve(std::ifstream& input, int totalDays) {
    std::array<long long, 9> fishes{};
    int reprDay{6};
    
    int data;
    while (input >> data) {
        ++fishes[data];
        input.ignore(256 ,',');
    }

    for (int day{}; day < totalDays; ++day) {
        std::rotate(fishes.begin(), fishes.begin() + 1, fishes.end());
        fishes[reprDay] += fishes[fishes.size() - 1];
    }

    long long totalFishes{};
    for (int i{}; i < fishes.size(); ++i) {
        totalFishes += fishes[i];
    }
    
    return totalFishes;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "[ERROR] missing argument [input] optional=[days]\n";
    } else {
        std::ifstream input(argv[1]);
        if (input.fail()) std::cout << "[ERROR] " << strerror(errno) << '\n';

        int days = (argc < 3) ? 256 : std::stoi(argv[2]);
        std::cout << solve(input, days) << '\n';
    }
}

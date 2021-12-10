#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>

int solve(std::istream& input) {
    std::vector<int> measurements; 

    std::string line;
    while (std::getline(input, line)) {
        measurements.push_back(std::stoi(line));
    }

    int increase{};
    for (uint i{3}; i < measurements.size(); ++i) {
        if (measurements[i] > measurements[i-3]) { ++increase; }
    }
    
    return increase;
}

int main() {
    std::fstream input("input");
    if (input.fail()) { std::cout << "[ERROR] " << strerror(errno) << '\n'; }

    std::cout << solve(input) << '\n';
}
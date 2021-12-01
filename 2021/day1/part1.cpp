#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>

int solve(std::stringstream& input) {
    std::vector<int> measurements; 
    
    for (std::string line; std::getline(input, line);) {
        measurements.push_back(std::stoi(line));
    }

    int increase{};
    for (int i{1}; i < measurements.size(); ++i) {
        if (measurements[i] > measurements[i-1]) { ++increase; }
    }
    
    return increase;
}

int main() {
    std::fstream input("input");
    if (input.fail()) { std::cout << "[ERROR] " << strerror(errno) << '\n'; }

    std::stringstream buffer;
    buffer << input.rdbuf();

    std::cout << solve(buffer) << '\n';
}

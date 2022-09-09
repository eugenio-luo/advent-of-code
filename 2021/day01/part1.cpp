#include <iostream>
#include <fstream>
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
    for (uint i{1}; i < measurements.size(); ++i) {
        increase += measurements[i] > measurements[i-1];
    }
    
    return increase;
}

int main(int argc, char* argv[]) {
    std::fstream input(argv[1]);
    if (input.fail()) { std::cout << "[ERROR] " << strerror(errno) << '\n'; }
    
    std::cout << solve(input) << '\n';
}
    

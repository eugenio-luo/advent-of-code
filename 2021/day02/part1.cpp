#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>

int solve(std::istream& input) {
    std::string instruction{};
    int depth{}, position{};
    
    while (std::getline(input, instruction)) {
        auto pos = instruction.find(' ');
        auto operation = instruction.substr(0, pos);
        int value = std::stoi(instruction.substr(pos));
        
        switch (operation[0]) {
        case 'f': position += value; break;
        case 'd': depth += value; break;
        case 'u': depth -= value; break;
        }

    }

    return depth * position;
}

int main() {
    std::ifstream input("input");
    if (input.fail()) std::cout << "[ERROR] " << strerror(errno);

    std::cout << solve(input) << '\n';
}

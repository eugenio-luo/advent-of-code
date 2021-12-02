#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>

int solve(std::stringstream& input) {
    std::string instruction{};
    int depth{}, position{}, aim{};
    
    while (std::getline(input, instruction)) {
        auto pos = instruction.find(' ');
        auto operation = instruction.substr(0, pos);
        int value = std::stoi(instruction.substr(pos));
        
        switch (operation[0]) {
        case 'f': position += value; depth += value * aim; break;
        case 'd': aim += value; break;
        case 'u': aim -= value; break;
        }

    }

    return depth * position;
}

int main() {
    std::ifstream input("input");
    if (input.fail()) std::cout << "[ERROR] " << strerror(errno);

    std::stringstream buffer;
    buffer << input.rdbuf();

    std::cout << solve(buffer) << '\n';
}

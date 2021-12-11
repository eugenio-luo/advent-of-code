#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>

int solve(std::istream& input) {
    std::string line{};
    bool firstTime{true};
    int *commonBit;
    int length;
    
    while (std::getline(input, line)) {
        if (firstTime) {
            length = line.size();
            commonBit = new int[length]();
            firstTime = false;
        }

        for (int i{}; i < length; ++i) {
            commonBit[i] += (line[i] - '0') ? 1 : -1; 
        }
    }

    int gamma{}, epsilon{};
    for (int i{}; i < length; ++i) {
        int multiplier = (1 << (length - i - 1));
        gamma += (commonBit[i] > 0) ? multiplier : 0;
        epsilon += (commonBit[i] > 0) ? 0 : multiplier;
    }

    delete[] commonBit;
    
    return gamma * epsilon;
}

int main(int argc, char* argv[]) {
    std::ifstream input(argv[1]);
    if (input.fail()) std::cout << "[ERROR] " << strerror(errno);

    std::cout << solve(input) << '\n';
}

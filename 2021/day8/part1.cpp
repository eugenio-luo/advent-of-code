#include <iostream>
#include <fstream>
#include <cstring>

int solve(std::istream& input) {

    int result{};
    std::string line;
    while (std::getline(input, line)) {
        int delim = line.find('|') + 2;
        
        for (int start{delim}; start < line.size();) {
            int end = line.find(' ', start);
            if (end == -1) end = line.size();

            switch(end - start) {
            case 2:
            case 3:
            case 4:
            case 7: ++result;
            default: break;
            }

            start = end + 1;
        }
    }

    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "[ERROR] missing argument [input]\n";
    } else {
        std::ifstream input(argv[1]);
        if (input.fail()) std::cout << "[ERROR] " << strerror(errno) << '\n';

        std::cout << solve(input) << '\n';
    }
}
            

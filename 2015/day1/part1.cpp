#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>

int solve(const std::string& input) {
    int floor = 0;
    
    for (char letter : input) {

        floor += (letter == '(') ? 1 : -1;
    }

    return floor; 
}

int main() {
    std::ifstream t("input");
    if (t.fail()) {
        std::cout << "ERROR: " << strerror(errno) << '\n';
    } else {
        std::stringstream buffer;
        buffer << t.rdbuf();

        std::cout << solve(buffer.str()) << '\n';     
    }
}

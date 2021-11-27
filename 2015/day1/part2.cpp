#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>

int solve(const std::string& input) {
    int floor = 0;
    int pos = 0;
    
    for (char letter : input) {
        ++pos;
        floor += (letter == '(') ? 1 : -1;

        if (floor < 0) {
            return pos;
        }
    }
    
    return -1;
}

int main() {
    std::ifstream t("input");
    if (t.fail()) {
        std::cout << "ERROR: " << strerror(errno) << '\n';
    } else {
        std::stringstream buffer;
        buffer << t.rdbuf();
        t.close();
        
        std::cout << solve(buffer.str()) << '\n';     
    }
}

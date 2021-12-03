#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>

std::string find(const std::string& input, bool inverse = false) {
    uint length = input.find('\n');
    std::string bits;
    std::string compareList = input;
    
    for (uint i{}; i < length; ++i) {
        int delta{};
        for (uint j{i}; j < compareList.size(); j += length + 1) {
            delta += ((compareList[j] - '0') > 0) ? 1 : -1;
        }
       
        if (inverse) {
            bits.push_back((delta >= 0) ? '0' : '1');
        } else {
            bits.push_back((delta >= 0) ? '1' : '0');
        }
        
        int size = compareList.size();
        for (int k{}; k < size; k += length + 1) {
            
            auto line = compareList.substr(k, length);
            bool same {true};
            
            for (uint m{}; m <= i; ++m) {
                if (line[m] != bits[m]) same = false;
            }
            
            if (same) {
                compareList += compareList.substr(k, length) + '\n';
            }
        }
       
        compareList = compareList.substr(size);
        
        if (compareList.size() == length + 1) {
            return compareList;
        }
    }

    return compareList;
}

int solve(std::stringstream& input) {
    auto oxygen = find(input.str());
    auto co2 = find(input.str(), true);

    return std::stoi(oxygen, nullptr, 2) * std::stoi(co2, nullptr, 2);
}

int main() {
    std::ifstream input("input");
    if (input.fail()) std::cout << "[ERROR] " << strerror(errno);

    std::stringstream buffer;
    buffer << input.rdbuf();

    std::cout << solve(buffer) << '\n';
}

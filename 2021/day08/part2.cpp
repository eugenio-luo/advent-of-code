#include <iostream>
#include <fstream>
#include <cstring>
#include <array>
#include <vector>
#include <algorithm>
#include <iterator>

void remove(std::string& string, char letter) {
    string.erase(std::remove(std::begin(string), std::end(string), letter), std::end(string));
}

char removeSave(std::string& string, char letter) {
    auto removed = std::remove(std::begin(string), std::end(string), letter);

    if (removed != std::end(string)) {
        string.erase(removed, std::end(string));
        return letter;
    } else {
        return '.';
    }
}       

void recognize(std::array<std::string, 10>& table, const std::string& line, int delim) {
    std::string value1, value4, value7, value8;
    std::vector<std::string> value069;
    for (int start{}; start < delim;) {
        int end = line.find(' ', start);

        std::string segment = line.substr(start, end - start);
        
        switch(end - start) {
        case 2: value1 = segment; break;
        case 3: value7 = segment; break;
        case 4: value4 = segment; break;
        case 6: value069.push_back(segment); break;
        case 7: value8 = segment; break;
        }
        
        start = end + 1;
    }

    std::array<std::string, 3> values8;
    std::fill(std::begin(values8), std::end(values8), value8);
    for (int i{}; i < values8.size(); ++i) {
        for (char letter : value069[i]) {
            remove(values8[i], letter);
        }
    }
    
    for (char letter : value7 + value4) {
        remove(value8, letter);
    }

    for (char letter : value1) {
        remove(value4, letter);
    }
    
    for (char letter : value1) {
        remove(value7, letter);
    }

    char saved1, saved4, saved8;
    for (auto string : values8) {
        char a = removeSave(value1, string[0]);
        char b = removeSave(value4, string[0]);
        char c = removeSave(value8, string[0]);

        if (a != '.') saved1 = a;
        if (b != '.') saved4 = b;
        if (c != '.') saved8 = c;
    }

    table[0] = std::string{value7[0]} + value4[0] + saved1 + saved8 + value1[0] + value8[0];
    table[2] = std::string{value7[0]} + saved1 + saved4 + saved8 + value8[0];
    table[3] = std::string{value7[0]} + saved1 + saved4 + value1[0] + value8[0];
    table[5] = std::string{value7[0]} + value4[0] + saved4 + value1[0] + value8[0];
    table[6] = std::string{value7[0]} + value4[0] + saved4 + saved8 + value1[0] + value8[0];
    table[9] = std::string{value7[0]} + value4[0] + saved1 + saved4 + value1[0] + value8[0];

    for (auto& string : table) {
        std::sort(std::begin(string), std::end(string));
    }
}

int find(const std::array<std::string, 10>& table, std::string& value) {
    std::sort(std::begin(value), std::end(value));

    int result{-1};
    for (int i{}; i < table.size(); ++i) {
        if (table[i] == value) { result = i; break; }
    }

    return result;
}

int solve(std::istream& input) {

    int result{};
    std::string line;
    std::array<std::string, 10> table{};
    while (std::getline(input, line)) {

        int delim = line.find('|');
        recognize(table, line, delim);

        int mult = 1000, number{};
        for (int start{delim + 2}; start < line.size();) {
            int end = line.find(' ', start);
            if (end == -1) end = line.size();

            switch(end - start) {
            case 2: number += mult * 1; break;
            case 3: number += mult * 7; break;
            case 4: number += mult * 4; break;
            case 7: number += mult * 8; break;
            default: {
                std::string value = line.substr(start, end - start);
                number += mult * find(table, value);
                break;
            }
            }

            mult /= 10;
            start = end + 1;
        }

        result += number;
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

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <stack>

char parse(const std::string& line) {
    std::stack<char> brackets{};
    for (char letter : line) {
        switch(letter) {
        case '(':
        case '[':
        case '{':
        case '<': brackets.push(letter); break;
        default: {
            char c = brackets.top();
            char check;
            switch(c) {
            case '(' : check = ')'; break;
            case '[' : check = ']'; break;
            case '{' : check = '}'; break;
            case '<' : check = '>'; break; 
            }

            if (letter != check) return letter;
            brackets.pop();
        }
        }
    }

    return ' ';
}

int solve(std::istream& input) {
    std::string line;
    std::vector<std::string> lines;
    while (input >> line) {
        lines.push_back(line);
    }

    int sum{};
    std::vector<std::string> incompleteLines;
    for (auto line : lines) {
        char c = parse(line);
        if (c != ' ') {
            switch(c) {
            case ')' : sum+=3; break; 
            case ']' : sum+=57; break; 
            case '}' : sum+=1197; break; 
            case '>' : sum+=25137; break; 
            }
        } 
    }
    
    return sum; 
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "[ERROR] missing argument [input]\n";
    } else {
        std::ifstream input(argv[1]);
        if (input.fail()) std::cerr << "[ERROR] " << strerror(errno) << '\n';

        std::cout << solve(input) << '\n';
    }
}

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <stack>
#include <algorithm>

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

long long complete(const std::string& line) {
    std::stack<char> brackets;
    for (char letter : line) {
        switch(letter) {
        case '(':
        case '[':
        case '{':
        case '<': brackets.push(letter); break;
        default: brackets.pop(); break;
        }
    }

    long long score{};
    while (!brackets.empty()) {
        char c = brackets.top();
        brackets.pop();
        score *= 5;
        switch (c) {
        case '(' : score += 1; break;
        case '[' : score += 2; break;
        case '{' : score += 3; break;
        case '<' : score += 4; break;
        }
    }

    return score;
}

long long solve(std::istream& input) {
    std::string line;
    std::vector<std::string> lines;
    while (input >> line) {
        lines.push_back(line);
    }

    std::vector<std::string> incompleteLines;
    for (auto line : lines) {
        char c = parse(line);
        if (c == ' ') incompleteLines.push_back(line); 
    }

    std::vector<long long> scores;
    for (auto line : incompleteLines) {
        scores.push_back(complete(line));
    }
    
    std::sort(std::begin(scores), std::end(scores));

    return scores[(scores.size() / 2)]; 
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

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <vector>
#include <stack>

struct Command {
    int start;
    int jump;
    int end;
    int jumpY;

    Command(int s, int j, int e, int jy = 0)
        : start{s}, jump{j}, end{e}, jumpY{jy} {}
};

void parse(std::stack<Command>& lines, std::stringstream& input, int sideLength) {
    std::string string;
    while (std::getline(input, string)) {
        auto comma = string.find(',');
        int startX = std::stoi(string.substr(0, comma));
        
        auto space = string.find(' ');
        int startY = std::stoi(string.substr(comma + 1, space - comma + 1));

        auto secondComma = string.find(',', space);
        int endX = std::stoi(string.substr(space + 4, secondComma - space + 4));
        int endY = std::stoi(string.substr(secondComma + 1));

        int start = startX + startY * sideLength;
        int end = endX + endY * sideLength;
        if (startX == endX) {
            int direction = (startY < endY) ? 1 : -1;
            int jump = sideLength * direction;
            lines.push(Command(start, jump, end));
        } else if (startY == endY) {
            int direction = (startX < endX) ? 1 : -1;
            lines.push(Command(start, direction, end));
        } else {
            int directionX = (startX < endX) ? 1 : -1;
            int directionY = (startY < endY) ? 1 : -1;
            int jumpY = sideLength * directionY;
            lines.push(Command(start, directionX, end, jumpY));
        }
    }
}

bool condition(int start, int end, int i) {
    return (start < end) ? i <= end : i >= end;
}

int solve(std::stringstream& input, int sideLength) {
    std::stack<Command> lines;
    parse(lines, input, sideLength);

    std::vector<short> positions(sideLength * sideLength);
    int overlaps{};

    while (!lines.empty()) {
        auto com = lines.top();
        
        for (int i{com.start}; condition(com.start, com.end, i); i+=com.jump + com.jumpY) {
            if (positions[i] == 0) {
                ++positions[i];
            } else if (positions[i] == 1) {
                ++overlaps;
                ++positions[i];
            } 
        }

        lines.pop();
    }
   
    return overlaps;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "[ERROR] missing argument, should be [input] [sideLength]\n";
    } else {
        std::ifstream input(argv[1]);
        if (input.fail()) std::cout << "[ERROR] " << strerror(errno);

        std::stringstream buffer;
        buffer << input.rdbuf();

        std::cout << solve(buffer, std::strtol(argv[2], nullptr, 10)) << '\n';
    }
}

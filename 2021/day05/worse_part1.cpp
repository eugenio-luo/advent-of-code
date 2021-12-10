#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>

struct Line {
    int start;
    int end;
    int pos;
    bool isVertical;

    Line(int s, int e, int p, bool i) : start{s}, end{e}, pos{p}, isVertical{i} {}
};

void parse(std::vector<Line>& lines, std::istream& input) {
    std::string string;
    while (std::getline(input, string)) {
        auto comma = string.find(',');
        int startX = std::stoi(string.substr(0, comma));
        
        auto space = string.find(' ');
        int startY = std::stoi(string.substr(comma + 1, space - comma + 1));

        auto secondComma = string.find(',', space);
        int endX = std::stoi(string.substr(space + 4, secondComma - space + 4));
        int endY = std::stoi(string.substr(secondComma + 1));

        if (startX == endX) {
            lines.push_back(Line(startY, endY, startX, true));
        } else if (startY == endY) {
            lines.push_back(Line(startX, endX, startY, false));
        }
    }
}

struct Pos {
    int x;
    int y;
    bool overlap;

    Pos() = default;
    Pos(int x, int y) : x{x}, y{y}, overlap{false} {}
    friend bool operator==(Pos a, Pos b) {
        return a.x == b.x && a.y == b.y;
    };
};

bool condition(int i, int start, int end) {
    return (start < end) ? i <= end : i >= end;
}

int solve(std::istream& input) {
    std::vector<Line> lines;
    parse(lines, input);

    int overlaps{};
    std::vector<Pos> positions;
    int count{};
    for (auto line : lines) {
        std::cout << "now checking: " << line.start << ' ' << line.end << ' ' << line.pos << '\n';
        int increase = (line.start < line.end) ? 1 : -1;
        for (int i{line.start}; condition(i, line.start, line.end); i += increase) {
            Pos pos;
            
            if (line.isVertical) {
                pos = Pos(line.pos, i);
            } else {
                pos = Pos(i, line.pos);
            }

            auto found = std::find(std::begin(positions), std::end(positions), pos);
            if (found != std::end(positions)) {
                if (!(found->overlap)) { ++overlaps; }
                found->overlap = true;
            } else {
                positions.push_back(pos);
            }
        }
    }
    
    return overlaps;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "[ERROR] missing argument\n";
    } else {
        std::ifstream input(argv[1]);
        if (input.fail()) std::cout << "[ERROR] " << strerror(errno);

        std::cout << solve(input) << '\n';
    }
}

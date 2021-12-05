#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>

struct Line {
    int sx;
    int sy;
    int ex;
    int ey;

    Line(int sx, int sy, int ex, int ey) : sx{sx}, sy{sy}, ex{ex}, ey{ey} {}
};

void parse(std::vector<Line>& lines, std::stringstream& input) {
    std::string string;
    while (std::getline(input, string)) {
        auto comma = string.find(',');
        int startX = std::stoi(string.substr(0, comma));
        
        auto space = string.find(' ');
        int startY = std::stoi(string.substr(comma + 1, space - comma + 1));

        auto secondComma = string.find(',', space);
        int endX = std::stoi(string.substr(space + 4, secondComma - space + 4));
        int endY = std::stoi(string.substr(secondComma + 1));

        lines.push_back(Line(startX, startY, endX, endY));
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

void found(std::vector<Pos>& posts, const Pos& pos, int& overlaps) {
    auto found = std::find(std::begin(posts), std::end(posts), pos);
    if (found != std::end(posts)) {
        if (!found->overlap) ++overlaps;
        found->overlap = true;
    } else {
        posts.push_back(pos);
    }
}

int loop(int start, int end, int fixed, bool isVertical, std::vector<Pos>& posts) {
    int overlaps{};
    int inc = (start < end) ? 1 : -1;
    for (int i{start}; condition(i, start, end); i += inc) {
        Pos pos;

        if (isVertical) {
            pos = Pos(fixed, i);
        } else {
            pos = Pos(i, fixed);
        }

        found(posts, pos, overlaps);
    }

    return overlaps;
}

int solve(std::stringstream& input) {
    std::vector<Line> lines;
    parse(lines, input);

    int overlaps{};
    std::vector<Pos> positions;
    int count{};
    for (auto line : lines) {
        std::cout << "now checking: " << line.sx << ',' << line.sy << " -> " << line.ex << ',' << line.ey << '\n';
        
        if (line.sx == line.ex) {
            overlaps += loop(line.sy, line.ey, line.sx, true, positions);
        } else if (line.sy == line.ey) {
            overlaps += loop(line.sx, line.ex, line.sy, false, positions);
        } else {

            int directionX = (line.sx < line.ex) ? 1 : -1;
            int directionY = (line.sy < line.ey) ? 1 : -1;

            for (int i{}; i <= std::abs(line.sx - line.ex); ++i) {
                Pos pos = Pos(line.sx + i * directionX, line.sy + i * directionY);

                found(positions, pos, overlaps);
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

        std::stringstream buffer;
        buffer << input.rdbuf();

        std::cout << solve(buffer) << '\n';
    }
}

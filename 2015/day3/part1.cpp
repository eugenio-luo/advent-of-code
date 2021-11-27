#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

struct Pos {
    int x;
    int y;

    Pos(int x, int y) : x{x}, y{y} {};

    friend bool operator==(const Pos a, const Pos b) {
        return a.x == b.x && a.y == b.y;
    }
};

void move(Pos& pos, char movement) {
    switch (movement) {
    case '>': ++pos.x; break;
    case 'v': --pos.y; break;
    case '^': ++pos.y; break;
    case '<': --pos.x; break;

    default: std::cout << "non existant input"; break;
    }
}

int solve(const std::string& input) {
    Pos sPos = Pos(0, 0);
    std::vector<Pos> visited;
    int presents{};
    
    visited.push_back(Pos(sPos.x, sPos.y));
    ++presents;
    
    for (char letter : input) {

        move(sPos, letter);

        if (std::find(visited.begin(), visited.end(), sPos) == visited.end()) {
            visited.push_back(sPos);
            ++presents;
        }
    }

    return presents;
}

int main() {
    std::fstream t("input");

    if (t.fail()) { std::cout << "ERROR: " << strerror(errno); }
    
    std::stringstream buffer;
    buffer << t.rdbuf();
    t.close();

    std::cout << solve(buffer.str()) << '\n';
}

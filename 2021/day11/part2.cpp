#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

struct Lvl {
    int val;
    bool fls;
 
    Lvl(int v) : val{v}, fls{false} {}
};

int flashing(std::vector<Lvl>& levels, int size, int index) {
    int flashes{};
    if (levels[index].val <= 9) return 0;

    levels[index].val = 0;
    ++flashes;
    levels[index].fls = true;
    std::vector<int> movesX{0};
    std::vector<int> movesY{0};
    
    if (index % size > 0) movesX.push_back(-1);
    if (index % size < size - 1) movesX.push_back(1);
    if (index >= size) movesY.push_back(-size);
    if (index < levels.size() - size) movesY.push_back(size);

    for (int y : movesY) {
        for (int x : movesX) {
            if (!levels[index + x + y].fls) {
                ++levels[index + x + y].val;
                flashes += flashing(levels, size, index + x + y);
            }
        }
    }

    return flashes;
}

bool turn(std::vector<Lvl>& levels, int size) {
    constexpr int flashLvl = 9;

    for (int i{}; i < levels.size(); ++i) {
        ++levels[i].val;
    }
    
    for (int i{}; i < levels.size(); ++i) {
        flashing(levels, size, i);
    }

    int zeroes{};
    for (auto& lvl : levels) {
        lvl.fls = false;
        if (lvl.val == 0) ++zeroes;
    }

    return zeroes == levels.size();
}

int solve(std::istream& input) {
    std::vector<Lvl> levels;
    std::string line;
    int size;
    bool oneTime{};
    while (std::getline(input, line)) {
        if (!oneTime) {
            size = line.size();
            oneTime = true;
        }
        
        for (char letter : line) {
            levels.push_back(Lvl(letter - '0'));
        }
    }

    for (int step{};; ++step) {
        if (turn(levels, size)) return step + 1;
    }

    return -1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "[ERROR] missing argument [input] [steps]\n";
    } else {
        std::ifstream input(argv[1]);
        if (input.fail()) std::cerr << "[ERROR] " << strerror(errno) << '\n';

        std::cout << solve(input) << '\n';
    }
}

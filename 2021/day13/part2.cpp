#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

struct Pos {
    int x;
    int y;

    Pos() = default;
    Pos(int x, int y) : x{x}, y{y} {}
};

struct Fold {
    bool isY;
    int num;

    Fold() = default;
    Fold(bool b, int i) : isY{b}, num{i} {}
};

void folding(std::vector<bool>& paper, const Pos& size, Pos& visualSize, int axis, bool isY) {
    int maxSize = (isY) ? size.x : size.y;

    for (int i{1}; i <= axis; ++i) {
        for (int j{}; j < maxSize; ++j) {
            if (isY) {
                bool up = paper[(axis - i) * size.x + j];
                bool down = paper[(axis + i) * size.x + j];
                
                paper[(axis - i) * size.x + j] = up || down;
            } else {
                bool left = paper[j * size.x + axis - i];
                bool right = paper[j * size.x + axis + i];
                
                paper[j * size.x + axis - i] = left || right;
            }
        }
    }
    
    if (isY) {
        visualSize.y /= 2;
    } else {
        visualSize.x /= 2;
    }
    
}

int solve(std::istream& input) {
    constexpr int charIndex = 11;
    std::string line;
    Pos size;
    
    std::vector<Fold> folds;
        
    while (std::getline(input, line)) {
        if (line[0] != 'f') continue;

        bool isY = line[charIndex] == 'y';
        int num = std::stoi(line.substr(charIndex + 2));

        folds.push_back(Fold(isY, num));
        
        if (size.x == 0 || size.y == 0) {
            if (line[charIndex] == 'x') {
                size.x = std::stoi(line.substr(charIndex+2)) * 2 + 1;
            } else {
                size.y = std::stoi(line.substr(charIndex+2)) * 2 + 1;
            }
        }
    }

    Pos visualSize = size;
    std::vector<bool> paper(size.x * size.y);  

    input.clear();
    input.seekg(0);

    while (std::getline(input, line)) {
        if (line.empty()) break;
        
        auto delim = line.find(',');
        int x = std::stoi(line.substr(0, delim));
        int y = std::stoi(line.substr(delim + 1));

        paper[y * size.x + x] = true;
    }

    
    for (auto& fold : folds) {
        if (fold.isY) {
            folding(paper, size, visualSize, fold.num, true);
        } else {
            folding(paper, size, visualSize, fold.num, false);
        }
    }
    
    for (int y{}; y < visualSize.y; ++y) {
        for (int x{}; x < visualSize.x; ++x) {
            std::cout << (paper[y * size.x + x] ? '#' : '.');
        }
        std::cout << '\n';
    }

    return 0;
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

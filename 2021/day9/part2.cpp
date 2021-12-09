#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>

struct Pt {
    int val;
    bool chk;

    Pt(int v) : val{v}, chk{false} {}
};

int check(int i, int size, std::vector<Pt>& points) {
    points[i].chk = true;
    if (points[i].val == 8) return 1;
    int count{}, index{};
    std::vector<int> moves{ i-1, i+1, i - size, i + size };
    std::vector<bool> conditions{ i % size != 0, i % size != size - 1, i >= size, i < points.size() - size};
    for (int value : moves) {
        if (conditions[index] &&
                points[value].val != 9 &&
                (!points[value].chk) &&
                points[i].val < points[value].val) {

            count += check(value, size, points);
        }
        ++index;
    }
    
    return ++count;
}

int solve(std::ifstream& input) {
    std::vector<Pt> points{};
    std::string line;
    int size{};
    bool oneTime{true};
    while (std::getline(input, line)) {
        if (oneTime) {
            size = line.size();
            oneTime = false;
        }

        for (char letter : line) {
            if (letter == '\n') break;
            points.push_back(Pt(letter - '0'));
        }
    }

    std::vector<int> lowPointIndex{};
    for (int i{}; i < points.size(); ++i) {
        if (points[i].val == 8) continue;
        
        if (i % size != 0) {
            if (points[i].val >= points[i - 1].val) continue;
        }

        if (i % size != size - 1) {
            if (points[i].val >= points[i + 1].val) continue;
        }

        if (i >= size) {
            if (points[i].val >= points[i - size].val) continue;
        }

        if (i < points.size() - size) {
            if (points[i].val >= points[i + size].val) continue;
        }

        lowPointIndex.push_back(i);
    }
    
    std::vector<int> counts{};
    for (int index : lowPointIndex) {
        int count = check(index, size, points);
        counts.push_back(count);
    }

    std::sort(std::begin(counts), std::end(counts), std::greater<int>());

    return counts[0] * counts[1] * counts[2];
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

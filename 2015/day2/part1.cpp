#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

struct Dim {
    int l;
    int w;
    int h;

    Dim(int l, int w, int h) : l{l}, w{w}, h{h} {}
};

const std::vector<Dim> parse(std::ifstream& input) {
    std::vector<Dim> dimensions;
    int l, w, h;
    std::string line;
    
    while (std::getline(input, line)) {
        std::string delimeterString;
        std::istringstream textStream(line);

        textStream >> l;
        std::getline(textStream, delimeterString, 'x');
        textStream >> w;
        std::getline(textStream, delimeterString, 'x');
        textStream >> h;
        std::getline(textStream, delimeterString, '\n');

        Dim d = Dim(l, w, h);
        dimensions.push_back(d);
    }

    return dimensions;
}

int solve(std::ifstream& input) {
    std::vector<Dim> dimensions = parse(input);
    int paperNeeded;
    
    for (Dim& dim : dimensions) {
        int side0 = dim.l*dim.w;
        int side1 = dim.w*dim.h;
        int side2 = dim.h*dim.l;
        
        paperNeeded += 2*side0 + 2*side1 + 2*side2 + std::min({side0, side1, side2});
    }

    return paperNeeded;
}

int main() {
    std::ifstream t("input");

    if (t.fail()) { std::cout << "ERROR: " << strerror(errno); }

    std::cout << solve(t) << '\n';
}

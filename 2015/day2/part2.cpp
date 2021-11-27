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
    int ribbonNeeded;

    for (Dim& dim : dimensions) {
        ribbonNeeded += dim.l*2 + dim.w*2 + dim.h*2 + dim.l*dim.w*dim.h;

        ribbonNeeded -= std::max({dim.l, dim.w, dim.h})*2;
    }

    return ribbonNeeded;
}

int main() {
    std::ifstream t("input");

    if (t.fail()) { std::cout << "ERROR: " << strerror(errno); }

    std::cout << solve(t) << '\n';
    t.close();
}

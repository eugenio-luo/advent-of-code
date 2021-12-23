#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

typedef long long ll;
typedef std::vector<bool> Pixels;
typedef std::vector<Pixels> Image;

ll bin2dec(const std::vector<bool>& binary) {
    ll result{};

    for (int i{}; i < binary.size(); ++i) {
        result += binary[i] * (1 << (binary.size() - i - 1));
    }

    return result;
}

void transform(const Image& input, Image& output, const Pixels& algorithm, ll xIndex, ll yIndex) {
    static std::vector<int> moves{-1, 0, 1};

    std::vector<bool> binary;
    for (auto y : moves) {
        for (auto x : moves) {
            binary.push_back(input[yIndex + y][xIndex + x]);
        }
    }
    
    auto algoIndex = bin2dec(binary);
    output[yIndex][xIndex] = algorithm[algoIndex];
}

ll solve(std::istream& input) {
    std::string line;
    Pixels algorithm;
    
    std::getline(input, line);
    for (char c : line) {
        algorithm.push_back(c == '#');
    }
    std::getline(input, line); // empty line

    Image original;
    while (std::getline(input, line)) {
        original.push_back(Pixels());

        for (char c : line) {
            original.back().push_back(c == '#');
        }
    }

    auto inputImage = original;
    ll steps = 50;
    ll increaseSize = 2;
    bool resultOf0 = algorithm[0];
    bool resultOfResult = algorithm[bin2dec(std::vector<bool>(9, resultOf0))];
    Pixels increaseVec0(increaseSize / 2, resultOf0);
    Pixels increaseVec1(increaseSize / 2, resultOfResult);
    bool turn{1};
    
    for (ll step{}; step < steps; ++step) {
        auto& increaseVec = (turn) ? increaseVec1 : increaseVec0;
        for (auto& pixels : inputImage) {
            
            pixels.insert(std::begin(pixels), std::begin(increaseVec), std::end(increaseVec));
            pixels.insert(std::end(pixels), std::begin(increaseVec), std::end(increaseVec));
        }

        ll size = inputImage[0].size();
        Pixels pixels = Pixels(size, (turn) ? resultOfResult : resultOf0);
        for (ll i{}; i < increaseSize / 2; ++i) {
            
            inputImage.insert(std::begin(inputImage), pixels);
            inputImage.insert(std::end(inputImage), pixels);
        }

        turn = !turn;
        auto outputImage = inputImage;
        auto imageSize = outputImage.size();
        
        for (ll y{}; y < imageSize; ++y) {
            auto size = outputImage[y].size();

            for (ll x{}; x < size; ++x) {
                if (y == 0 || x == 0 || y == imageSize - 1 || x == size - 1) {
                    outputImage[y][x] = (turn) ? resultOfResult : resultOf0;
                    continue;
                }

                transform(inputImage, outputImage, algorithm, x, y);
            }
        }
        
        inputImage = outputImage;
    }

    ll sum{};
    for (auto& pixels : inputImage) {
        for (auto b : pixels) {
            sum += b;
        }
    }
    
    return sum;
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

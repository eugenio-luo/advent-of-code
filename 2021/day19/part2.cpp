#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include <queue>

struct Pos {
    int x;
    int y;
    int z;

    Pos(int x, int y, int z) : x{x}, y{y}, z{z} {}

    Pos& operator-=(const Pos& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;

        return *this;
    }

    friend Pos operator-(Pos lhs, const Pos& rhs) {
        lhs -= rhs;

        return lhs;
    }

    bool operator< (const Pos& b) const {
        return x < b.x || (x == b.x && y < b.y) ||
            (x == b.x && y == b.y && z < b.z);
    }

    friend bool operator==(const Pos& a, const Pos& b) {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }
};

struct Scanner {
    int id;
    std::vector<Pos> beacons;

    Scanner(int id) : id{id} {}
};

enum {
    X = 1,
    Y,
    Z
};

std::vector<std::vector<int>> rotations {
    {X, Y, Z},
    {X, -Z, Y},
    {X, -Y, -Z},
    {X, Z, -Y},
    {-X, -Y, Z},
    {-X, -Z, -Y},
    {-X, Y, -Z},
    {-X, Z, Y},
    {-Z, X, -Y},
    {Y, X, -Z},
    {Z, X, Y},
    {-Y, X, Z},
    {Z, -X, -Y},
    {Y, -X, Z},
    {-Z, -X, Y},
    {-Y, -X, -Z},
    {-Y, -Z, X},
    {Z, -Y, X},
    {Y, Z, X},
    {-Z, Y, X},
    {Z, Y, -X},
    {-Y, Z, -X},
    {-Z, -Y, -X},
    {Y, -Z, -X}
};

void transform(const std::vector<Pos>& transformer, std::vector<Pos>& transformations,
               const std::vector<int>& rotat) {

    for (auto& beacon : transformer) {
        std::vector<int> holder;
        
        for (auto rot : rotat) {
            switch (rot) {
            case X: holder.push_back(beacon.x); break;
            case Y: holder.push_back(beacon.y); break;
            case Z: holder.push_back(beacon.z); break;
            case -X: holder.push_back(-beacon.x); break;
            case -Y: holder.push_back(-beacon.y); break;
            case -Z: holder.push_back(-beacon.z); break;
            }
        }
        
        auto newBeacon = Pos(holder[0], holder[1], holder[2]);

        transformations.push_back(newBeacon);
    }
}

std::pair<bool, Pos> check(const std::vector<Pos>& transformations,
                           const std::set<Pos>& receiver, int id) {

    std::map<Pos, int> distanceMap;
    
    for (auto& trans : transformations) {
        for (auto& beacon : receiver) {
            auto distance = trans - beacon;

            if (++distanceMap[distance] == 12) {
                return {true, distance};
            }
        }
    }

    return {false, Pos(0, 0, 0)};
}

int solve(std::istream& input) {
    std::string line;
    std::vector<Scanner> scanners; 
    int id{};
    
    while (std::getline(input, line)) {
        if (line.empty()) continue;

        if (line[1] == '-') {
            scanners.push_back(Scanner(id++));
            continue;
        }

        auto comma = line.find(',');
        auto secondComma = line.find(',', comma + 1);
        int x = std::stoi(line.substr(0, comma));
        int y = std::stoi(line.substr(comma + 1, secondComma - comma - 1));
        int z = std::stoi(line.substr(secondComma + 1));
        
        scanners.back().beacons.push_back(Pos(x, y, z));
    }

    std::set<Pos> knownBeacon = {std::begin(scanners[0].beacons), std::end(scanners[0].beacons)};
    std::queue<int> scannersQueue;
    for (int i{}; i < scanners.size(); ++i) {
        scannersQueue.push(i);
    }
    std::vector<Pos> knownPositions;

    while (!scannersQueue.empty()) {
        int index = scannersQueue.front();
        auto scanner = scanners[index];
        scannersQueue.pop();
        const auto& beacons = scanner.beacons;
        bool found = false;
        
        for (int i{}; i < 24; ++i) {
            std::vector<Pos> transformations;
            const auto& rotat = rotations[i];
            transform(beacons, transformations, rotat);

            auto result = check(transformations, knownBeacon, scanner.id);

            if (result.first) {
                auto distance = result.second;
                knownPositions.push_back(distance);
                found = true;
                
                for (auto transform : transformations) {
                    transform -= distance;

                    knownBeacon.insert(transform);
                }
            }
        }

        if (!found) scannersQueue.push(index); 
    }

    int max{};
    for (auto& a : knownPositions) {
        for (auto& b : knownPositions) {
            auto c = a - b;
            int distance = std::abs(c.x) + std::abs(c.y) + std::abs(c.z);
            if (distance > max) max = distance;
        }
    }
    
    return max;
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

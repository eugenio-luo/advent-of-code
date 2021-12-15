#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

typedef std::vector<long long> IntVec;
typedef std::vector<bool> BoolVec;

void nextVertex(const IntVec& cavern, IntVec& shortestPath, IntVec& prev,
                int index, int next) {

    long long value = cavern[next] + shortestPath[index];
    long long& nextVertex = shortestPath[next];
        
    if (nextVertex == -1 || value < nextVertex)  {
        nextVertex = value;
        prev[next] = index;
    }
}

void vertex(const IntVec& cavern, IntVec& shortestPath, IntVec& prev,
            BoolVec& visited, int index, int size) {

    visited[index] = true;
    
    if (index % size != size - 1) {
        nextVertex(cavern, shortestPath, prev, index, index + 1);
    }
    
    if (index % size != 0) {
        nextVertex(cavern, shortestPath, prev, index, index - 1);
    }
        
    if (index < cavern.size() - size) {
        nextVertex(cavern, shortestPath, prev, index, index + size);
    }

    if (index >= size) {
        nextVertex(cavern, shortestPath, prev, index, index - size);
    }
}

long long getShortestPath(const IntVec& cavern, const IntVec& prev, int index) {
    if (index == 0) return 0;
    long long result = cavern[index];
    result += getShortestPath(cavern, prev, prev[index]);
    return result;
}

long long path(const IntVec& cavern, int size, int bandwidth) {
    IntVec shortestPath(cavern.size());
    IntVec prev(cavern.size());
    BoolVec visited(cavern.size());
    std::fill(std::begin(shortestPath) + 1, std::end(shortestPath), -1);
    int index = 0;
    long long minValue;
    int min{};
    
    do {
        vertex(cavern, shortestPath, prev, visited, index, size);

        minValue = INT64_MAX;
        int initial = (min - bandwidth < 0) ? 0 : min - bandwidth;
        int end = (min + bandwidth > shortestPath.size()) ? shortestPath.size() : min + bandwidth; 
        for (int i{initial}; i < end; ++i) {
            if (shortestPath[i] != -1 && !visited[i] && shortestPath[i] < minValue) {
                min = i;
                minValue = shortestPath[i];
            }
        }

        index = min;
        if (index == size * size - 1) break;
        
    } while (minValue != INT64_MAX);
    
    long long riskLvl = getShortestPath(cavern, prev, size * size - 1);
    return riskLvl;
}

long long solve(std::istream& input, int bandwidth) {
    IntVec cavern;
    std::string line;
    bool oneTime = true;
    int size;
    
    while (std::getline(input, line)) {
        if (oneTime) {
            size = line.size();
            oneTime = false;
        }

        for (char c : line) {
            cavern.push_back(c - '0');
        }
    }

    long long riskLvl = path(cavern, size, bandwidth);
    
    return riskLvl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "[ERROR] missing argument [input] optional=[bandwidth]\n";
    } else {
        std::ifstream input(argv[1]);
        if (input.fail()) std::cerr << "[ERROR] " << strerror(errno) << '\n';

        int bandwidth = (argc == 3) ? std::stoi(argv[3]) : 601;
        std::cout << solve(input, bandwidth) << '\n';
    }
}

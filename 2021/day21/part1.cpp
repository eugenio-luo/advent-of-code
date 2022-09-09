#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

typedef long long ll;

ll rollDie() {
    static ll dieResult = 0;
    
    if (dieResult == 100) dieResult = 0;
    return ++dieResult;
}

ll solve(std::istream& input) {
    std::string line;
    std::vector<ll> positions;
    
    while (std::getline(input, line)) {
        auto found = line.find(':');
        positions.push_back(std::stoll(line.substr(found + 2)));
    }

    ll turn = 0;
    ll die = 0;
    std::vector<ll> scores(positions.size());
    while (1) {
        auto& position = positions[turn];

        for (int i{}; i < 3; ++i) {
            position += rollDie();
            ++die;
        }
        if (position > 10) position %= 10;
        if (position == 0) position = 10;
        
        auto& score = scores[turn];
        score += position;
        
        if (score >= 1000) {
            auto losing = scores[!turn];
            return losing * die;
        }
            
        turn = !turn;
    }

    return 1;
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

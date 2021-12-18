#include <iostream>
#include <fstream>
#include <cstring>
#include <map>

typedef long long ll;

struct Pos {
    ll x;
    ll y;

    Pos(ll x, ll y) : x{x}, y{y} {}

    friend bool operator<(Pos a, Pos b) {
        return (a.x < b.x) || (a.x == b.x && a.y < b.y);
    }
};

bool calcTrajectory(Pos v, const Pos& t1, const Pos& t2) {
    Pos init = Pos(0, 0);

    while (init.x <= t2.x && init.y >= t1.y) {
        init.x += v.x;
        init.y += v.y;

        if (v.x != 0) {
            v.x += (v.x > 0) ? -1 : 1;
        }

        v.y -= 1;
 
        if (init.x >= t1.x && init.x <= t2.x &&
            init.y >= t1.y && init.y <= t2.y) {

            return true;
        }
    }

    return false;
}

constexpr ll maxTries = 250;

ll count(const Pos& t1, const Pos& t2) {
    ll count{};
    
    for (ll x{}; x < maxTries; ++x) {
        for (ll y{-maxTries}; y < maxTries; ++y) {
            count += calcTrajectory(Pos(x, y), t1, t2);
        }
    }
    
    return count;
}

ll solve(std::istream& input) {
    std::string line;
    std::getline(input, line);
    auto equal = line.find('=');
    auto period = line.find('.');
    auto comma = line.find(',');
    ll initX = stoll(line.substr(equal + 1, period - equal));
    ll endX = stoll(line.substr(period + 2, comma - period + 1));

    equal = line.find('=', comma);
    period = line.find('.', comma);
    ll initY = stoll(line.substr(equal + 1, period - equal));
    ll endY = stoll(line.substr(period + 2));

    Pos initTarget = Pos(initX, initY);
    Pos endTarget = Pos(endX, endY);
    
    return count(initTarget, endTarget);
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

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>

typedef long long ll;

ll getIndex(ll actualIndex, ll diceRoll) {
    /* 
       GameStates:
       Index    S s P p 
       0        0 0 1 1
       1        0 0 1 2
       ...
       9        0 0 1 10
       10       0 0 2 1
       ...
       99       0 0 10 10
       100      0 1 1 1
       ...
       2099     20 10 10
       2100     0 1 1
       ...
       44099    20 20 10 10

       examples:
       37       0 0 4 8
       4541     2 3 5 2
       5044     2 8 5 5
       21371    10 3 8 2
       8230     4 0 4 1
       
       formula = p1Points * 2100 + p2Points * 100 + (p1Position - 1) * 10 + (p2Position - 1)
   */
    
    ll newIndex;
    auto oldNextPosition = (actualIndex % 2100 % 100 % 10) + 1;
    auto oldActualPosition = (actualIndex % 2100 % 100 / 10) + 1;
    auto oldNextScore = actualIndex % 2100 / 100;
    auto oldActualScore = actualIndex / 2100;
    
    auto newActualPosition = oldActualPosition + diceRoll;
    if (newActualPosition > 10) newActualPosition %= 10;
    if (newActualPosition == 0) newActualPosition = 10;

    auto newActualScore = oldActualScore + newActualPosition;
    
    if (newActualScore >= 21) {
        return -1;
    }

    newIndex = (oldNextScore * 2100) + (newActualScore * 100) +
        ((oldNextPosition - 1) * 10) + (newActualPosition - 1); 
    
    return newIndex;
}


ll solve(std::istream& input) {
    std::string line;
    std::vector<int> positions;
    
    while (std::getline(input, line)) {
        auto found = line.find(':');
        positions.push_back(std::stoi(line.substr(found + 2)));
    }

    ll neededPointsToWin = 21;
    ll possiblePositions = 10;
    ll gameStatesForOne = neededPointsToWin * possiblePositions;
    ll gameStates = gameStatesForOne * gameStatesForOne;
    std::vector<ll> possibleGameStates(gameStates);
    std::queue<ll> gameStatesQueue;

    ll index = (positions[0] - 1) * 10 + (positions[1] - 1);
    possibleGameStates[index] = 1;
    gameStatesQueue.push(index);

    ll player1Wins{};
    ll player2Wins{};
    ll winner = -1;
    auto player2Queue = gameStatesQueue;
    
    while (!player2Queue.empty()) {
        std::queue<ll> player1Queue;

        while(!player2Queue.empty()) {
            ll i = player2Queue.front();
            player2Queue.pop();
            ll value = possibleGameStates[i];
            possibleGameStates[i] = 0;

            for (int roll{3}; roll <= 9; ++roll) {
                ll index = getIndex(i, roll);

                ll multiplier;
                switch (roll) {
                case 3:
                case 9: multiplier = 1; break;
                case 4:
                case 8: multiplier = 3; break;
                case 5:
                case 7: multiplier = 6; break;
                case 6: multiplier = 7; break;
                }

                if (index == winner) {
                    player1Wins += value * multiplier;
                    continue;
                }

                possibleGameStates[index] += value * multiplier;
                player1Queue.push(index);
            }
        }

        while (!player1Queue.empty()) {
            ll i = player1Queue.front();
            player1Queue.pop();
            ll value = possibleGameStates[i];
            possibleGameStates[i] = 0;
            
            for (int roll{3}; roll <= 9; ++roll) {
                ll index = getIndex(i, roll);
                
                ll multiplier;
                switch (roll) {
                case 3:
                case 9: multiplier = 1; break;
                case 4:
                case 8: multiplier = 3; break;
                case 5:
                case 7: multiplier = 6; break;
                case 6: multiplier = 7; break;
                }

                if (index == winner) {
                    player2Wins += value * multiplier;
                    continue;
                }
                
                possibleGameStates[index] += value * multiplier;
                player2Queue.push(index);
            }
        }
    }
    
    return (player1Wins > player2Wins) ? player1Wins : player2Wins;
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

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <algorithm>

struct Tile {
    int number;
    bool marked;

    Tile(int n) : number{ n }, marked{ false } {} 
};

void boardsParser(std::istream& input, std::vector<Tile>& tiles) {
    std::string line;
    
    while (std::getline(input, line)) {
        if (line.size() < 2) { continue; }

        std::string holder{};
        uint index{};
        while (index < line.size()) {
            
            if (line[index] != ' ') { holder.push_back(line[index++]); continue; }

            ++index;
            if (holder.empty()) { continue; }
                
            tiles.push_back(Tile(std::stoi(holder)));
            holder.clear();
            continue;   
        }

        tiles.push_back(Tile(std::stoi(holder)));
    }
}

int calculateSumBoard(const std::vector<Tile>& tiles, int board) {
    int sum{};
    
    for (int i{board*25}; i < (board+1)*25; ++i) {
        if (!tiles[i].marked) {
            sum += tiles[i].number;
        }
    }

    return sum;
}

int winCondition(const std::vector<Tile>& tiles, std::vector<int>& winnerBoards) {
    int win{};
    
    for (uint i{}; i < tiles.size(); i+=5) {
        for (uint j{i}; j < i+5; ++j) {
            if (!tiles[j].marked) { break; }
            ++win;
        }
        
        int board = i / 25;
        bool alreadyWinner = std::find(winnerBoards.begin(), winnerBoards.end(), board) != winnerBoards.end();
        if (win == 5 && !alreadyWinner) {
            winnerBoards.push_back(board);
            return calculateSumBoard(tiles, board);
        }
        
        win = 0;
    }

    for (uint i{}; i < tiles.size(); ++i) {
        for (uint j{}; j < 5; ++j) {
            if (!tiles[i + j * 5].marked) { break; }
            ++win;
        }
        
        int board = i / 25;
        bool alreadyWinner = std::find(winnerBoards.begin(), winnerBoards.end(), board) != winnerBoards.end();
        if (win == 5 && !alreadyWinner) {
            winnerBoards.push_back(board);
            return calculateSumBoard(tiles, board);
        }
        
        win = 0;
        if ((i + 1) % 5 == 0) { i += 20; };
    }

    return -1;
}

int solve(std::istream& input) {
    std::string line;
    
    std::getline(input, line);
    uint index{};
    std::vector<int> drawnNumber;

    while (index < line.size()) {
        int pos = line.find(',', index);
        if (pos == -1) { pos = line.size(); }

        std::string num = line.substr(index, pos - index);
        
        drawnNumber.push_back(std::stoi(num));
        index = pos + 1;
    }

    std::vector<Tile> tiles;
    boardsParser(input, tiles);
    std::vector<int> winnerBoards{};
    std::vector<int> wins{};
    
    for (int draw : drawnNumber) {
        for (auto& tile : tiles) {
            if ((!tile.marked) && tile.number == draw) { tile.marked = true; }
        }

        int win;
        while (1) {
            win = winCondition(tiles, winnerBoards);
            if (win == -1) { break; }
            wins.push_back(win*draw); 
        }
    }

    return wins.back();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "[ERROR] missing argument\n";
    } else {
        std::ifstream input(argv[1]);
        if (input.fail()) std::cout << strerror(errno);

        std::cout << solve(input) << '\n';
    }
}

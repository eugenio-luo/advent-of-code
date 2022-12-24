#include <iostream>
#include <cstring>
#include <fstream>
#include <queue>
#include <map>
#include <array>

enum {
        UP,
        DOWN,
        LEFT,
        RIGHT,
};

class Knot
{
public:
        int m_x;
        int m_y;
};

void
parse(std::fstream &input, std::queue<int> &commands)
{
        std::string line;
        while (std::getline(input, line)) {
                switch (line[0]) {
                case 'U': commands.push(UP); break; 
                case 'D': commands.push(DOWN); break; 
                case 'L': commands.push(LEFT); break; 
                case 'R': commands.push(RIGHT); break; 
                } 

                commands.push(std::stoi(line.substr(2)));
        }
}

void
move_head(Knot &head, int dir)
{
        switch (dir) {
        case UP:    ++head.m_y; break;
        case DOWN:  --head.m_y; break;
        case LEFT:  --head.m_x; break;
        case RIGHT: ++head.m_x; break;
        }
}

int
move_tail(std::map<long long, bool> &map, Knot &head, Knot &tail, bool check_pos)
{
        int x_diff = head.m_x - tail.m_x;
        int y_diff = head.m_y - tail.m_y;
        
        if (x_diff > 1 || x_diff < -1) {

                (x_diff > 1) ? ++tail.m_x : --tail.m_x;
                if (y_diff != 0)
                        (y_diff > 0) ? ++tail.m_y : --tail.m_y;
                        
        } else if (y_diff > 1 || y_diff < -1) {

                (y_diff > 1) ? ++tail.m_y : --tail.m_y;
                if (x_diff != 0)
                        (x_diff > 0) ? ++tail.m_x : --tail.m_x;
        }

        if (check_pos) {
                unsigned long long num = ((unsigned long long)tail.m_x << 32) | (tail.m_y & 0xFFFFFFFF);
                if (!map[num]) {
                        map[num] = true;
                        return 1;
                }
        }

        return 0;
}

int
solve(std::fstream &input)
{
        std::queue<int> commands;
        std::map<long long, bool> map{};
        parse(input, commands);
        Knot head{};
        std::array<Knot, 9> tails;
        
        int total = 0;
        while (!commands.empty()) {
                int dir = commands.front();
                commands.pop();
                int times = commands.front(); 
                commands.pop();

                for (int i = 0; i < times; ++i) {
                        move_head(head, dir);
                        move_tail(map, head, tails[0], false);
                        for (int i = 1; i < 8; ++i)
                                move_tail(map, tails[i - 1], tails[i], false);
                        total += move_tail(map, tails[7], tails[8], true);
                }
        }

        return total;
}

int
main(int argc, char *argv[])
{
        if (argc < 2) {
                std::cout << "[ERROR] missing argument, should be [input]\n";
                return 1;
        }

        std::fstream input{argv[1]};
        if (input.fail())
                std::cout << "[ERROR] " << strerror(errno) << '\n';

        std::cout << solve(input) << '\n';
}

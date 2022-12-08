#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <stack>

void
parse(std::vector<std::deque<char>>& stacks, std::queue<int>& moves, std::fstream& input)
{
        std::string line;
        
        while (std::getline(input, line) && line.size() > 0) {
                if (line[1] == '1')
                        continue;
                        
                size_t stacks_num = line.size() / 4 + 1;
                if (stacks.size() != stacks_num)
                        stacks.resize(stacks_num);
                
                for (int i = 0; i < stacks_num; ++i) {
                        char c = line[i * 4 + 1];
                        if (c == ' ')
                                continue;

                        stacks[i].push_front(c);
                }
        }

        while (std::getline(input, line)) {

                int s1 = line.find(' ');
                int e1 = line.find(' ', s1 + 1);
                int s2 = line.find(' ', e1 + 1);
                int e2 = line.find(' ', s2 + 1);
                int s3 = line.find(' ', e2 + 1);

                int quantity = std::stoi(line.substr(s1, e1));
                int from = std::stoi(line.substr(s2, e2));
                int to = std::stoi(line.substr(s3));
                moves.push(quantity);
                moves.push(from);
                moves.push(to);
        }
}

std::string
solve(std::fstream& input)
{
        std::vector<std::deque<char>> stacks;
        std::queue<int> moves; 
        parse(stacks, moves, input);

        std::stack<char> tmp;
        while (!moves.empty()) {
                int quantity = moves.front();
                moves.pop();
                int from = moves.front();
                moves.pop();
                int to = moves.front();
                moves.pop();

                for (int i = 0; i < quantity; ++i) {
                        char c = stacks[from - 1].back();
                        stacks[from - 1].pop_back();

                        tmp.push(c);
                }

                for (int i = 0; i < quantity; ++i) {
                        stacks[to - 1].push_back(tmp.top());
                        tmp.pop();
                }
        }

        std::string message;
        for (int i = 0; i < stacks.size(); ++i)
                message += stacks[i].back();
        return message;
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

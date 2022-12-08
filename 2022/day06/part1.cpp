#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <array>
#include <queue>

#define LENGTH 4

int
solve(std::fstream& input)
{
        std::array<int, 26> map{};
        std::queue<char> queue{}; 
        int counter = 0;
        
        std::string line;
        std::getline(input, line);
        for (int i = 0; i < line.size(); ++i) {
                if (i > LENGTH - 1) {
                        if (i - (LENGTH - 1) == map[queue.front() - 'a'])
                                map[queue.front() - 'a'] = 0;

                        queue.pop();
                }
                
                char c = line[i];
                queue.push(c);
                int entry = map[c - 'a'];
                int new_counter = i - entry + 1;
                
                if (entry && counter >= new_counter)
                        counter = new_counter;
                else if (++counter == LENGTH)
                        return i + 1;

                map[c - 'a'] = i + 1;
        }
                       
        return 0;
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

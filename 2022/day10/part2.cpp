#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <array>

long long
add(std::string &line)
{
        return std::stoi(line.substr(5));
}

void
draw(std::array<bool, 40 * 6> &crt, long long reg, long long cycle)
{
        long long pixel = (cycle - 1) % 40;
        if (pixel == reg + 1 || pixel == reg || pixel == reg - 1)
                crt[cycle - 1] = true;
}

long long
solve(std::fstream& input)
{
        std::array<bool, 40 * 6> crt {0};
        
        long long reg = 1;
        long long cycle = 1;

        std::string line;
        while (std::getline(input, line)) {
                char instruction = line[0];
                draw(crt, reg, cycle);
                
                switch (instruction) {
                case 'a':
                        ++cycle;
                        draw(crt, reg, cycle);
                        reg += add(line);
                        break;
                case 'n':
                        break;
                }

                ++cycle;
        }

        for (int y = 0; y < 6; ++y) {
                for (int x = 0; x < 40; ++x)
                        std::cout << (crt[y * 40 + x] ? '#' : '.'); 
                
                std::cout << '\n';
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

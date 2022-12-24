#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

long long
add(std::string &line)
{
        return std::stoi(line.substr(5));
}

long long
inc_cycle(long long &cycle, long long reg)
{
        ++cycle;
        if ((cycle - 20) % 40)
                return 0;
        
        return reg * cycle;
}

long long
solve(std::fstream& input)
{
        long long total = 0; 
        long long reg = 1;
        long long cycle = 1;

        std::string line;
        while (std::getline(input, line)) {
                char instruction = line[0];

                switch (instruction) {
                case 'a':
                        total += inc_cycle(cycle, reg);
                        reg += add(line);
                        break;
                case 'n':
                        break;
                }

                total += inc_cycle(cycle, reg);
        }

        return total;
}

/*
long long
solve(std::fstream& input)
{
        long long total = 0; 
        long long reg = 1;
        long long cycle = 1;
        long long store = 0;
        
        std::string line;
        while (std::getline(input, line)) {
                char instruction = line[0];

                reg += store;
                std::cout << cycle << ' ' << reg << '\n';
                store = 0;
                switch (instruction) {
                case 'a':
                        store += add(line);
                        break;
                case 'n':
                        break;
                }

                if ((cycle - 20) % 40 == 0) {
                        std::cout << cycle << ": " << reg << '\n';
                        total += reg * cycle;
                }
                ++cycle;
        }

        return total;
}
*/

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

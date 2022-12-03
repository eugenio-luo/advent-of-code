#include <iostream>
#include <fstream>
#include <cstring>
#include <array>

static std::array<int, 52> alphabet; 

int
calc_point(char c)
{
        if (c >= 'a')
                return c - 'a';

        return c - 'A' + 26;
}

int
solve(std::istream& input)
{
        int priorities = 0;

        std::string line;
        while (std::getline(input, line)) {

                int size = line.size();
                for (int i = 0; i < size / 2; ++i)
                        ++alphabet[calc_point(line[i])];

                for (int i = size / 2; i < size; ++i)
                        if (alphabet[calc_point(line[i])]) {
                                priorities += calc_point(line[i]) + 1;
                                break;
                        }

                alphabet.fill(0);
        }
        
        return priorities;
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

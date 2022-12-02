#include <iostream>
#include <fstream>
#include <cstring>

int solve(std::istream& input)
{
        int points = 0;

        std::string line;
        while (std::getline(input, line)) {

                int opp_shape = line[0] - 'A'; 
                int round_end = line[2] - 'X';

                /* strange formula because we need to do "modulo" of negative number */
                int user_shape = (((opp_shape - 1 + round_end) % 3) + 3) % 3;
                points += round_end * 3 + user_shape + 1;
        }

        return points;
}

int main(int argc, char *argv[])
{
        if (argc < 2) {
                std::cout << "[ERROR] missing argument, should be [input]\n";
                return 1;
        }
                
        std::fstream input(argv[1]);
        if (input.fail())
                std::cout << "[ERROR] " << strerror(errno) << '\n';

        std::cout << solve(input) << '\n';
}

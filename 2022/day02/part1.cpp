#include <iostream>
#include <fstream>
#include <cstring>

int solve(std::istream& input)
{
        int points = 0;

        std::string line;
        while (std::getline(input, line)) {

                int opp_shape = line[0] - 'A'; 
                int user_shape = line[2] - 'X';

                points += user_shape + 1; 
                
                if (opp_shape == user_shape)
                        points += 3;
                else if ((opp_shape + 1) % 3 == user_shape)
                        points += 6;
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

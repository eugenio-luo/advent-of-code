#include <iostream>
#include <fstream> 
#include <string>
#include <cstring>

int
solve(std::istream& input)
{
        int max = 0;
        int value;
        
        std::string line;
        while (std::getline(input, line)) {

                if (line != "")
                        value += std::stoi(line);
                else {
                        if (value > max)
                                max = value;

                        value = 0;
                }
        }

        if (value > max)
                max = value;
        
        return max;
}

int
main(int argc, char *argv[])
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

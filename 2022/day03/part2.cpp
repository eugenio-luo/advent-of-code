#include <iostream>
#include <fstream>
#include <cstring>
#include <array>

static std::array<int, 52 * 2> alphabet; 

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
        int counter = 0;
        while (std::getline(input, line)) {

                int size = line.size();
                if (counter < 2) {
                        for (int i = 0; i < size; ++i)
                                ++alphabet[calc_point(line[i]) * 2 + counter];

                        ++counter;
                        continue;
                }

                for (int i = 0; i < size; ++i) {
                        int index = calc_point(line[i]) * 2;

                        if (alphabet[index] && alphabet[index + 1]) {
                                priorities += calc_point(line[i]) + 1;
                                std::cout << line[i] << '\n';
                                break;
                        }
                }
                
                counter = 0;
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

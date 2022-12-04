#include <iostream>
#include <fstream>
#include <cstring>

int
solve(std::istream& input)
{
        int fully_contains = 0;

        std::string line;
        while (std::getline(input, line)) {

                size_t pos1 = line.find("-");
                size_t pos2 = line.find(",");
                size_t pos3 = line.find("-", pos2);

                int s1 = std::stoi(line.substr(0, pos1));
                int e1 = std::stoi(line.substr(pos1 + 1, pos2));

                int s2 = std::stoi(line.substr(pos2 + 1, pos3));
                int e2 = std::stoi(line.substr(pos3 + 1));

                if (((s1 < s2) ^ (e1 < e2)) || (s1 == s2) || (e1 == e2))
                        ++fully_contains;
        }

        return fully_contains;
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

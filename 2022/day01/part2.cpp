#include <iostream>
#include <fstream> 
#include <string>
#include <cstring>
#include <queue> 

int
solve(std::istream& input)
{
        int value;
        std::priority_queue<int> values;
        
        std::string line;
        while (std::getline(input, line)) {

                if (line != "")
                        value += std::stoi(line);
                else {
                        values.push(value);
                        value = 0;
                }

        }
       
        values.push(value);

        int max = 0;
        for (int i = 0; i < 3; ++i, values.pop())
                max += values.top();
        
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

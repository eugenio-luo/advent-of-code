#include <iostream>
#include <chrono>
#include <sstream>
#include <chrono>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "\033[31;1m[ERROR]\033[0m missing argument [year] [num]\n";
    } else {
        std::vector<long double> timings;

        double progress{};
        double progressStep = (1.0 / (std::stod(argv[2]) - 1)) + 0.001;
        int barWidth = 70;
        std::cout << "\nCOMPILING AND EXECUTING\n";
        for (int i{1}; i <= std::stoi(argv[2]); ++i) {
            for (int j{1}; j <= 2; ++j) {
                std::stringstream com;
                com << "g++ " << argv[1] << "/day";
                if (i < 10) com << "0";
                com << i << "/part" << j << ".cpp -o a.out";
                std::system(com.str().c_str());

                com.str("");
                com << "./a.out " << argv[1] << "/day";
                if (i < 10) com << "0";
                com << i << "/input >> results";
                auto start = std::chrono::high_resolution_clock::now();
                std::system(com.str().c_str());
                auto elapsed = std::chrono::high_resolution_clock::now() - start;

                long double microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
                timings.push_back(microseconds / 1000000.0);
            }
            
            std::cout << "[";
            int pos = barWidth * progress;
            for (int i{}; i < barWidth; ++i) {
                if (i < pos) std::cout << "=";
                else if (i == pos) std::cout << ">";
                else std::cout << " ";
            }
            
            std::string color = (progress > 0.98) ? "32" : "33";
            std::cout << "] \033[1;" << color << 'm' << (int)(progress * 100.0) << " %\033[0m\r";
            std::cout.flush();

            progress += progressStep;
        }
        std::cout << "\n\n";

        int day{1};
        int part{1};
        long double total{};
        std::cout << "year\tday\tpart\ts\n";
        for (auto ms : timings) {
            std::cout << argv[1] << '\t' << day << '\t' << part << "\t\033[1;32m" << ms << "\n\033[0m";
            ++part;
            if (part == 3) {
                ++day;
                part = 1;
            }

            total+= ms;
        }
        std::cout << "\n\033[1;31mTOTAL:\t\t\t" << total << "\n\n";
    }
}

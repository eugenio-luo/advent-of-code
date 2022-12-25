#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <queue>

using llong = long long;

enum OP_TYPE {
        ADD,
        MULT,
        SELF,
};

class Monkey
{
public:
        std::queue<llong> items;
        OP_TYPE op_type;
        llong op_num;
        llong test_num;
        llong true_mon;
        llong false_mon;
        llong inspected;
};

void
check_item(std::string &line, Monkey &monkey)
{
        int start = 18, end = line.find(',', start);
        for (; start; start = end + 1, end = line.find(',', start)) {
                long long item = std::stoi(line.substr(start, end)); 
                monkey.items.push(item);
        } 
}

void
check_op(std::string &line, Monkey &monkey)
{
        if (line[25] == 'o') {
                monkey.op_type = SELF;
                return;
        }
        
        switch (line[23]) {
        case '*':
                monkey.op_type = MULT;
                break;
        case '+':
                monkey.op_type = ADD;
                break;
        }

        monkey.op_num = std::stoi(line.substr(25));
}
void
parse(std::fstream &input, std::vector<Monkey> &monkeys)
{
        std::string line;
        while (std::getline(input, line)) {
                monkeys.push_back(Monkey{});
                Monkey &monkey = monkeys.back();
                
                std::getline(input, line);
                check_item(line, monkey);
                
                std::getline(input, line);
                check_op(line, monkey);
                
                std::getline(input, line);
                monkey.test_num = std::stoi(line.substr(21));
               
                std::getline(input, line);
                monkey.true_mon = std::stoi(line.substr(29));
                
                std::getline(input, line);
                monkey.false_mon = std::stoi(line.substr(30));

                std::getline(input, line);
        }
}

llong
operation(llong item, OP_TYPE op_type, llong op_num)
{
        switch (op_type) {
        case ADD:
                return item + op_num;
        case MULT:
                return item * op_num;
        case SELF:
                return item * item;
        default:
                return 0;
        }
}

void
monkey_inspection(std::vector<Monkey> &monkeys, Monkey &monkey, llong total_divisor)
{
        while (!monkey.items.empty()) {
                ++monkey.inspected;
                llong item = monkey.items.front();
                monkey.items.pop();
                
                item = operation(item, monkey.op_type, monkey.op_num);
                int receiver = (item % monkey.test_num) ? monkey.false_mon : monkey.true_mon;
                item %= total_divisor;
                
                monkeys[receiver].items.push(item);
        }
}

void
monkeys_inspection(std::vector<Monkey> &monkeys, llong total_divisor)
{
        for (auto &monkey : monkeys)
                monkey_inspection(monkeys, monkey, total_divisor);
}

llong
solve(std::fstream &input)
{
        std::vector<Monkey> monkeys;
        parse(input, monkeys);
        llong total_divisor = 1;
        for (auto &monkey : monkeys)
                total_divisor *= monkey.test_num;
        
        for (int i = 0; i < 10000; ++i)
                monkeys_inspection(monkeys, total_divisor);
                
        llong max_1 = 0, max_2 = 0;
        for (auto &monkey : monkeys) {
                if (monkey.inspected > max_1) {
                        max_2 = max_1;
                        max_1 = monkey.inspected;
                        continue;
                }

                if (monkey.inspected > max_2)
                        max_2 = monkey.inspected;
        }

        return max_1 * max_2;
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

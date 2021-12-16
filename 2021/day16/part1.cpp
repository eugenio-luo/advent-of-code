#include <iostream>
#include <fstream>
#include <cstring>
#include <queue>
#include <vector>

constexpr int hexSizeInBit = 4;

void push(std::queue<bool>& source, const bool (&binary)[hexSizeInBit]) {
    for (int i{}; i < hexSizeInBit; ++i) {
        source.push(binary[i]);
    }
}

void hex2bin(std::istream& input, std::queue<bool>& source) {
    char c;
    while (input >> c) {
        switch (c) {
        case '0': push(source, {0, 0, 0, 0}); break;
        case '1': push(source, {0, 0, 0, 1}); break;
        case '2': push(source, {0, 0, 1, 0}); break;
        case '3': push(source, {0, 0, 1, 1}); break;
        case '4': push(source, {0, 1, 0, 0}); break;
        case '5': push(source, {0, 1, 0, 1}); break;
        case '6': push(source, {0, 1, 1, 0}); break;
        case '7': push(source, {0, 1, 1, 1}); break;
        case '8': push(source, {1, 0, 0, 0}); break;
        case '9': push(source, {1, 0, 0, 1}); break;
        case 'A': push(source, {1, 0, 1, 0}); break;
        case 'B': push(source, {1, 0, 1, 1}); break;
        case 'C': push(source, {1, 1, 0, 0}); break;
        case 'D': push(source, {1, 1, 0, 1}); break;
        case 'E': push(source, {1, 1, 1, 0}); break;
        case 'F': push(source, {1, 1, 1, 1}); break;
        }
    }
}

int bin2int(const std::vector<bool>& binary) {
    int ret{};
    for (int i{}; i < binary.size(); ++i) {
        ret += binary[i] * (1 << (binary.size() - i - 1));
    }

    return ret;
}

class Packet {
public:
    int version_;
    int typeID_;

    virtual ~Packet() = default;
    virtual void print() = 0;
    
protected:
    Packet(int v, int t) : version_{v}, typeID_{t} {}
};

class Literal : public Packet {
public:
    int value_{0};

    Literal(int v, int t) : Packet(v, t) {}
    Literal(int v, int t, int val) : Packet(v, t), value_{val} {}
    ~Literal() = default;

    void print() {
        std::cout << version_ << ' ' << typeID_ << ' ' << value_;
    }
};

class Operator : public Packet {
public:
    int lengthTypeID_ = 0;
    int lengthField_ = 0;
    
    Operator(int v, int t) : Packet(v, t) {}
    Operator(int v, int t, int lt, int lf)
        : Packet(v, t), lengthTypeID_{lt}, lengthField_{lf} {}

    void print() {
        std::cout << version_ << ' ' << typeID_ << ' ' << lengthTypeID_ << ' ' << lengthField_;
    }
};

constexpr int packetInfoLength = 3;
constexpr int literalInfoLength = 4;
constexpr int totalLenBitsLength = 15;
constexpr int numberOfSubPackLength = 11;

int getBits(std::queue<bool>& source, int length) {
    std::vector<bool> bits;
    for (int i{}; i < length; ++i) {
        bits.push_back(source.front());
        source.pop();
    }
    return bin2int(bits);
}

/*
void ignore(std::queue<bool>& source, int n) {
    for (int i{}; i < n; ++i) {
        source.pop();
    }
}
*/

bool match(std::queue<bool>& source, bool match) {
    if (source.front() != match) return false;
    source.pop();
    return true;
}

int getValue(std::queue<bool>& source) {
    std::vector<bool> bits;
    while (match(source, 1)) {
        for (int i{}; i < literalInfoLength; ++i) {
            bits.push_back(source.front());
            source.pop();
        }
    }

    source.pop();
    for (int i{}; i < literalInfoLength; ++i) {
        bits.push_back(source.front());
        source.pop();
    }
    
    return bin2int(bits);
}

void parse(std::queue<bool>& source, std::vector<Packet*>& code) {
    while (!source.empty()) {
        if (source.size() <= 4) break;
        int version = getBits(source, packetInfoLength);
        int typeID = getBits(source, packetInfoLength);
        
        switch (typeID) {
        case 4: {
            int value = getValue(source);
            auto* literal = new Literal(version, typeID, value);
            code.push_back(literal);
            break;
        }
        default: {
            int lengthTypeID = getBits(source, 1);
            int lengthField = (lengthTypeID) ? getBits(source, 11) : getBits(source, 15);
            auto* op = new Operator(version, typeID, lengthTypeID, lengthField);
            code.push_back(op);
            break;
        }
            /*
        default: {
            std::cout << "no operation with such id\n";
            throw -1;
        }
            */
        }
    }
}

int solve(std::istream& input) {
    std::queue<bool> source;
    hex2bin(input, source);

    std::vector<Packet*> code;
    parse(source, code);

    int sum{};
    for (int i{}; i < code.size(); ++i) {
        sum += code[i]->version_;
        delete code[i];
    }

    return sum;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "[ERROR] missing argument [input]\n"; 
    } else {
        std::ifstream input(argv[1]);
        if (input.fail()) std::cerr << "[ERROR] " << strerror(errno) << '\n';

        try {
            std::cout << solve(input) << '\n';
        } catch (int i) {
            std::cout << i << '\n';
        }
    }
}

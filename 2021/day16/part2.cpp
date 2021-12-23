#include <iostream>
#include <fstream>
#include <cstring>
#include <queue>
#include <vector>
#include <functional>

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

long long bin2int(const std::vector<bool>& binary) {
    long long ret{};
    for (int i{}; i < binary.size(); ++i) {
        ret += binary[i] * ((long long)1 << (binary.size() - i - 1));
    }
    
    return ret;
}

class Packet {
public:
    int version_;
    int typeID_;

    virtual ~Packet() = default;
    
protected:
    Packet(int v, int t) : version_{v}, typeID_{t} {}
};

class Literal : public Packet {
public:
    long long value_;

    Literal(int v, int t, long long val) : Packet(v, t), value_{val} {}
};

class Operator : public Packet {
public:
    std::vector<Packet*> operands_;
    
    Operator(int v, int t)
        : Packet(v, t), operands_{} {}
    ~Operator() {
        for (auto& op : operands_) {
            delete op;
        }
    }
};

constexpr int packetInfoLength = 3;
constexpr int literalInfoLength = 4;
constexpr int totalLenBitsLength = 15;
constexpr int numberOfSubPackLength = 11;

long long getBits(std::queue<bool>& source, int length) {
    std::vector<bool> bits;
    for (int i{}; i < length; ++i) {
        bits.push_back(source.front());
        source.pop();
    }
    return bin2int(bits);
}

bool match(std::queue<bool>& source, bool match) {
    if (source.front() != match) return false;
    source.pop();
    return true;
}

long long getValue(std::queue<bool>& source) {
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

bool parse(std::queue<bool>& source, std::vector<Packet*>& code) {
    int version = getBits(source, packetInfoLength);
    int typeID = getBits(source, packetInfoLength);
    
    switch (typeID) {
    case 4: {
        long long value = getValue(source);
        auto* literal = new Literal(version, typeID, value);
        code.push_back(literal);
        return source.size() > 4;
    }
        
    default: {
        int lengthTypeID = getBits(source, 1);
        auto* op = new Operator(version, typeID);

        if (lengthTypeID) {
            int lengthField = getBits(source, numberOfSubPackLength);
            for (int i{}; i < lengthField; ++i) {
                if (source.size() < 4) break;
                parse(source, op->operands_);
            }
            
        } else {
            int lengthField = getBits(source, totalLenBitsLength);
            for (int size = source.size(); size - source.size() != lengthField;) {
                if (source.size() < 4) break;
                parse(source, op->operands_);
            }
        }
        
        code.push_back(op);
        return false;
    }
    }

    return false;
}

long long compile(Packet* packet);

void min(long long& a, long long b) {
    if (b < a) {
        a = b;
    }
}

void max(long long& a, long long b) {
    if (b > a) {
        a = b;
    }
}

long long opPacket(Operator* packet, std::function<void(long long&, long long)> func, long long start = 0) {
    long long ret = start;
    
    for (auto operand : packet->operands_) {
        func(ret, compile(operand));
    }
    
    return ret;
}

long long compPacket(Operator* packet, std::function<bool(long long, long long)> func) {
    long long a = compile(packet->operands_[0]);
    long long b = compile(packet->operands_[1]);
    
    return func(a, b);
}

long long compile(Packet* packet) {
    
    switch (packet->typeID_) {
    case 0: return opPacket(static_cast<Operator*>(packet), [](long long& a, long long b){ a += b; });
    case 1: return opPacket(static_cast<Operator*>(packet), [](long long& a, long long b){ a *= b; }, 1);
    case 2: return opPacket(static_cast<Operator*>(packet), min, INT64_MAX);
    case 3: return opPacket(static_cast<Operator*>(packet), max);
    case 4: return static_cast<Literal*>(packet)->value_;
    case 5: return compPacket(static_cast<Operator*>(packet), [](long long a, long long b){ return a > b; });
    case 6: return compPacket(static_cast<Operator*>(packet), [](long long a, long long b){ return a < b; });
    case 7: return compPacket(static_cast<Operator*>(packet), [](long long a, long long b){ return a == b; });

    default: {
        std::cerr << "no operator with such ID\n";
        throw -1;
    }
    }
}

long long solve(std::istream& input) {
    std::queue<bool> source;
    hex2bin(input, source);

    std::vector<Packet*> code;
    while (parse(source, code)); // no body
   
    long long result{};
    for (auto packet : code) {
        result += compile(packet);
    }

    for (auto packet : code) delete packet;

    return result;
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

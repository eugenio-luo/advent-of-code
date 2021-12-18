#include <iostream>
#include <fstream>
#include <cstring>
#include <queue>

typedef long long ll;

struct Pair;

union Value {
    Pair* pair;
    int num;
};

enum ValueType {
    uninitValue_t,
    numberValue_t,
    pairValue_t
};

struct Pair {
    Value left;
    Value right;
    Pair* parent;
    int leftType = uninitValue_t;
    int rightType = uninitValue_t;
    int size;
    
    ~Pair() {
        if (leftType == pairValue_t) delete left.pair;
        if (rightType == pairValue_t) delete right.pair;
    }
    
    friend void print(Pair* p) {
        std::cout << '[';
        if (p->leftType == numberValue_t) {
            std::cout << p->left.num;
        } else {
            print(p->left.pair);
        }
        
        std::cout << ',';
        if (p->rightType == numberValue_t) {
            std::cout << p->right.num;
        } else {
            print(p->right.pair);
        }
        
        std::cout << ']';
    }

    void insertNum(bool isLeft, int num) {
        if (isLeft) {
            this->leftType = numberValue_t;
            this->left.num = num;
        } else {
            this->rightType = numberValue_t;
            this->right.num = num;
        }
    }
    
    Pair* insertPair(bool isLeft, Pair* pair = nullptr) {
        Pair* newPair;
        if (pair == nullptr) {
            newPair = new Pair();
        } else {
            newPair = pair;
        }
        newPair->size = this->size + 1;
        newPair->parent = this;

        if (isLeft) {
            this->leftType = pairValue_t;
            this->left.pair = newPair;
        } else {
            this->rightType = pairValue_t;
            this->right.pair = newPair;
        }
        
        return newPair;
    }
};

void parse(const std::string& line, std::queue<Pair*>& numbers, Pair* basePair) {
    auto* actualPair = basePair;
    bool isLeft = true;

    for (int i{1}; i < line.size() - 1; ++i) {
        switch (line[i]) {
        case '[': {
            auto* newPair = actualPair->insertPair(isLeft);
            isLeft = true;
            actualPair = newPair;
            break;
        }
        case ']': {
            actualPair = actualPair->parent;
            break;
        }
        case ',': {
            isLeft = false;
            break;
        }
        default: {
            actualPair->insertNum(isLeft, line[i] - '0');
            isLeft = true;
            break;
        }
        }
    }
}

void convert(std::istream& input, std::queue<Pair*>& numbers) {
    std::string line;
    while (std::getline(input, line)) {
        auto basePair = new Pair();
        basePair->parent = nullptr;
        basePair->size = 0;
        
        parse(line, numbers, basePair);

        numbers.push(basePair);
    }
}

void updateSize(Pair* pair) {
    ++pair->size;

    if (pair->leftType == pairValue_t) {
        updateSize(pair->left.pair);
    }
    if (pair->rightType == pairValue_t) {
        updateSize(pair->right.pair);
    } 
}

Pair* addition(Pair* a, Pair* b) {
    auto* newPair = new Pair();
    newPair->size = a->size;
    newPair->insertPair(true, a);
    updateSize(a);
    --a->size;
    newPair->insertPair(false, b);
    updateSize(b);
    --b->size;
    return newPair;
}

void explode(Pair* pair) {
    auto* parentPair = pair->parent;
    
    if (parentPair->leftType == pairValue_t && parentPair->left.pair == pair) {
        auto* rightPair = parentPair;
        if (rightPair->rightType != numberValue_t) {
            rightPair = parentPair->right.pair;
            while (rightPair->leftType != numberValue_t) {
                rightPair = rightPair->left.pair;
            }
            rightPair->left.num += pair->right.num;
        } else {
            rightPair->right.num += pair->right.num;
        }
        
        auto* actualPair = parentPair;
        while (actualPair->parent != nullptr && actualPair->parent->left.pair == actualPair) {
            actualPair = actualPair->parent;
        }

        if (actualPair->parent != nullptr) {
            if (actualPair->parent->leftType == pairValue_t) {
                actualPair = actualPair->parent->left.pair;
                while (actualPair->rightType != numberValue_t) {
                    actualPair = actualPair->right.pair;
                }
                actualPair->right.num += pair->left.num;
            } else {
                actualPair = actualPair->parent;
                actualPair->left.num += pair->left.num;
            }
        } 
            
        parentPair->left.num = 0;
        parentPair->leftType = numberValue_t;
        
    } else {
        auto* leftPair = parentPair;
        if (leftPair->leftType != numberValue_t) {
            leftPair = leftPair->left.pair;
            while (leftPair->rightType != numberValue_t) {
                leftPair = leftPair->right.pair;
            }
            leftPair->right.num += pair->left.num;
        } else {
            leftPair->left.num += pair->left.num;
        }
        
        auto* actualPair = parentPair;
        while (actualPair->parent != nullptr && actualPair->parent->right.pair == actualPair) {
            actualPair = actualPair->parent;
        }

        if (actualPair->parent != nullptr) {
            if (actualPair->parent->rightType == pairValue_t) {
                actualPair = actualPair->parent->right.pair;
                while (actualPair->leftType != numberValue_t) {
                    actualPair = actualPair->left.pair;
                }
                actualPair->left.num += pair->right.num;
            } else {
                actualPair = actualPair->parent;
                actualPair->right.num += pair->right.num;
            }
        }
            
        parentPair->right.pair = nullptr;
        parentPair->right.num = 0;
        parentPair->rightType = numberValue_t;
    }
}

void split(Pair* pair) {
    if (pair->leftType == numberValue_t && pair->left.num > 9) {
        int a = pair->left.num / 2;
        int b = pair->left.num / 2;
        if (pair->left.num % 2 == 1) ++b;

        pair->leftType = pairValue_t;
        auto* newPair = new Pair();
        pair->left.pair = newPair;
        newPair->size = pair->size + 1;
        newPair->parent = pair;
        newPair->leftType = numberValue_t;
        newPair->left.num = a;
        newPair->rightType = numberValue_t;
        newPair->right.num = b;
    } else {
        int a = pair->right.num / 2;
        int b = pair->right.num / 2;
        if (pair->right.num % 2 == 1) ++b;

        pair->rightType = pairValue_t;
        auto* newPair = new Pair();
        pair->right.pair = newPair;
        newPair->size = pair->size + 1;
        newPair->parent = pair;
        newPair->leftType = numberValue_t;
        newPair->left.num = a;
        newPair->rightType = numberValue_t;
        newPair->right.num = b;
    }
}

Pair* searchForExplosion(Pair* pair) {
    if (pair->size >= 4) {
        return pair;
    }
    if (pair->leftType == pairValue_t) {
        auto* ret = searchForExplosion(pair->left.pair);
        if (ret != nullptr) return ret;
    }
    if (pair->rightType == pairValue_t) {
        auto* ret = searchForExplosion(pair->right.pair);
        if (ret != nullptr) return ret;
    }

    return nullptr;
}

Pair* searchForSplit(Pair* pair) {
    if (pair->leftType == numberValue_t && pair->left.num > 9) {
        return pair;
    }
    if (pair->leftType == pairValue_t) {
        auto* ret = searchForSplit(pair->left.pair);
        if (ret != nullptr) return ret;
    }

    if (pair->rightType == numberValue_t && pair->right.num > 9) {
        return pair;
    }
    if (pair->rightType == pairValue_t) {
        auto* ret = searchForSplit(pair->right.pair);
        if (ret != nullptr) return ret;
    }

    return nullptr;
}

ll magnitude(Pair* pair) {
    ll result{};
    if (pair->leftType == numberValue_t) {
        result += 3 * pair->left.num;
    } else {
        result += 3 * magnitude(pair->left.pair);
    }

    if (pair->rightType == numberValue_t) {
        result += 2 * pair->right.num;
    } else {
        result += 2 * magnitude(pair->right.pair);
    }

    return result;
}

ll solve(std::istream& input) {
    std::queue<Pair*> numbers;
    convert(input, numbers);

    auto* a = numbers.front();
    numbers.pop();
    while (!numbers.empty()) {
        auto* b = numbers.front();
        numbers.pop();
        a = addition(a, b);
        
        Pair *c, *d; 
        while (true) {
            c = searchForExplosion(a);
            if (c != nullptr) {
                explode(c);
                continue;
            }
            
            d = searchForSplit(a);
            if (d != nullptr) {
                split(d);
                continue;
            }
            break;
        }
    }

    ll ret = magnitude(a);
    
    return ret;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "[ERROR] missing argument [input]\n";
    } else {
        std::ifstream input(argv[1]);
        if (input.fail()) std::cerr << "[ERROR] " << strerror(errno) << '\n';
        std::ofstream output("result.txt");
        
        std::cout << solve(input) << '\n';
    }
}

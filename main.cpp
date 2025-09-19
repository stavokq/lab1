#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
#include <cstdint>
using namespace std;

struct Cell {
    int x;
    int y;
};

class RandomCell {
public:
    explicit RandomCell(int N)
        : N_(N)
    {
        if (N_ <= 0) {
            throw std::invalid_argument("RandomCell: N must be > 0");
        }
        std::random_device rd;
        engine_.seed(rd());
        dist_ = std::uniform_int_distribution<int>(0, N_ - 1);
    }

    Cell operator()() {
        return Cell{ dist_(engine_), dist_(engine_) };
    }

private:
    int N_;
    std::mt19937 engine_;
    std::uniform_int_distribution<int> dist_;
};


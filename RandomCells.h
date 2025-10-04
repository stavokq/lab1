#pragma once
#include "Cell.h"
#include <random>

class RandomCell {
public:
    explicit RandomCell(int N);
    Cell operator()();

private:
    int N_;
    std::mt19937 engine_;
    std::uniform_int_distribution<int> dist_;
};

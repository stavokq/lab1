#pragma once
#include "Cell.h"
#include <random>

class RandomCells {
public:
    explicit RandomCells(int boardSize);
    Cell operator()();

private:
    int boardSize_;
    std::mt19937 engine_;
    std::uniform_int_distribution<int> dist_;
};

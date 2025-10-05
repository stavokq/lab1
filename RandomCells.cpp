#include "RandomCells.h"
#include <stdexcept>

RandomCells::RandomCells(int boardSize)
    : boardSize_(boardSize)
{
    if (boardSize_ <= 0) {
        throw std::invalid_argument("Board size must be > 0");
    }
    std::random_device rd;
    engine_.seed(rd());
    dist_ = std::uniform_int_distribution<int>(0, boardSize_ - 1);
}

Cell RandomCells::operator()() {
    return Cell{ dist_(engine_), dist_(engine_) };
}

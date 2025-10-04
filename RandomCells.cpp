#include "RandomCells.h"
#include <stdexcept>

RandomCell::RandomCell(int N)
    : N_(N)
{
    if (N_ <= 0) {
        throw std::invalid_argument("N must be > 0");
    }
    std::random_device rd;
    engine_.seed(rd());
    dist_ = std::uniform_int_distribution<int>(0, N_ - 1);
}

Cell RandomCell::operator()() {
    return Cell{ dist_(engine_), dist_(engine_) };
}

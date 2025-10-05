#pragma once
#include <vector>
#include "Cell.h"

class RandomCells; 

class CellStats {
public:
    explicit CellStats(int N);

    void add(Cell c);
    void sample(RandomCells& rng, int M);

    double meanMultiplicity() const;
    double medianMultiplicity() const;

private:
    int n;
    std::vector<int> counts;

    int indexOf(Cell c) const;
    void checkCell(Cell c) const;
};

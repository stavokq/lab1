#include "Stats.h"
#include "RandomCells.h"
#include <stdexcept>
#include <algorithm>

CellStats::CellStats(int N) : n(N) {
    if (N <= 0) {
        throw std::invalid_argument("N must be > 0");
    }
    counts.resize(n * n, 0);
}

void CellStats::add(Cell c) {
    checkCell(c);
    int idx = indexOf(c);
    ++counts[idx]; 
}

void CellStats::sample(RandomCells& rng, int M) {
    for (int i = 0; i < M; ++i) {
        Cell c = rng();
        add(c);
    }
}

double CellStats::meanMultiplicity() const {
    long long total = 0;
    for (int v : counts) {
        total += v;
    }
    return static_cast<double>(total) / counts.size();
}

double CellStats::medianMultiplicity() const {
    std::vector<int> sorted = counts;
    std::sort(sorted.begin(), sorted.end());
    int m = sorted.size();

    if (m % 2 == 1) {
        return sorted[m / 2];
    } else {
        return (sorted[m / 2 - 1] + sorted[m / 2]) / 2.0;
    }
}

int CellStats::indexOf(Cell c) const {
    return c.x * n + c.y;
}

void CellStats::checkCell(Cell c) const {
    if (c.x < 0 || c.y < 0 || c.x >= n || c.y >= n) {
        throw std::out_of_range("Cell out of range");
    }
}

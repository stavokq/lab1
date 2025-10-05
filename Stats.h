#pragma once
#include <vector>
#include <string>
#include "Cell.h"
#include "RandomCells.h"

class CellStats {
public:
    explicit CellStats(int N);

    void add(Cell c);
    void sample(RandomCells& rng, int M);

    double meanMultiplicity() const;
    double medianMultiplicity() const;

    bool saveCountsCSV(const std::string& path) const;
    bool loadCountsCSV(const std::string& path);
    bool saveSummaryTXT(const std::string& path) const;

private:
    int n;
    std::vector<int> counts;

    int indexOf(Cell c) const;
    void checkCell(Cell c) const;
};

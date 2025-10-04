#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
#include <cstdint>

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
            throw std::invalid_argument("N must be > 0");
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

class CellStats {
public:
    CellStats(int N) {
        if (N <= 0) {
            throw std::invalid_argument("N must be > 0");
        }
        n = N;
        counts.resize(n * n, 0);
    }

    void add(Cell c) {
        checkCell(c);
        int idx = indexOf(c);
        counts[idx] = counts[idx] + 1;
    }

    void sample(class RandomCell& rng, int M) {
        for (int i = 0; i < M; i++) {
            Cell c = rng();
            add(c);
        }
    }

    double meanMultiplicity() {
        int total = 0;
        for (int i = 0; i < counts.size(); i++) {
            total = total + counts[i];
        }
        return (double)total / counts.size();
    }

    double medianMultiplicity() {
        std::vector<int> sorted = counts; // копія
        std::sort(sorted.begin(), sorted.end());
        int m = sorted.size();

        if (m % 2 == 1) {
            return sorted[m / 2];
        } else {
            return (sorted[m / 2 - 1] + sorted[m / 2]) / 2.0;
        }
    }

private:
    int n;
    std::vector<int> counts;

    int indexOf(Cell c) {
        return c.x * n + c.y;
    }

    void checkCell(Cell c) {
        if (c.x < 0 || c.y < 0 || c.x >= n || c.y >= n) {
            throw std::out_of_range("Cell out of range");
        }
    }
};

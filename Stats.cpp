#include "Stats.h"
#include "RandomCells.h"
#include <stdexcept>
#include <algorithm>
#include <fstream>
#include <limits>

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

void CellStats::sample(RandomCell& rng, int M) {
    for (int i = 0; i < M; ++i) {
        Cell c = rng();
        add(c);
    }
}

double CellStats::meanMultiplicity() const {
    long long total = 0;
    for (int v : counts) total += v;
    return static_cast<double>(total) / static_cast<double>(counts.size());
}

double CellStats::medianMultiplicity() const {
    std::vector<int> sorted = counts;
    std::sort(sorted.begin(), sorted.end());
    const std::size_t m = sorted.size();
    if (m % 2 == 1) {
        return static_cast<double>(sorted[m / 2]);
    } else {
        return (static_cast<double>(sorted[m / 2 - 1]) +
                static_cast<double>(sorted[m / 2])) / 2.0;
    }
}

int CellStats::indexOf(Cell c) const { return c.x * n + c.y; }

void CellStats::checkCell(Cell c) const {
    if (c.x < 0 || c.y < 0 || c.x >= n || c.y >= n) {
        throw std::out_of_range("Cell out of range");
    }
}

bool CellStats::saveCountsCSV(const std::string& path) const {
    std::ofstream out(path, std::ios::out | std::ios::trunc);
    if (!out.is_open() || !out.good()) return false;
    out << n << "\n";
    if (!out.good()) return false;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            const int idx = i * n + j;
            out << counts[idx];
            if (j + 1 < n) out << ',';
        }
        out << "\n";
        if (!out.good()) return false;
    }
    out.flush();
    if (!out.good()) return false;
    out.close();
    if (out.fail()) return false;

    std::ifstream in(path);
    if (!in.is_open() || !in.good()) return false;
    int n_check = -1;
    in >> n_check;
    if (!in.good()) return false;
    return n_check == n;
}

bool CellStats::loadCountsCSV(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open() || !in.good()) return false;

    int n_file = 0;
    in >> n_file;
    if (!in.good() || n_file <= 0) return false;
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<int> tmp;
    tmp.resize(n_file * n_file, 0);

    for (int i = 0; i < n_file; ++i) {
        for (int j = 0; j < n_file; ++j) {
            int val = 0;
            if (!(in >> val)) return false;
            tmp[i * n_file + j] = val;
            if (in.peek() == ',') in.get();
        }
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (!in.good() && !in.eof()) return false;
    in.close();
    if (in.fail()) return false;

    n = n_file;
    counts.swap(tmp);
    return true;
}

bool CellStats::saveSummaryTXT(const std::string& path) const {
    std::ofstream out(path, std::ios::out | std::ios::trunc);
    if (!out.is_open() || !out.good()) return false;
    out.setf(std::ios::fixed);
    out.precision(6);
    out << n << " " << meanMultiplicity() << " " << medianMultiplicity() << "\n";
    if (!out.good()) return false;
    out.flush();
    if (!out.good()) return false;
    out.close();
    if (out.fail()) return false;

    std::ifstream in(path);
    if (!in.is_open() || !in.good()) return false;
    int n_check = 0;
    double a = 0.0, b = 0.0;
    in >> n_check >> a >> b;
    if (!in.good()) return false;
    return n_check == n;
}

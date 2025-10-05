//Was compiled with CLion
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <limits>
#include "Cell.h"
#include "RandomCells.h"
#include "Stats.h"

using namespace std;

static bool readConfig(const string& path, int& N, int& M, int& K) {
    ifstream in(path);
    if (!in.is_open() || !in.good()) return false;
    if (!(in >> N >> M >> K)) return false;
    in.close();
    if (in.fail()) return false;
    return (N > 0 && M >= 0 && K >= 0);
}

int main() {
    try {
        int N = 0, M = 0, K = 0;
        if (!readConfig("config.txt", N, M, K)) {
            cout << "Error: failed to read config.txt (expected: N M K with N>0, M>=0, K>=0)\n";
            return 1;
        }

        try {
            RandomCells bad(0);
            cout << "Error: RandomCells(0) did not throw invalid_argument\n";
            return 1;
        } catch (const invalid_argument&) {}

        RandomCells rng(N);
        CellStats stats(N);

        try {
            stats.sample(rng, M);
        } catch (const exception& e) {
            cout << string("Error: sampling failed: ") + e.what() + "\n";
            return 1;
        }

        if (!stats.saveCountsCSV("counts.csv")) {
            cout << "Error: failed to write counts.csv\n";
            return 1;
        }
        if (!stats.saveSummaryTXT("summary.txt")) {
            cout << "Error: failed to write summary.txt\n";
            return 1;
        }

        CellStats stats2(1);
        if (!stats2.loadCountsCSV("counts.csv")) {
            cout << "Error: failed to read back counts.csv\n";
            return 1;
        }

        {
            ofstream out("chi2.txt", ios::out | ios::trunc);
            if (!out.is_open() || !out.good()) { cout << "Error: cannot open chi2.txt for writing\n"; return 1; }
            out.close();
            if (out.fail()) { cout << "Error: cannot close chi2.txt after truncate\n"; return 1; }
        }

        {
            vector<int> counts(static_cast<size_t>(N) * static_cast<size_t>(N), 0);
            for (int i = 0; i < K; ++i) {
                Cell c = rng();
                if (c.x < 0 || c.y < 0 || c.x >= N || c.y >= N) { cout << "Error: out-of-range cell from RNG\n"; return 1; }
                int idx = c.x * N + c.y;
                ++counts[static_cast<size_t>(idx)];
            }
            int mn = counts[0], mx = counts[0];
            for (int i = 1; i < static_cast<int>(counts.size()); ++i) {
                if (counts[i] < mn) mn = counts[i];
                if (counts[i] > mx) mx = counts[i];
            }
            const double cells = static_cast<double>(N) * static_cast<double>(N);
            const double expected = static_cast<double>(K) / cells;
            double chi2 = 0.0;
            for (int v : counts) {
                const double diff = static_cast<double>(v) - expected;
                chi2 += diff * diff / expected;
            }
            ofstream out("chi2.txt", ios::out | ios::app);
            if (!out.is_open() || !out.good()) { cout << "Error: cannot open chi2.txt for append\n"; return 1; }
            out << fixed << setprecision(6) << mn << " " << mx << " " << chi2 << "\n";
            if (!out.good()) { cout << "Error: failed to write to chi2.txt\n"; return 1; }
            out.flush();
            if (!out.good()) { cout << "Error: flush failed for chi2.txt\n"; return 1; }
            out.close();
            if (out.fail()) { cout << "Error: cannot close chi2.txt after append\n"; return 1; }

            ifstream in("chi2.txt");
            if (!in.is_open() || !in.good()) { cout << "Error: cannot open chi2.txt for reading\n"; return 1; }
            string last, line;
            while (getline(in, line)) last = line;
            in.close();
            if (in.fail()) { cout << "Error: cannot close chi2.txt after reading\n"; return 1; }
            if (last.empty()) { cout << "Error: chi2.txt is empty after write\n"; return 1; }
        }

        {
            ofstream out("mean_median.txt", ios::out | ios::trunc);
            if (!out.is_open() || !out.good()) { cout << "Error: cannot open mean_median.txt for writing\n"; return 1; }
            out << fixed << setprecision(6)
                << stats.meanMultiplicity() << " " << stats.medianMultiplicity() << "\n";
            if (!out.good()) { cout << "Error: failed to write to mean_median.txt\n"; return 1; }
            out.flush();
            if (!out.good()) { cout << "Error: flush failed for mean_median.txt\n"; return 1; }
            out.close();
            if (out.fail()) { cout << "Error: cannot close mean_median.txt\n"; return 1; }

            ifstream in("mean_median.txt");
            if (!in.is_open() || !in.good()) { cout << "Error: cannot open mean_median.txt for reading\n"; return 1; }
            double a = 0.0, b = 0.0;
            if (!(in >> a >> b)) { cout << "Error: failed to parse mean_median.txt\n"; return 1; }
            in.close();
            if (in.fail()) { cout << "Error: cannot close mean_median.txt after reading\n"; return 1; }
        }

        return 0;
    } catch (const exception& e) {
        cout << string("Unhandled exception: ") + e.what() + "\n";
        return 1;
    } catch (...) {
        cout << "Unhandled unknown error\n";
        return 1;
    }
}

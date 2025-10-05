#include <iostream>
#include <iomanip>
#include <fstream>
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
        if (!readConfig("config.txt", N, M, K)) { cout << "CFG_ERR\n"; return 1; }

        try {
            RandomCell bad(0);
            (void)bad;
            cout << "INVARG_ERR\n";
        } catch (const invalid_argument&) {
            cout << "INVARG_OK\n";
        }

        RandomCell rng(N);
        CellStats stats(N);
        stats.sample(rng, M);

        if (!stats.saveCountsCSV("counts.csv")) { cout << "SAVE_COUNTS_ERR\n"; return 2; }
        if (!stats.saveSummaryTXT("summary.txt")) { cout << "SAVE_SUMMARY_ERR\n"; return 3; }

        CellStats stats2(1);
        if (!stats2.loadCountsCSV("counts.csv")) { cout << "LOAD_COUNTS_ERR\n"; return 4; }

        {
            ofstream out("chi2.txt", ios::out | ios::trunc);
            if (!out.is_open() || !out.good()) { cout << "OPEN_CHI2_ERR\n"; return 5; }
            out.close();
            if (out.fail()) { cout << "CLOSE_CHI2_ERR\n"; return 6; }
        }

        {
            vector<int> counts(static_cast<size_t>(N) * static_cast<size_t>(N), 0);
            for (int i = 0; i < K; ++i) {
                Cell c = rng();
                if (c.x < 0 || c.y < 0 || c.x >= N || c.y >= N) { cout << "RANGE_ERR\n"; return 7; }
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
            if (!out.is_open() || !out.good()) { cout << "OPEN_CHI2_ERR\n"; return 8; }
            out << fixed << setprecision(6) << mn << " " << mx << " " << chi2 << "\n";
            if (!out.good()) { cout << "WRITE_CHI2_ERR\n"; return 9; }
            out.flush();
            if (!out.good()) { cout << "FLUSH_CHI2_ERR\n"; return 10; }
            out.close();
            if (out.fail()) { cout << "CLOSE_CHI2_ERR\n"; return 11; }

            ifstream in("chi2.txt");
            if (!in.is_open() || !in.good()) { cout << "READ_CHI2_ERR\n"; return 12; }
            string last, line;
            while (getline(in, line)) last = line;
            in.close();
            if (in.fail()) { cout << "READ_CHI2_CLOSE_ERR\n"; return 13; }
            if (last.empty()) { cout << "READ_CHI2_EMPTY\n"; return 14; }
        }

        {
            ofstream out("mean_median.txt", ios::out | ios::trunc);
            if (!out.is_open() || !out.good()) { cout << "OPEN_MM_ERR\n"; return 15; }
            out << fixed << setprecision(6)
                << stats.meanMultiplicity() << " " << stats.medianMultiplicity() << "\n";
            if (!out.good()) { cout << "WRITE_MM_ERR\n"; return 16; }
            out.flush();
            if (!out.good()) { cout << "FLUSH_MM_ERR\n"; return 17; }
            out.close();
            if (out.fail()) { cout << "CLOSE_MM_ERR\n"; return 18; }

            ifstream in("mean_median.txt");
            if (!in.is_open() || !in.good()) { cout << "READ_MM_ERR\n"; return 19; }
            double a = 0.0, b = 0.0;
            if (!(in >> a >> b)) { cout << "PARSE_MM_ERR\n"; return 20; }
            in.close();
            if (in.fail()) { cout << "READ_MM_CLOSE_ERR\n"; return 21; }
        }

        cout << "OK\n";
        return 0;
    } catch (...) {
        cout << "FATAL\n";
        return 99;
    }
}

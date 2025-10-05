#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <numeric>
#include <stdexcept>

#include "Cell.h"
#include "RandomCells.h"
#include "Stats.h"

using namespace std;

static void printRule(char ch = '-', int len = 72) {
    for (int i = 0; i < len; ++i) cout << ch;
    cout << '\n';
}

static void ptitle(const string& t) {
    printRule('=');
    cout << t << '\n';
    printRule('=');
}

static void demo_basic(int N, int M) {
    RandomCell rng(N);
    CellStats stats(N);
    stats.sample(rng, M);
    cout << fixed << setprecision(6)
         << stats.meanMultiplicity() << " "
         << stats.medianMultiplicity() << "\n";
    printRule();
}

static void test_randomness_operator(int N, int K) {
    RandomCell rng(N);
    vector<int> counts(static_cast<size_t>(N) * static_cast<size_t>(N), 0);
    for (int i = 0; i < K; ++i) {
        Cell c = rng();
        if (c.x < 0 || c.y < 0 || c.x >= N || c.y >= N) {
            throw runtime_error("out of range");
        }
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
    cout << mn << " " << mx << " " << fixed << setprecision(3) << chi2 << "\n";
    printRule();
}

static void experiment_vs_M(int N, const vector<int>& Ms) {
    for (int M : Ms) {
        RandomCell rng(N);
        CellStats stats(N);
        stats.sample(rng, M);
        const double mean = stats.meanMultiplicity();
        const double median = stats.medianMultiplicity();
        const double theor  = static_cast<double>(M) / (N * 1.0 * N);
        cout << M << " "
             << fixed << setprecision(6) << mean << " "
             << fixed << setprecision(6) << theor << " "
             << fixed << setprecision(6) << median << "\n";
    }
    printRule();
}

static void experiment_vs_N(const vector<int>& Ns, int M) {
    for (int N : Ns) {
        RandomCell rng(N);
        CellStats stats(N);
        stats.sample(rng, M);
        const double mean  = stats.meanMultiplicity();
        const double theor = static_cast<double>(M) / (N * 1.0 * N);
        const double med   = stats.medianMultiplicity();
        cout << N << " " << (N * N) << " "
             << fixed << setprecision(6) << mean << " "
             << fixed << setprecision(6) << theor << " "
             << fixed << setprecision(6) << med << "\n";
    }
    printRule();
}

int main() {
    try {
        try {
            RandomCell bad(0);
            (void)bad;
        } catch (const invalid_argument& e) {
            cout << "0\n";
        }
        try {
            demo_basic(10, 10000);
        } catch (const exception& e) {
            cout << "1\n";
        }
        try {
            test_randomness_operator(10, 50000);
        } catch (const exception& e) {
            cout << "2\n";
        }
        try {
            vector<int> Ms = {1000, 5000, 10000, 20000, 50000};
            experiment_vs_M(10, Ms);
        } catch (const exception& e) {
            cout << "3\n";
        }
        try {
            vector<int> Ns = {5, 10, 15, 20};
            experiment_vs_N(Ns, 20000);
        } catch (const exception& e) {
            cout << "4\n";
        }
        return 0;
    } catch (const exception& e) {
        cout << "5\n";
        return 1;
    } catch (...) {
        cout << "6\n";
        return 2;
    }
}

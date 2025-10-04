#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "Cell.h"

void writePointsToFile(const std::string& filename, const std::vector<Cell>& points);

void writeStatsToFile(const std::string& filename, int N, std::uint64_t M, double average, double median);

void readInput(const std::string& filename, int& N, std::uint64_t& M);

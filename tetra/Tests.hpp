#pragma once
#include "TetraPacker.hpp"

int testShouldBe50PercentageOfPrismaCube(int N);
std::array<int, 3> testPointShouldNeverBeAcceptedByTwoOrZeroDomains(int N);
std::array<int, 9> testRandomizedEdgeAcceptance(const int N);
std::array<int, 28> testFullyRandomizedCubeAcceptance(const int N);
std::array<int, 65> testAcceptanceInMiniGrid(const int N);

bool testRandomizedTetrashouldAcceptPointInTheMiddle();
std::array<bool, 8> testRandomizedTetraShouldAcceptOnlyFirstCorner();
std::array<bool, 18> testRandomizedTetraShouldAcceptOnlyFirst6Edges();
bool testTooSmallOcta();
bool testBigEnoughOcta();

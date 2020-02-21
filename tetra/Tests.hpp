#pragma once
#include "TetraPacker.hpp"

int testShouldBe50PercentageOfPrismaCube(int N);
std::array<int, 3> testPointShouldNeverBeAcceptedByTwoOrZeroDomains(int N);

bool testRandomizedTetrashouldAcceptPointInTheMiddle();
std::array<bool, 8> testRandomizedTetraShouldAcceptOnlyFirstCorner();
std::array<bool, 18> testRandomizedTetraShouldAcceptOnlyFirst6Edges();

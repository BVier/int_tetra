#include "Tests.hpp"

int main()
{
	static int N = 1'000'000;
	int n = testShouldBe50PercentageOfPrismaCube(N);
	printf("Should be 50.0 \n");
	printf("%5.2lf%%\n", n * 100.0 / N);

	std::array<int, 3> distAcc = testPointShouldNeverBeAcceptedByTwoOrZeroDomains(N);
	printf("Accepted by no domain: %i \n", distAcc[0]);
	printf("Accepted by one domain: %i \n", distAcc[1]);
	printf("Accepted by both domains: %i \n", distAcc[2]);

	bool pointInTheMiddle = testRandomizedTetrashouldAcceptPointInTheMiddle();
	printf("\nAccepted Point (0.5,0.5,0.5): %d \n", pointInTheMiddle);

	std::array<bool, 8> acceptedCorners = testRandomizedTetraShouldAcceptOnlyFirstCorner();
	printf("\nAccepted corner 0: %d \n", acceptedCorners[0]);
	for (int i = 1; i < 8; i++) {
		printf("Accepted corner %i: %d \n", i, acceptedCorners[i]);
	}

}
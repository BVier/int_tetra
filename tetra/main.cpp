#include "Tests.hpp"

int main()
{
	bool shouldBeTooSmall = testTooSmallOcta();
	printf("Should be 0: %i \n", shouldBeTooSmall);
	bool shouldBeBigEnough = testBigEnoughOcta();
	printf("Should be 1: %i \n\n", shouldBeBigEnough);

	static int N = 100'000;
	int n = testShouldBe50PercentageOfPrismaCube(N);
	printf("Should be 50.0 \n");
	printf("%5.2lf%%\n", n * 100.0 / N);

	printf("\nTest multiple accept of one side \n");
	std::array<int, 3> distAcc = testPointShouldNeverBeAcceptedByTwoOrZeroDomains(N);
	printf("Accepted by no domain: %i \n", distAcc[0]);
	printf("Accepted by one domain: %i \n", distAcc[1]);
	printf("Accepted by both domains: %i \n", distAcc[2]);

	printf("\nTest multiple accept of one vertexPoint \n");
	std::array<int, 9> acceptedBy = testRandomizedEdgeAcceptance(N);
	for (int i = 0; i < 9; i++) {
		printf("Accepted by %i domains: %i \n", i, acceptedBy[i]);
	}
	printf("\nTest multiple accept of one random Domain \n");
	std::array<int, 28> acceptedBy2 = testFullyRandomizedCubeAcceptance(N);
	for (int i = 0; i < 8; i++) {
		printf("Accepted by %i domains: %i \n", i, acceptedBy2[i]);
	}
	printf("\nTest multiple accept of mini Grid Cluster \n");
	std::array<int, 65> acceptedBy3 = testAcceptanceInMiniGrid(N);
	for (int i = 0; i < 8; i++) {
		printf("Accepted by %i domains: %i \n", i, acceptedBy3[i]);
	}

	bool pointInTheMiddle = testRandomizedTetrashouldAcceptPointInTheMiddle();
	printf("\nAccepted Point (0.5,0.5,0.5): %d \n", pointInTheMiddle);

	std::array<bool, 8> acceptedCorners = testRandomizedTetraShouldAcceptOnlyFirstCorner();
	printf("\nAccepted corner 0: %d \n", acceptedCorners[0]);
	for (int i = 1; i < 8; i++) {
		printf("Accepted corner %i: %d \n", i, acceptedCorners[i]);
	}

}
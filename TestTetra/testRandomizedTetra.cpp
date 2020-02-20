#include "pch.h" 
#include <stdlib.h>

double randomFloat(int start, double range) {
    return double(start) + double(rand()) / double((RAND_MAX)) * range;
}

Vec3d* generateRandomCorners() 
{
    static Vec3d edges[8];
    edges[0] = Vec3d{ randomFloat(0, 0.5), randomFloat(0, 0.5), randomFloat(0, 0.5) };
    edges[1] = Vec3d{ randomFloat(1, 0.5), randomFloat(0, 0.5), randomFloat(0, 0.5) };
    edges[2] = Vec3d{ randomFloat(0, 0.5), randomFloat(1, 0.5), randomFloat(0, 0.5) };
    edges[3] = Vec3d{ randomFloat(1, 0.5), randomFloat(1, 0.5), randomFloat(0, 0.5) };
    edges[4] = Vec3d{ randomFloat(0, 0.5), randomFloat(0, 0.5), randomFloat(1, 0.5) };
    edges[5] = Vec3d{ randomFloat(1, 0.5), randomFloat(0, 0.5), randomFloat(1, 0.5) };
    edges[6] = Vec3d{ randomFloat(0, 0.5), randomFloat(1, 0.5), randomFloat(1, 0.5) };
    edges[7] = Vec3d{ randomFloat(1, 0.5), randomFloat(1, 0.5), randomFloat(1, 0.5) };
    return edges;
}

Vec3d midOfEdgeBetween(Vec3d a, Vec3d b) {
    return add(a, divide(subtract(b, a), Vec3d{ 2, 2, 2 }));
}
TEST(testRandomizedTetra, shouldAcceptPointInTheMiddle) {
    Vec3d* corners = generateRandomCorners();
    Octagon domain = Octagon(corners);
    Vec3d test = Vec3d{ 0.5, 0.5, 0.5 };
    EXPECT_TRUE(domain.contains(test));
}

TEST(testRandomizedTetra, shouldAcceptAllCorners) {
    Vec3d* corners = generateRandomCorners();
    Octagon domain = Octagon(corners);
    for (int i = 0; i < 8; i++) {
        EXPECT_TRUE(domain.contains(corners[i]));
    }
}

TEST(testRandomizedTetra, shouldAcceptAllEdges) {
    Vec3d* corners = generateRandomCorners();
    Octagon domain = Octagon(corners);

    for (int i = 0; i < 8; i++) {
        EXPECT_TRUE(dot(Vec3d{ 1, 1, 1 }, corners[i]) >= 0);
        EXPECT_TRUE(dot(Vec3d{ 1, 1, 1 }, corners[i]) <= 4.5);
    }

    Vec3d shouldContain [18];

    Vec3d start = corners[0];
    Vec3d end = corners[7];
    Vec3d before = corners[5];

    int cOrder[6] = { 1,3,2,6,4,5 };
    for (int i = 1; i < 7; i++) {
        shouldContain[i - 1] = midOfEdgeBetween(start, corners[i]);
        shouldContain[5 + i] = midOfEdgeBetween(end, corners[i]);
        Vec3d vertical = corners[cOrder[i - 1]];
        shouldContain[11 + i] = midOfEdgeBetween(before, vertical);
        before = vertical;
    }
    for (int i = 0; i < 18; i++) {
        bool contains = domain.contains(shouldContain[i]);
        EXPECT_TRUE(contains);
    }
}
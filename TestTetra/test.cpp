#include "pch.h"
Vec3d* generateCubicDomain()
{
    static Vec3d edges[8];
    edges[0] = Vec3d{ 0.0, 0.0, 0.0 };
    edges[1] = Vec3d{ 1.0, 0.0, 0.0 };
    edges[2] = Vec3d{ 0.0, 1.0, 0.0 };
    edges[3] = Vec3d{ 1.0, 1.0, 0.0 };
    edges[4] = Vec3d{ 0.0, 0.0, 1.0 };
    edges[5] = Vec3d{ 1.0, 0.0, 1.0 };
    edges[6] = Vec3d{ 0.0, 1.0, 1.0 };
    edges[7] = Vec3d{ 1.0, 1.0, 1.0 };
    return edges;
}

TEST(TestDefaultTetraPacker, shouldAcceptPointInTheMiddle) {

    Octagon domain = Octagon(generateCubicDomain());
    Vec3d test = {0.5, 0.5, 0.5};
    EXPECT_TRUE(domain.contains(test));
}

TEST(TestDefaultTetraPacker, shouldOnlyAcceptFirstEdge) {
    Vec3d* corners = generateCubicDomain();
    Octagon domain = Octagon(corners);
    EXPECT_TRUE(domain.contains(corners[0]));
    for (int i = 1; i < 8; i++) {
        EXPECT_TRUE(domain.contains(corners[i]));
    }
}
#include "Tests.hpp"
#include<array>

double randomFloat(int start, double range) {
    return double(start) + double(rand()) / double((RAND_MAX)) * range;
}

std::array<Vec3dF, 8> generateRandomCorners()
{
    std::array<Vec3dF, 8> corners{ {
            Vec3dF{ randomFloat(0, 0.5), randomFloat(0, 0.5), randomFloat(0, 0.5) },
            Vec3dF{ randomFloat(1, 0.5), randomFloat(0, 0.5), randomFloat(0, 0.5) },
            Vec3dF{ randomFloat(0, 0.5), randomFloat(1, 0.5), randomFloat(0, 0.5) },
            Vec3dF{ randomFloat(1, 0.5), randomFloat(1, 0.5), randomFloat(0, 0.5) },
            Vec3dF{ randomFloat(0, 0.5), randomFloat(0, 0.5), randomFloat(1, 0.5) },
            Vec3dF{ randomFloat(1, 0.5), randomFloat(0, 0.5), randomFloat(1, 0.5) },
            Vec3dF{ randomFloat(0, 0.5), randomFloat(1, 0.5), randomFloat(1, 0.5) },
            Vec3dF{ randomFloat(1, 0.5), randomFloat(1, 0.5), randomFloat(1, 0.5) },
    } };
    return corners;
}

Vec3dF midOfEdgeBetween(Vec3dF a, Vec3dF b) {
    Vec3dF edgePoint;
    for (int i = 0; i < 3; i++) {
        edgePoint[i] = a[i] + ((b[i] - a[i]) / 2);
    }
    return edgePoint;
}


bool testRandomizedTetrashouldAcceptPointInTheMiddle() {
    std::array<Vec3dF,8> corners = generateRandomCorners();
    Octagon domain = Octagon(corners);
    Vec3dF test = { 0.5, 0.5, 0.5 };
    return domain.contains(test);
}

std::array<bool, 8> testRandomizedTetraShouldAcceptOnlyFirstCorner() {
    std::array<Vec3dF, 8> corners = generateRandomCorners();
    Octagon domain = Octagon(corners);
    std::array<bool, 8> acceptedCorners = {};
    for (int i = 0; i < 8; i++) {
        acceptedCorners[i] = domain.contains(corners[i]);
    }
    return acceptedCorners;
}

std::array<bool, 18> testRandomizedTetraShouldAcceptOnlyFirst6Edges() {
    std::array<Vec3dF, 8> corners = generateRandomCorners();
    Octagon domain = Octagon(corners);

    Vec3dF edges[18];

    Vec3dF start = corners[0];
    Vec3dF end = corners[7];
    Vec3dF before = corners[5];

    int cOrder[6] = { 1,3,2,6,4,5 };
    for (int i = 1; i < 7; i++) {
        edges[i - 1] = midOfEdgeBetween(start, corners[i]);
        edges[5 + i] = midOfEdgeBetween(end, corners[i]);
        Vec3dF vertical = corners[cOrder[i - 1]];
        edges[11 + i] = midOfEdgeBetween(before, vertical);
        before = vertical;
    }

    std::array<bool,18> containsEdge = {};
    for (int i = 0; i < 18; i++) {
        containsEdge[i] = domain.contains(edges[i]);
    }
    return containsEdge;
}
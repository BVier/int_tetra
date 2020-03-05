#include "Tests.hpp"

double randomFloat(int start, double range) {
    return double(start) + double(rand()) / double((RAND_MAX)) * range;
}

std::array<tetra::Vec3d, 8> generateRandomCorners()
{
    std::array<tetra::Vec3d, 8> corners{ {
            tetra::Vec3d{ randomFloat(0, 0.5), randomFloat(0, 0.5), randomFloat(0, 0.5) },
            tetra::Vec3d{ randomFloat(1, 0.5), randomFloat(0, 0.5), randomFloat(0, 0.5) },
            tetra::Vec3d{ randomFloat(0, 0.5), randomFloat(1, 0.5), randomFloat(0, 0.5) },
            tetra::Vec3d{ randomFloat(1, 0.5), randomFloat(1, 0.5), randomFloat(0, 0.5) },
            tetra::Vec3d{ randomFloat(0, 0.5), randomFloat(0, 0.5), randomFloat(1, 0.5) },
            tetra::Vec3d{ randomFloat(1, 0.5), randomFloat(0, 0.5), randomFloat(1, 0.5) },
            tetra::Vec3d{ randomFloat(0, 0.5), randomFloat(1, 0.5), randomFloat(1, 0.5) },
            tetra::Vec3d{ randomFloat(1, 0.5), randomFloat(1, 0.5), randomFloat(1, 0.5) },
    } };
    return corners;
}

tetra::Vec3d midOfEdgeBetween(tetra::Vec3d a, tetra::Vec3d b) {
    tetra::Vec3d edgePoint;
    for (int i = 0; i < 3; i++) {
        edgePoint[i] = a[i] + ((b[i] - a[i]) / 2);
    }
    return edgePoint;
}


bool testRandomizedTetrashouldAcceptPointInTheMiddle() {
    std::array<tetra::Vec3d,8> corners = generateRandomCorners();
    auto domain = tetra::Octagon(corners);
    tetra::Vec3d test = { 0.5, 0.5, 0.5 };
    return domain.contains(test);
}

std::array<bool, 8> testRandomizedTetraShouldAcceptOnlyFirstCorner() {
    std::array<tetra::Vec3d, 8> corners = generateRandomCorners();
    auto domain = tetra::Octagon(corners);
    std::array<bool, 8> acceptedCorners = {};
    for (int i = 0; i < 8; i++) {
        acceptedCorners[i] = domain.contains(corners[i]);
    }
    return acceptedCorners;
}

std::array<bool, 18> testRandomizedTetraShouldAcceptOnlyFirst6Edges() {
    std::array<tetra::Vec3d, 8> corners = generateRandomCorners();
    auto domain = tetra::Octagon(corners);

    tetra::Vec3d edges[18];

    tetra::Vec3d start = corners[0];
    tetra::Vec3d end = corners[7];
    tetra::Vec3d before = corners[5];

    int cOrder[6] = { 1,3,2,6,4,5 };
    for (int i = 1; i < 7; i++) {
        edges[i - 1] = midOfEdgeBetween(start, corners[i]);
        edges[5 + i] = midOfEdgeBetween(end, corners[i]);
        tetra::Vec3d vertical = corners[cOrder[i - 1]];
        edges[11 + i] = midOfEdgeBetween(before, vertical);
        before = vertical;
    }

    std::array<bool,18> containsEdge = {};
    for (int i = 0; i < 18; i++) {
        containsEdge[i] = domain.contains(edges[i]);
    }
    return containsEdge;
}

bool testTooSmallOcta() {
    tetra::Octagon o = tetra::Octagon(generateRandomCorners());
    return o.isValid();
}

bool testBigEnoughOcta() {
    std::array<tetra::Vec3d, 8> corners{ {
            { 0, 0, 0 },
            { 5, 0, 0 },
            { 0, 5, 0 },
            { 5, 5, 0 },
            { 0, 0, 5 },
            { 5, 0, 5 },
            { 0, 5, 5 },
            { 5, 5, 5 },
    } };
    auto o = tetra::Octagon(corners);
    return o.isValid();
}
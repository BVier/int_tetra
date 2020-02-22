// See LICENSE for license details.

#include <cstdio>
#include <chrono>
#include <random>
#include "Tests.hpp"

struct Randgen
{
    Randgen()
        : mt(std::chrono::high_resolution_clock::now()
                 .time_since_epoch()
                 .count()),
          d(0, 1) {}
    //Randgen(Randintgen &&other)
    //    : mt(std::move(other.mt)), d(std::move(other.d)) {}
    double operator()() { return d(mt); }

  private:
    std::mt19937 mt;
    std::uniform_real_distribution<> d;
};


int ninside(std::array<tetra::Vec3d, 8> cs, int N)
{
    auto o = tetra::Octagon{ cs };
    auto rnd = Randgen{};
    int n = 0;

    for (int i = 0; i < N; ++i) {
        tetra::Vec3d p = tetra::Vec3d{ rnd(), rnd(), rnd() };
        if (o.contains(p)) {
            n++;
        }
    }
    return n;
}

std::array<int,3> ninsideTwoDomains(std::array<tetra::Vec3d, 8> corner1, std::array<tetra::Vec3d, 8> corner2, int N)
{
    auto o1 = tetra::Octagon(corner1);
    auto o2 = tetra::Octagon(corner2);
    auto rnd = Randgen{};
    
    std::array<int, 3> counter{ {0,0,0} };

    for (int i = 0; i < N; ++i) {
        tetra::Vec3d p = tetra::Vec3d{ rnd(), rnd(), rnd() };
        bool c1 = o1.contains(p);
        bool c2 = o2.contains(p);
        counter[c1 + c2]++;
    }
    return counter;
}

int testShouldBe50PercentageOfPrismaCube(int N) {
    std::array<tetra::Vec3d, 8> cs = { tetra::Vec3d{0, .5,  0},
                                      tetra::Vec3d{0,  1, .5},
                                      tetra::Vec3d{0,  0, .5},
                                      tetra::Vec3d{0, .5,  1},
                                      tetra::Vec3d{1, .5,  0},
                                      tetra::Vec3d{1,  1, .5},
                                      tetra::Vec3d{1,  0, .5},
                                      tetra::Vec3d{1, .5,  1} };

    return ninside(cs, N);
}

std::array<int, 3> testPointShouldNeverBeAcceptedByTwoOrZeroDomains(int N) {
    auto rnd = Randgen{};
    auto p1 = tetra::Vec3d{ rnd(), 0, 0 };
    auto p2 = tetra::Vec3d{ rnd(), 1, 0 };
    auto p3 = tetra::Vec3d{ rnd(), 0, 1 };
    auto p4 = tetra::Vec3d{ rnd(), 1, 1 };
    std::array<tetra::Vec3d, 8> corner1 = {
        tetra::Vec3d{0, 0, 0},
        p1,
        tetra::Vec3d{0, 1, 0},
        p2,
        tetra::Vec3d{0, 0, 1},
        p3,
        tetra::Vec3d{0, 1, 1},
        p4,
    };
    std::array<tetra::Vec3d, 8> corner2 = {
        p1,
        tetra::Vec3d{1, 0, 0},
        p2,
        tetra::Vec3d{1, 1, 0},
        p3,
        tetra::Vec3d{1, 0, 1},
        p4,
        tetra::Vec3d{1, 1, 1},
    };

    return ninsideTwoDomains(corner1, corner2, N);
}
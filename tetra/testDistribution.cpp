// See LICENSE for license details.

#include <cstdio>
#include <chrono>
#include <random>
#include "TetraPacker.hpp"

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


int ninside(std::array<Vec3dF, 8> cs, int N)
{
    auto o = Octagon(cs);
    auto rnd = Randgen{};
    int n = 0;

    for (int i = 0; i < N; ++i) {
        Vec3dF p = Vec3dF{ rnd(), rnd(), rnd() };
        if (o.contains(p)) {
            n++;
        }
    }
    return n;
}

std::array<int,3> ninsideTwoDomains(std::array<Vec3dF, 8> corner1, std::array<Vec3dF, 8> corner2, int N)
{
    auto o1 = Octagon(corner1);
    auto o2 = Octagon(corner2);
    auto rnd = Randgen{};
    
    std::array<int, 3> counter{ {0,0,0} };

    for (int i = 0; i < N; ++i) {
        Vec3dF p = Vec3dF{ rnd(), rnd(), rnd() };
        bool c1 = o1.contains(p);
        bool c2 = o2.contains(p);
        counter[c1 + c2]++;
    }
    return counter;
}

int testShouldBe50PercentageOfPrismaCube(int N) {
    std::array<Vec3dF, 8> cs = { Vec3dF{0, .5,  0},
                                      Vec3dF{0,  1, .5},
                                      Vec3dF{0,  0, .5},
                                      Vec3dF{0, .5,  1},
                                      Vec3dF{1, .5,  0},
                                      Vec3dF{1,  1, .5},
                                      Vec3dF{1,  0, .5},
                                      Vec3dF{1, .5,  1} };

    return ninside(cs, N);
}

std::array<int,3> testPointShouldNeverBeAcceptedByTwoOrZeroDomains(int N) {
    std::array<Vec3dF, 8> corner1 = {
        Vec3dF{0, 0, 0},
        Vec3dF{0.5, 0, 0},
        Vec3dF{0, 1, 0},
        Vec3dF{0.5, 1, 0},
        Vec3dF{0, 0, 1},
        Vec3dF{0.5, 0, 1},
        Vec3dF{0, 1, 1},
        Vec3dF{0.5, 1, 1},
    };
    std::array<Vec3dF, 8> corner2 = {
        Vec3dF{0.5, 0, 0},
        Vec3dF{1, 0, 0},
        Vec3dF{0.5, 1, 0},
        Vec3dF{1, 1, 0},
        Vec3dF{0.5, 0, 1},
        Vec3dF{1, 0, 1},
        Vec3dF{0.5, 1, 1},
        Vec3dF{1, 1, 1},
    };

    return ninsideTwoDomains(corner1, corner2, N);
}
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


int ninside( Vec3d cs[8], int N)
{
    auto o = Octagon(cs);
    auto rnd = Randgen{};
    int n = 0;

    for (int i = 0; i < N; ++i) {
        Vec3d p = Vec3d{rnd(), rnd(), rnd()};
        if (o.contains(p))
            n++;
    }
    return n;
}

int main()
{
    // 50% of the volume of the unit cube
    Vec3d cs [8] = {Vec3d{0, .5,  0},
                                      Vec3d{0,  0, .5},
                                      Vec3d{0,  1, .5},
                                      Vec3d{0, .5,  1},
                                      Vec3d{1, .5,  0},
                                      Vec3d{1,  0, .5},
                                      Vec3d{1,  1, .5},
                                      Vec3d{1, .5,  1}};

    const int N = 1'000'000;

    int n = ninside(cs, N);

    printf("%5.2lf%%\n", n * 100.0 / N);
}


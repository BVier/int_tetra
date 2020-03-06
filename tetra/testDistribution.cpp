// See LICENSE for license details.

#include <cstdio>
#include <chrono>
#include <random>
#include <fstream>
#include <string>
#include "Tests.hpp"

using namespace tetra;
using std::array;

const int range = 10;

struct Randgen
{
    Randgen()
        : mt(std::chrono::high_resolution_clock::now()
                 .time_since_epoch()
                 .count()),
          d(0, range) {}
    //Randgen(Randintgen &&other)
    //    : mt(std::move(other.mt)), d(std::move(other.d)) {}
    double operator()() { return d(mt); }

  private:
    std::mt19937 mt;
    std::uniform_real_distribution<> d;
};

int ninside(array<Vec3d, 8> cs, int N)
{
    auto o = Octagon{ cs };
    auto rnd = Randgen{};
    int n = 0;

    for (int i = 0; i < N; ++i) {
        Vec3d p = Vec3d{ rnd(), rnd(), rnd() };
        if (o.contains(p)) {
            n++;
        }
    }
    return n;
}

int testShouldBe50PercentageOfPrismaCube(int N) {
    array<Vec3d, 8> cs = { Vec3d{0, .5 * range,  0},
                           Vec3d{0, range, .5 * range},
                           Vec3d{0,  0, .5 * range},
                           Vec3d{0, .5 * range,  range},
                           Vec3d{range, .5 * range,  0},
                           Vec3d{range,  range, .5 * range},
                           Vec3d{range,  0, .5 * range},
                           Vec3d{range, .5 * range,  range} };

    return ninside(cs, N);
}


template<int size>
struct PointArray
{
    array<array<array<Vec3d, size>, size>, size> point = {};
    PointArray();
    Randgen rnd = Randgen{};
    int domains = size - 1;
    int ddom = double(size - 1);
    double valueFor(int i)
    {
        if (i == 0 || i == domains) { return double(range) * double(i) / ddom; }
        return randomAround(i);
    }
    double randomAround(int i) { 
        return (((double(i)*ddom)-1)*range+2*rnd())/std::pow(ddom,2); 
    }
};

template<int size>
PointArray<size>::PointArray()
{
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            for (int z = 0; z < size; z++) {
                point[x][y][z] = Vec3d{ valueFor(x),valueFor(y),valueFor(z) };
            }
        }
    }
}
template <std::size_t size>
array<int, size + 1> acceptedByDomains(array<array<Vec3d,8>, size> vertices, int N)
{
    auto rnd = Randgen{};

    array<Octagon, size> octs = {};
    for (int i = 0; i < size; i++) { octs[i] = Octagon{ vertices[i] }; }
    array<int, size + 1> counter = {};
    
    std::ofstream csv("doublePoints" + std::to_string(size) + ".csv");
    for (int i = 0; i < N; ++i) {
        Vec3d p = Vec3d{ rnd(), rnd(), rnd() };
        int count = 0;
        for (int k = 0; k < size; ++k) {
            if (octs[k].contains(p)) { count++; }
        }
        counter[count]++;
        if (count != 1) {
            csv << std::to_string(p[0]) << ","
                << std::to_string(p[1]) << ","
                << std::to_string(p[2]) << ","
                << std::to_string(count) << "\n";
        }
    }
    csv.close();
    return counter;
}

std::array<int, 3> testPointShouldNeverBeAcceptedByTwoOrZeroDomains(int N) {
    auto rnd = Randgen{};
    auto p1 = Vec3d{ 0.25 + 0.5 * rnd(), 0, 0 };
    auto p2 = Vec3d{ 0.25 + 0.5 * rnd(), range, 0 };
    auto p3 = Vec3d{ 0.25 + 0.5 * rnd(), 0, range };
    auto p4 = Vec3d{ 0.25 + 0.5 * rnd(), range, range };

    array<array<Vec3d, 8>, 2> corners = {};
    corners[0] = {
        Vec3d{0, 0, 0},
        p1,
        Vec3d{0, range, 0},
        p2,
        Vec3d{0, 0, range},
        p3,
        Vec3d{0, range, range},
        p4,
    };
    corners[1] = {
        p1,
        Vec3d{range, 0, 0},
        p2,
        Vec3d{range, range, 0},
        p3,
        Vec3d{range, 0, range},
        p4,
        Vec3d{range, range, range},
    };
    return acceptedByDomains(corners, N);
}

template<int domain>
std::array<int, domain*domain*domain+1> testHowOftenAcceptDomains(int N)
{
    using namespace tetra;
    const int size = domain + 1;
    PointArray<size> p;

    const int numberOfDomains = domain * domain * domain; //int(std::pow(domain,3)) is not accepted
    array<array<Vec3d, 8>, numberOfDomains> vertices = {};
    int index = 0;

    for (int i = 0; i < domain; i++) {
        for (int k = 0; k < domain; k++) {
            for (int l = 0; l < domain; l++) {
                vertices[index] = {
                    p.point[0 + l][0 + k][0 + i],
                    p.point[1 + l][0 + k][0 + i],
                    p.point[0 + l][1 + k][0 + i],
                    p.point[1 + l][1 + k][0 + i],
                    p.point[0 + l][0 + k][1 + i],
                    p.point[1 + l][0 + k][1 + i],
                    p.point[0 + l][1 + k][1 + i],
                    p.point[1 + l][1 + k][1 + i],
                };
                index++;
            }
        }
    }

    std::ofstream verticeStream("corner" + std::to_string(numberOfDomains) + ".csv");
    for (auto vertex : vertices) {
        for (int i = 0; i < 8; i++) {
            verticeStream << vertex[i][0] << ",";
            verticeStream << vertex[i][1] << ",";
            verticeStream << vertex[i][2] << "\n";
        }
    }
    verticeStream.close();

    return acceptedByDomains(vertices, N);
}

std::array<int, 9> testRandomizedEdgeAcceptance(const int N) {
    return testHowOftenAcceptDomains<2>(N);
}

std::array<int, 28> testFullyRandomizedCubeAcceptance(const int N) {
    return testHowOftenAcceptDomains<3>(N);
}

std::array<int, 65> testAcceptanceInMiniGrid(const int N) {
    return testHowOftenAcceptDomains<4>(N);
}
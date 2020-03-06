// See LICENSE for license details.

#include <cstdio>
#include <chrono>
#include <random>
#include <fstream>
#include <string>
#include "Tests.hpp"

using std::array;
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

template<int size>
array<array<array<tetra::Vec3d, size>, size>, size> initPointArray() {
    array<array<array<tetra::Vec3d, size>, size>, size> pointArray{};
    for (int i = 0; i < size; i++) {
        for (int k = 0; k < size; k++) {
            for (int j = 0; j < size; j++) {
                pointArray[i][k][j] = tetra::Vec3d{
                    double(i) / (size - 1) ,
                    double(k) / (size - 1),
                    double(j) / (size - 1) };
            }
        }
    }
    return pointArray;
}

template <std::size_t size>
std::array<int, size + 1> acceptedByDomains(std::array<std::array<tetra::Vec3d, 8>, size> corners, int N)
{
    std::array<tetra::Octagon, size> octs = {};
    for (int i = 0; i < size; i++) {
        octs[i] = tetra::Octagon(corners[i]);
    }
    auto rnd = Randgen{};

    std::array<int, size + 1> counter = {};
    std::ofstream csv("doublePoints" + std::to_string(size) + ".csv");
    for (int i = 0; i < N; ++i) {
        tetra::Vec3d p = tetra::Vec3d{ rnd(), rnd(), rnd() };
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
    auto p1 = tetra::Vec3d{ rnd(), 0, 0 };
    auto p2 = tetra::Vec3d{ rnd(), 1, 0 };
    auto p3 = tetra::Vec3d{ rnd(), 0, 1 };
    auto p4 = tetra::Vec3d{ rnd(), 1, 1 };

    std::array<std::array<tetra::Vec3d, 8>,2> corners = {};
    corners[0] = {
        tetra::Vec3d{0, 0, 0},
        p1,
        tetra::Vec3d{0, 1, 0},
        p2,
        tetra::Vec3d{0, 0, 1},
        p3,
        tetra::Vec3d{0, 1, 1},
        p4,
    };
    corners[1] = {
        p1,
        tetra::Vec3d{1, 0, 0},
        p2,
        tetra::Vec3d{1, 1, 0},
        p3,
        tetra::Vec3d{1, 0, 1},
        p4,
        tetra::Vec3d{1, 1, 1},
    };
    return acceptedByDomains(corners, N);
}

double randomAround(int i, int domains)
{ 
    auto rnd = Randgen{}; 
    return (double(i) - 0.5 + rnd()) / domains; 
}

template<int domain>
std::array<int, domain*domain*domain+1> testHowOftenAcceptDomains(int N)
{
    using namespace tetra;
    const int size = domain + 1;
    array<array<array<Vec3d, size>, size>, size> point = initPointArray<size>();
    for (int i = 1; i < domain; i++) {
        for (int k = 1; k < domain; k++) {
            for (int l = 1; l < domain; l++) {
                point[i][k][l] = { {randomAround(i, domain), randomAround(k, domain), randomAround(l,domain)} };
            }
        }
    }
    const int numberOfDomains = domain * domain * domain;
    std::array<std::array<tetra::Vec3d, 8>, numberOfDomains> vertices = {};
    int index = 0;

    for (int i = 0; i < domain; i++) {
        for (int k = 0; k < domain; k++) {
            for (int l = 0; l < domain; l++) {
                vertices[index] = {
                    point[0 + l][0 + k][0 + i],
                    point[1 + l][0 + k][0 + i],
                    point[0 + l][1 + k][0 + i],
                    point[1 + l][1 + k][0 + i],
                    point[0 + l][0 + k][1 + i],
                    point[1 + l][0 + k][1 + i],
                    point[0 + l][1 + k][1 + i],
                    point[1 + l][1 + k][1 + i],
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
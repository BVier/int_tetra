#pragma once
#include <string>
#include <array>
#include "Point.hpp"

struct Norm{
	Vec3d normVector;
	int64_t heightOfPlane;
	bool isAbove(Vec3d v);
	bool isAbove(Vec3d v, bool equal);
};

struct Tetra{
	Norm norms[4] = {};
	bool valid = false;
	Tetra() {};
	Tetra(std::array<Vec3d, 4> corners);
	bool contains(Vec3d p);
};

class Octagon
{
	int cornerOrder[6] = { 1, 3, 2, 6, 4, 5 };
	Tetra tetras[6] = {};
	int16_t precision = 10;
	void initTetras(std::array<Vec3dF, 8> corners);
public:
	Octagon(std::array<Vec3dF, 8> corners);
	Octagon(std::array<Vec3dF, 8> corners, int16_t precision);
	bool contains(Vec3dF);
};


Norm computeNorm(Vec3d p1, Vec3d p2, Vec3d p3);
Vec3d castToInt(Vec3dF v, int16_t precision);

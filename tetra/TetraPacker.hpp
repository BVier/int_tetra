#pragma once
#include <string>
#include <array>
#include "Point.hpp"

struct Norm{
	Vec3d normVector;
	double heightOfPlane;
	bool isAbove(Vec3d);
};

struct Tetra{
	Norm norms[4] = {};
	bool valid = false;
	Tetra() {};
	Tetra(Vec3d corners[4]);
	bool contains(Vec3d p);
};

class Octagon
{
	int cornerOrder[6] = { 1, 3, 2, 6, 4, 5 };
	Tetra tetras[6] = {};
public:
	Octagon(Vec3d corners[8]);
	bool contains(Vec3d);
};


Norm computeNorm(Vec3d p1, Vec3d p2, Vec3d p3);
Norm computeNorm(Vec3d p1, Vec3d p2, Vec3d p3, bool normalized);


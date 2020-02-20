
#include "TetraPacker.hpp"

/*
	NORM
*/
bool Norm::isAbove(Vec3d point) {
	return dot(point, this->normVector) >= this->heightOfPlane;
}

/*
	TETRA
*/
Tetra::Tetra(Vec3d corners[4])
{
	this->norms[0] = computeNorm(corners[0], corners[1], corners[2]);
	this->norms[1] = computeNorm(corners[0], corners[2], corners[3]);
	this->norms[2] = computeNorm(corners[0], corners[3], corners[1]);
	this->norms[3] = computeNorm(corners[1], corners[3], corners[2]);
	
	this->valid = true;
}

bool Tetra::contains(Vec3d p) {
	if (!this->valid) {
		return false;
	}

	bool contains = true;
	for (int i = 0; i < 4 && contains; i++) {
		contains &= this->norms[i].isAbove(p);
	}
	return contains;
}



/*
	TETRAPACKER
*/

Octagon::Octagon(Vec3d corners[8]) 
{
	Vec3d start = corners[0];
	Vec3d end = corners[7];
	Vec3d last = corners[5];
	for (int i = 0; i < 6; i++) {
		Vec3d next = corners[this->cornerOrder[i]];
		Vec3d tetraCorners[4] = { start, end, last, next };
		this->tetras[i] = Tetra(tetraCorners);
		last = next;
	}
}

bool Octagon::contains(Vec3d p)
{
	for (int i = 0; i < 6; i++) {
		Tetra tetra = this->tetras[i];
		if (tetra.contains(p)) {
			return true;
		}
	}
	return false;
}

/*
	FUNCTIONS
*/

Norm computeNorm(Vec3d p1, Vec3d p2, Vec3d p3, bool normalized) {
	Vec3d v1 = subtract(p2, p1);
	Vec3d v2 = subtract(p1, p3);
	Vec3d c = cross(v2, v1);
	if (normalized) {
		double euclideanDist = euclid(c);
		c = divide(c, Vec3d({ euclideanDist, euclideanDist, euclideanDist }));
	}
	double reference = dot(c, p1);
	return { c, reference };
}

Norm computeNorm(Vec3d p1, Vec3d p2, Vec3d p3) {
	return computeNorm(p1, p2, p3, false);
}
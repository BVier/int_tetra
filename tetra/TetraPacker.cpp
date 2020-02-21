
#include "TetraPacker.hpp"

/*
	NORM
*/
bool Norm::isAbove(Vec3d point, bool equal) {
	if (equal)
		return dot(point, this->normVector) >= this->heightOfPlane;
	return dot(point, this->normVector) > this->heightOfPlane;
}

bool Norm::isAbove(Vec3d point) {
	return isAbove(point, true);
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
	for (int i = 0; i < 3 && contains; i++) {
		contains &= this->norms[i].isAbove(p);
	}
	contains &= this->norms[3].isAbove(p, false);
	return contains;
}



/*
	TETRAPACKER
*/

Octagon::Octagon(Vec3dF corners[8])
{
	initTetras(corners);
}

Octagon::Octagon(Vec3dF corners[8], int16_t precision)
{
	this->precision = precision;
	initTetras(corners);
}

void Octagon::initTetras(Vec3dF floatCorners[8]) 
{
	Vec3d corners[8];
	for (int i = 0; i < 8; i++) {
		corners[i] = castToInt(floatCorners[i], precision);
	}
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

bool Octagon::contains(Vec3dF point)
{
	Vec3d p = castToInt(point, this->precision);
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

Norm computeNorm(Vec3d p1, Vec3d p2, Vec3d p3) {
	Vec3d v1 = subtract(p2, p1);
	Vec3d v2 = subtract(p1, p3);
	Vec3d c = cross(v2, v1);
	int64_t reference = dot(c, p1);
	return { c, reference };
}

Vec3d castToInt(Vec3dF v, int16_t precision) {
	Vec3d new_v;
	new_v[0] = int(v[0] * precision);
	new_v[1] = int(v[1] * precision);
	new_v[2] = int(v[2] * precision);
	return new_v;
}
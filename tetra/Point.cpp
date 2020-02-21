#include "Point.hpp"
#include <math.h>


Vec3d add(Vec3d p1, Vec3d p2) {
	Vec3d res;
	res[0] = p1[0] + p2[0];
	res[1] = p1[1] + p2[1];
	res[2] = p1[2] + p2[2];
	return res;
}

Vec3d subtract(Vec3d p1, Vec3d p2) {
	Vec3d res;
	res[0] = p1[0] - p2[0];
	res[1] = p1[1] - p2[1];
	res[2] = p1[2] - p2[2];
	return res;
}

Vec3d multiply(Vec3d p1, Vec3d p2) {
	Vec3d res;
	res[0] = p1[0] * p2[0];
	res[1] = p1[1] * p2[1];
	res[2] = p1[2] * p2[2];
	return res;
}

Vec3d divide(Vec3d p1, Vec3d p2) {
	Vec3d res;
	res[0] = p1[0] / p2[0];
	res[1] = p1[1] / p2[1];
	res[2] = p1[2] / p2[2];
	return res;
}


Vec3d cross(Vec3d v1, Vec3d v2) {
	Vec3d result;
	result[0] = v1[1] * v2[2] - v1[2] * v2[1];
	result[1] = v1[2] * v2[0] - v1[0] * v2[2];
	result[2] = v1[0] * v2[1] - v1[1] * v2[0];
	return result;
}

int64_t dot(Vec3d p1, Vec3d p2)
{
	return p1[0] * p2[0] + p1[1] * p2[1] + p1[2] * p2[2];
}


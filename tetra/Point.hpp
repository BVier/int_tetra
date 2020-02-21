#pragma once
#include <string>
#include <array>

typedef std::array<int64_t, 3> Vec3d;
typedef std::array<double, 3> Vec3dF;

Vec3d subtract(Vec3d p1, Vec3d p2);
Vec3d add(Vec3d p1, Vec3d p2);
Vec3d multiply(Vec3d p1, Vec3d p2);
Vec3d divide(Vec3d p1, Vec3d p2);

Vec3d cross(Vec3d p1, Vec3d p2);
int64_t dot(Vec3d p1, Vec3d p2);

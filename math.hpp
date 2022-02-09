#ifndef MATH_HPP
#define MATH_HPP

#include "structs.h"

int det2(int m[2][2]);

Vec3f cross(const Vec3f &v1, const Vec3f &v2);

Vec3f baricenter(Vec2i A, Vec2i B, Vec2i C, Vec2i P);

#endif // MATH_HPP
#include "math.hpp"
#include "structs.h"
#include <stdlib.h>

int det2(int m[2][2])
{
    return m[0][0]*m[1][1] - m[0][1]*m[0][1];
}

Vec3f cross(const Vec3f &v1, const Vec3f &v2) {
    return Vec3f(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

Vec3f baricenter(Vec2i A, Vec2i B, Vec2i C, Vec2i P)
{
    // det of vec(AC, AB, PA)
    Vec3f b = cross(Vec3f(C.x - A.x, B.x - A.x, A.x - P.x), Vec3f(C.y - A.y, B.y - A.y, A.y - P.y));
    // jsp
    if (std::abs(b.z) == 0)
        b = Vec3f(1, 1, -1);
    b = Vec3f(1.f - (b.x + b.y) / b.z, b.y / b.z, b.x / b.z);
    return b;
}

#pragma once

#include <iostream>
#include "../raylib/include/raymath.h"

using namespace std;

struct Vec3I {
    int x;
    int y;
    int z;

    Vec3I() {}
    Vec3I(int xX, int yY, int zZ)
    {
        this->x = xX;
        this->y = yY;
        this->z = zZ;
    }
    Vec3I(const Vec3I & v) : Vec3I(v.x, v.y, v.z) {}
};

#define RIGHTVOXEL Vec3I{ -1,  0,  0 }
#define LEFTVOXEL  Vec3I{  1,  0,  0 }
#define LOWERVOXEL Vec3I{  0, -1,  0 }
#define UPPERVOXEL Vec3I{  0,  1,  0 }
#define FRONTVOXEL Vec3I{  0,  0, -1 }
#define BACKVOXEL  Vec3I{  0,  0,  1 }

inline ostream & operator << (ostream & stream, const Vec3I & v)
{
    stream << "{ " << v.x << ", " << v.y << ", " << v.z << " }";
    return stream;
}

inline Vec3I   operator + (const Vec3I   & lhs, const Vec3I   & rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z }; }
inline Vec3I   operator - (const Vec3I   & lhs, const Vec3I   & rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z }; }
inline Vector3 operator + (const Vector3 & lhs, const Vec3I   & rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z }; }
inline Vector3 operator - (const Vector3 & lhs, const Vec3I   & rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z }; }
inline Vector3 operator + (const Vec3I   & lhs, const Vector3 & rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z }; }
inline Vector3 operator - (const Vec3I   & lhs, const Vector3 & rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z }; }

inline Vec3I   & operator -= (Vec3I   & lhs, const Vec3I & rhs) { lhs = lhs - rhs; return lhs; }
inline Vec3I   & operator += (Vec3I   & lhs, const Vec3I & rhs) { lhs = lhs + rhs; return lhs; }
inline Vector3 & operator -= (Vector3 & lhs, const Vec3I & rhs) { lhs = lhs - rhs; return lhs; }
inline Vector3 & operator += (Vector3 & lhs, const Vec3I & rhs) { lhs = lhs + rhs; return lhs; }

inline bool operator == (const Vec3I & lhs, const Vec3I & rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}
inline bool operator != (const Vec3I & lhs, const Vec3I & rhs)
{
    return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
}
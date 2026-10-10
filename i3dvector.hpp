#pragma once

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
};

inline Vec3I operator + (const Vec3I & lhs, const Vec3I & rhs)
{
    return Vec3I(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

inline const Vec3I & operator += (Vec3I & lhs, const Vec3I & rhs)
{
    lhs = lhs + rhs;
    return lhs;
}

inline Vec3I operator - (const Vec3I & lhs, const Vec3I & rhs)
{
    return Vec3I(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

inline Vec3I & operator -= (Vec3I & lhs, const Vec3I & rhs)
{
    lhs = lhs - rhs;
    return lhs;
}

inline bool operator == (const Vec3I & lhs, const Vec3I & rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

inline bool operator != (const Vec3I & lhs, const Vec3I & rhs)
{
    return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
}
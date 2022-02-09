#ifndef STRUCT_H
#define STRUCT_H

struct Vec2i
{
    int x;
    int y;

    Vec2i()
    {
        this->x = 0;
        this->y = 0;
    }

    Vec2i(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    Vec2i operator * (Vec2i v)
    {
        return Vec2i(x*v.x, y*v.y);
    }
};

struct Vec3f
{
    float x;
    float y;
    float z;

    Vec3f()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    Vec3f(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vec3f(Vec2i v, float z)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = z;
    }

    Vec3f operator - (Vec3f v)
    {
        return Vec3f(x-v.x, y-v.y, z-v.z);
    }
};

#endif // STRUCT_H
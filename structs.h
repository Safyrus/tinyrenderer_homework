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

    Vec2i operator*(Vec2i v)
    {
        return Vec2i(x * v.x, y * v.y);
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

    Vec3f operator-(Vec3f v)
    {
        return Vec3f(x - v.x, y - v.y, z - v.z);
    }
};

struct Vec4f
{
    float x;
    float y;
    float z;
    float w;

    Vec4f()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->w = 0;
    }

    Vec4f(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    Vec4f(Vec3f v, float w)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = w;
    }

    Vec4f operator*(Vec4f v)
    {
        return Vec4f(x * v.x, y * v.y, z * v.z, w * v.w);
    }
};

struct Mat44
{
    Vec4f vecs[4];

    Mat44()
    {
        vecs[0] = Vec4f();
        vecs[1] = Vec4f();
        vecs[2] = Vec4f();
        vecs[3] = Vec4f();
    }

    Mat44 operator*(Vec4f v)
    {
        Mat44 ret = Mat44();
        ret.vecs[0]*v;
        ret.vecs[1]*v;
        ret.vecs[2]*v;
        ret.vecs[3]*v;
        return ret;
    }
};

#endif // STRUCT_H
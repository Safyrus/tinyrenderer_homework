#include "tgaimage.h"
#include "model.h"

#include "structs.h"
#include "math.hpp"

#include <math.h>
#include <iostream>

#define width 128
#define height 128

const TGAColor black = TGAColor(0, 0, 0, 255);
const TGAColor magenta = TGAColor(170, 0, 170, 255);
const TGAColor pink = TGAColor(255, 85, 255, 255);
const TGAColor red = TGAColor(255, 0, 85, 255);
const TGAColor dark_blue = TGAColor(0, 0, 85, 255);
const TGAColor grey = TGAColor(85, 85, 85, 255);
const TGAColor dark_red = TGAColor(170, 0, 85, 255);
const TGAColor orange = TGAColor(255, 170, 0, 255);
const TGAColor blue = TGAColor(0, 85, 170, 255);
const TGAColor light_blue = TGAColor(170, 170, 255, 255);
const TGAColor light_grey = TGAColor(170, 170, 170, 255);
const TGAColor yellow = TGAColor(255, 255, 85, 255);
const TGAColor azure = TGAColor(0, 170, 255, 255);
const TGAColor dark_green = TGAColor(0, 85, 0, 255);
const TGAColor green = TGAColor(85, 255, 85, 255);
const TGAColor white = TGAColor(255, 255, 255, 255);

TGAColor colors[16] = {black, magenta, pink, red, dark_blue, grey, dark_red, orange, blue, light_blue, light_grey, yellow, azure, dark_green, green, white};

void line(Vec2i v0, Vec2i v1, TGAImage &image, TGAColor color)
{
    bool steep = false;
    if (std::abs(v0.x - v1.x) < std::abs(v0.y - v1.y))
    {
        std::swap(v0.x, v0.y);
        std::swap(v1.x, v1.y);
        steep = true;
    }
    if (v0.x > v1.x)
    {
        std::swap(v0.x, v1.x);
        std::swap(v0.y, v1.y);
    }
    int dx = v1.x - v0.x;
    int dy = v1.y - v0.y;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = v0.y;
    for (int x = v0.x; x <= v1.x; x++)
    {
        if (steep)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx)
        {
            y += (v1.y > v0.y ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void square(Vec2i t0, Vec2i t1, TGAImage &image, TGAColor color)
{
    if (t0.x > t1.x)
    {
        std::swap(t0.x, t1.x);
    }
    if (t0.y > t1.y)
    {
        std::swap(t0.y, t1.y);
    }
    for (int j = 0; j <= t1.y - t0.y; j++)
    {
        for (int i = 0; i <= t1.x - t0.x; i++)
        {
            image.set(t0.x + i, t0.y + j, color);
        }
    }
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color)
{
    line(t0, t1, image, color);
    line(t1, t2, image, color);
    line(t2, t0, image, color);
}

void triangle_fill(Vec3f T1, Vec3f T2, Vec3f T3, TGAImage &image, TGAColor color, float *zbuf)
{
    Vec2i A = Vec2i((T1.x + 1) * (width / 2), (T1.y + 1) * (height / 2));
    Vec2i B = Vec2i((T2.x + 1) * (width / 2), (T2.y + 1) * (height / 2));
    Vec2i C = Vec2i((T3.x + 1) * (width / 2), (T3.y + 1) * (height / 2));

    Vec2i square_min(std::min(A.x, std::min(B.x, C.x)), std::min(A.y, std::min(B.y, C.y)));
    Vec2i square_max(std::max(A.x, std::max(B.x, C.x)), std::max(A.y, std::max(B.y, C.y)));

    float z = std::max(T1.z, std::max(T2.z, T3.z));

    for (int j = square_min.y; j <= square_max.y; j++)
    {
        for (int i = square_min.x; i <= square_max.x; i++)
        {
            Vec2i P(i, j);
            if (P.x >= width || P.x < 0 || P.y >= height || P.y < 0)
                continue;

            Vec3f b = baricenter(A, B, C, P);

            if (b.x < 0 || b.y < 0 || b.z < 0)
                continue;

            if (zbuf[P.y * width + P.x] < z)
            {
                zbuf[P.y * width + P.x] = z;
                image.set(P.x, P.y, color);
            }
        }
    }
}

void triangle_texture(Vec3f T[3], Vec2i uv[3], TGAImage &image, TGAImage &texture, float *zbuf, float intensity)
{
    Vec2i A = Vec2i((T[0].x + 1) * (width / 2), (T[0].y + 1) * (height / 2));
    Vec2i B = Vec2i((T[1].x + 1) * (width / 2), (T[1].y + 1) * (height / 2));
    Vec2i C = Vec2i((T[2].x + 1) * (width / 2), (T[2].y + 1) * (height / 2));

    Vec2i square_min(std::min(A.x, std::min(B.x, C.x)), std::min(A.y, std::min(B.y, C.y)));
    Vec2i square_max(std::max(A.x, std::max(B.x, C.x)), std::max(A.y, std::max(B.y, C.y)));

    for (int j = square_min.y; j <= square_max.y; j++)
    {
        for (int i = square_min.x; i <= square_max.x; i++)
        {

            Vec2i P(i, j);
            if (P.x >= width || P.x < 0 || P.y >= height || P.y < 0)
                continue;

            Vec3f b = baricenter(A, B, C, P);

            float z = T[0].z * b.x + T[1].z * b.y + T[2].z * b.z;

            if (b.x < 0 || b.y < 0 || b.z < 0)
                continue;

            if (zbuf[P.y * width + P.x] < z)
            {

                zbuf[P.y * width + P.x] = z;

                int u = uv[0].x * b.x + uv[1].x * b.y + uv[2].x * b.z;
                int v = uv[0].y * b.x + uv[1].y * b.y + uv[2].y * b.z;

                TGAColor color = texture.get(u, v);
                color.r *= intensity;
                color.g *= intensity;
                color.b *= intensity;
                image.set(P.x, P.y, color);
            }
        }
    }
}

int main(int argc, char **argv)
{
    std::string modelFileName = "model.obj";
    std::string modelTextureFileName = "model.tga";
    if (argc > 1)
    {
        modelFileName = argv[1];
    }
    if (argc > 2)
    {
        modelTextureFileName = argv[2];
    }

    // create empty image
    TGAImage image(width, height, TGAImage::RGB);
    square(Vec2i(0, 0), Vec2i(width, height), image, dark_blue);

    // load texture image
    TGAImage texture;
    texture.read_tga_file(modelTextureFileName.c_str());
    texture.flip_vertically();

    // z buffer
    float zbuf[height * width];
    for (int i = 0; i < height * width; i++)
    {
        zbuf[i] = -1 * __FLT_MAX__;
    }

    // camera distance
    float c = 10;
    Vec3f camPos = Vec3f(-0.2, 0, 0);
    Vec3f camRot = Vec3f(0*M_PI*2, 0.1*M_PI*2, 0*M_PI*2);
    // light direction
    Vec3f dir(-1, 0, 1);

    // load model
    Model *model = new Model(modelFileName.c_str());
    // draw model
    for (int i = 0; i < model->nfaces(); i++)
    {
        // get vertex and 2d triangle vectors
        Vec3f vects[3];
        for (int j = 0; j < 3; j++)
        {
            vects[j] = model->vert(model->face(i)[j * 2]);
            Vec3f newVec = vects[j];

            newVec.y = newVec.y * cos(camRot.x) + newVec.z * -sin(camRot.x);
            newVec.z = newVec.y * sin(camRot.x) + newVec.z * cos(camRot.x);

            newVec.x = newVec.x * cos(camRot.y) + newVec.z * sin(camRot.y);
            newVec.z = newVec.x * -sin(camRot.y) + newVec.z * cos(camRot.y);

            newVec.x = newVec.x * cos(camRot.z) + newVec.y * -sin(camRot.z);
            newVec.y = newVec.x * sin(camRot.z) + newVec.y * cos(camRot.z);

            float tmp = 1 - (newVec.z / c);

            newVec.x = (newVec.x + camPos.x) / tmp;
            newVec.y = (newVec.y + camPos.y) / tmp;
            newVec.z = (newVec.z + camPos.z) / tmp;

            vects[j] = newVec;
        }

        // find angle between 2 3d vectors
        Vec3f A = vects[1] - vects[0];
        Vec3f B = vects[2] - vects[0];
        Vec3f norm = cross(A, B);
        float crossProd = norm.x * dir.x + norm.y * dir.y + norm.z * dir.z;
        float magNorm = sqrt(norm.x * norm.x + norm.y * norm.y + norm.z * norm.z);
        float magDir = sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
        float angle = crossProd / (magNorm * magDir);

        // get texture vertexes
        Vec2i vt[3];
        for (int j = 0; j < 3; j++)
        {
            Vec3f tmp = model->vertText(model->face(i)[j * 2 + 1]);
            // std::cout << tmp.x << " " << tmp.y << " " << tmp.z << std::endl;
            vt[j] = Vec2i(tmp.x * texture.get_width(), tmp.y * texture.get_height());
        }
        // std::cout << vt[0].x << "/" << vt[0].y << " " << vt[1].x << "/" << vt[1].y << " " << vt[2].x << "/" << vt[2].y << std::endl;

        // light intensity
        // float intensity = std::cos(angle);
        float intensity = abs(angle);
        if (intensity > 0)
        {
            // TGAColor gradiant[9] = {black, dark_blue, blue, magenta, dark_red, red, orange, yellow, white};
            TGAColor gradiant[9] = {white, yellow, orange, red, dark_red, magenta, blue, dark_blue, black};
            int colorIndex = intensity * 9;
            // triangle_fill(vects[0], vects[1], vects[2], image, TGAColor(255*intensity, 255*intensity, 255*intensity, 255), zbuf);
            triangle_fill(vects[0], vects[1], vects[2], image, gradiant[colorIndex], zbuf);
            // triangle_texture(vects, vt, image, texture, zbuf, intensity);
        }
    }

    // save image
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");

    return 0;
}

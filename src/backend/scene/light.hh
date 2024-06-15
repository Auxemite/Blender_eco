#pragma once

#include "../utils/color.hh"

class Light
{
public:
    Point3 center;
    float power = 1.0f;
    Color color={};
};

class Point_Light : public Light
{
public:

    Point_Light()= default;
    Point_Light(const Point3& center_, float power_)
    {
        center = center_;
        power = power_;
        color = Color(1.0f, 1.0f, 1.0f);
    };

    Point_Light(const Point3& center_, float power_, Color color_)
    {
        center = center_;
        power = power_;
        color = color_;
    };
};

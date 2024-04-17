#pragma once

#include "../utils/color.hh"

class Light
{
public:
    Point3 center;
    double power = 1;
    Color color;
};

class Point_Light : public Light
{
public:

    Point_Light()= default;
    Point_Light(Point3 center_, double power_)
    {
        center = center_;
        power = power_;
        color = Color(1., 1., 1.);
    };

    Point_Light(Point3 center_, double power_, Color color_)
    {
        center = center_;
        power = power_;
        color = color_;
    };
};

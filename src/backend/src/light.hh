//
// Created by ernes on 15/03/2024.
//

#ifndef RAYTRACER_LIGHT_HH
#define RAYTRACER_LIGHT_HH

#include "color.hh"

class Light
{};

class Point_Light : public Light
{
public:
    Point3 center = Point3();
    double power = 1;
    Color color = Color(1.0, 1.0, 1.0);

    Point_Light()= default;
    Point_Light(Point3 center_, double power_)
    {
        center = center_;
        power = power_;
    };
    Point_Light(Point3 center_, double power_, Color color_)
    {
        center = center_;
        power = power_;
        color = color_;
    };
};

#endif //RAYTRACER_LIGHT_HH

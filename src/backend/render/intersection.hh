#pragma once

#include "../scene/scene.hh"
#include <climits>
Point3 null_point = Point3(FLT_MAX, FLT_MAX, FLT_MAX);

class Intersection
{
    public :
        Intersection()= default;
        Intersection(const Point3& origin_, const Vector3& dir_);

        void update(const Point3& origin_, const Vector3& dir_);
        void fast_throw_ray(Shape *object);
        void fast_throw_ray(const Scene& scene);
        void throw_ray(Shape *object);
        void throw_ray(const Scene& scene);

        Color diffuse(Vector3 light_ray, Vector3 normale) const;
        Color specular(Light *light, const Vector3& light_ray, const Vector3& refaction) const;

        static bool inside_object(const Scene& scene, Light *light);

        Point3 origin;
        Vector3 dir;
        Point3 inter_loc;
        Shape* object={};
};
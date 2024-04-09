#pragma once

#define PI 3.14159265

#include "../scene/scene.hh"
#include "../image/image.hh"

class Intersection {
    public :
        Intersection()= default;
        Intersection(Point3 origin_, Vector3 dir_);

        void update(Point3 origin_, Vector3 dir_);
        void throw_ray(const Scene& scene);

        Color bg_color();
        Color fast_ray_color(const Scene& scene);
        Color ray_color(const Scene& scene, int recursive);
        Color diffuse(Vector3 light_ray, Vector3 normale);
        Color specular(Point_Light light, Vector3 light_ray, Vector3 refaction);

        static bool inside_object(const Scene& scene, Point_Light light);

        Point3 origin;
        Vector3 dir;
        Point3 inter_loc;
        Sphere sphere;
};
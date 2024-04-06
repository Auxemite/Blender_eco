//
// Created by ernes on 22/03/2024.
//

#ifndef RAYTRACER_INTERSECTION_HH
#define RAYTRACER_INTERSECTION_HH

#define PI 3.14159265

#include "vector3.hh"
#include "object/object.hh"
#include "scene.hh"
#include "image.hh"

class Intersection {
    public :
        Intersection()= default;
        Intersection(Point3 origin_, Vector3 dir_);

        void update(Point3 origin_, Vector3 dir_);
        void throw_ray(const Scene& scene);
        Color bg_color();
        Color ray_color(const Scene& scene, int recursive);
        Color diffuse(Vector3 light_ray, Vector3 normale);
        Color specular(Point_Light light, Vector3 light_ray, Vector3 refaction);
        static bool inside_object(const Scene& scene, Point_Light light);

        Point3 origin;
        Vector3 dir;
        Point3 inter_loc;
        Sphere sphere;
};

#endif //RAYTRACER_INTERSECTION_HH

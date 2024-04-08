//
// Created by ernes on 08/04/2024.
//

#ifndef BLENDER_ECO_BACKEND_MAIN_HH
#define BLENDER_ECO_BACKEND_MAIN_HH

#include "render.hh"

namespace backend {
    void bounce(const vector<Sphere> &s, const vector<Point_Light> &lights);
    void around(const vector<Sphere> &s, const vector<Point_Light> &lights);
    void around2(const vector<Point_Light> &lights);
    void sunset(const vector<Sphere> &s, Point3 cam_loc, Color sun_color);
    int backend_main();
}

#endif //BLENDER_ECO_BACKEND_MAIN_HH

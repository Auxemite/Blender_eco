//
// Created by ernes on 24/03/2024.
//

#ifndef RAYTRACER_RENDER_HH
#define RAYTRACER_RENDER_HH

#include <utility>

#include "src/camera.hh"
#include "src/image.hh"

auto aspect_ratio = 16.0 / 9.0;
int width = 640;
int height = static_cast<int>(width / aspect_ratio);

void render_image(vector<Sphere> spheres, vector<Point_Light> lights, Point3 cam_loc, Point3 look_at,
                  const string& pathname) {
    Camera camera = Camera(cam_loc, look_at, width, height);
    Scene scene = Scene(std::move(spheres), std::move(lights), camera);
    Image image(width, height);
    image.render(scene);
    image.save_as_ppm(pathname);
}

void render_all_cams(const vector<Sphere>& spheres, const vector<Point_Light>& lights, vector<Point3> cam_loc, Point3 look_at,
                     const string& pathname) {
    for (int i = 0; i < cam_loc.size(); ++i) {
        render_image(spheres, lights, cam_loc[i], look_at, pathname + to_string(i) + ".ppm");
//        render_image(spheres, lights, cam_loc[i], look_at, pathname + static_cast<char>('a' + i) + ".ppm");
    }
}

void render_all_lights(const vector<Sphere>& spheres, vector<Point3> light_loc, int power, Point3 cam_loc, Color sun,
                       const string& pathname) {
    for (int i = 0; i < light_loc.size(); ++i) {
        vector<Point_Light> lights;
        lights.emplace_back(light_loc[i], power, sun);
        render_image(spheres, lights, cam_loc, {0,0,1}, pathname +  to_string(i) + ".ppm");
//        render_image(spheres, lights, cam_loc, {0,0,1}, pathname +  static_cast<char>('a' + i) + ".ppm");
    }
}

#endif //RAYTRACER_RENDER_HH

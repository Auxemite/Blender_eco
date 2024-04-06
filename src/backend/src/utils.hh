//
// Created by ernes on 22/03/2024.
//

#ifndef RAYTRACER_UTILS_HH
#define RAYTRACER_UTILS_HH

#include <iostream>
#include <list>
#include "color.hh"
#include "texture_material.hh"
#include "vector3.hh"

namespace u {
    Color red = Color(1.0, 0.0, 0.0);
    Color green = Color(0.0, 1.0, 0.0);
    Color blue = Color(0.0, 0.0, 1.0);
    Color cyan = Color(0.0, 1.0, 1.0);
    Color magenta = Color(1.0, 0.0, 1.0);
    Color yellow = Color(1.0, 1.0, 0.0);
    Color white = Color(1.0, 1.0, 1.0);
    Color gray = Color(0.5, 0.5, 0.5);
    Color black = Color(0.0, 0.0, 0.0);
    Color purple = Color(1.0, 0.0, 0.5);
    Color turquoise = Color(0.0, 1.0, 0.5);
    Color orange = Color(1, 0.5, 0);
    // Experimental
    Color smooth_bleu = Color(0.3, 0.1, 0.7);
    Color gray_sky = Color(0.4, 0.4, 0.5);

    Uniform_Texture basic(Color color) {
        return Uniform_Texture(color, 0, 0);
    }
    Uniform_Texture simple(Color color) {
        return Uniform_Texture(color, 1, 0.1);
    }
    Uniform_Texture plastic(Color color) {
        return Uniform_Texture(color, 10, 0.5);
    }
    Uniform_Texture metal(Color color) {
        return Uniform_Texture(color, 50, 1);
    }

    Point3 p0 = Point3(0,0,0);
}

#endif //RAYTRACER_UTILS_HH

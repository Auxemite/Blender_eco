#pragma once

#include "color.hh"
#include "../utils/vector3.hh"

struct material
{
    Color color;
    double kd, ks, ns;
};

class Texture_Material {
    public:

    virtual struct material *get_texture() = 0;
    virtual struct material *get_texture(const Point3& position) = 0;
};

class Uniform_Texture : Texture_Material {
    public:
        struct material mat;

        Uniform_Texture()= default;
        Uniform_Texture(Color color, double kd, double ks, double ns);

    struct material *get_texture();
    struct material *get_texture(const Point3& position);
};

namespace basic
{
    namespace texture
    {
        static Uniform_Texture basic(Color color) {
            return Uniform_Texture(color, 1, 0, 0);
        }
        static Uniform_Texture simple(Color color) {
            return Uniform_Texture(color, 1, 1, 0.1);
        }
        static Uniform_Texture plastic(Color color) {
            return Uniform_Texture(color, 1, 10, 0.5);
        }
        static Uniform_Texture metal(Color color) {
            return Uniform_Texture(color, 1, 50, 1);
        }
    }
}
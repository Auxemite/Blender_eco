#pragma once

#include "color.hh"
#include "../utils/vector3.hh"

struct Texture
{
    double kd, ks, ns;
};

struct Material
{
    Color color;
    Texture texture;
};

class Texture_Material {
    public:

    virtual struct Material get_texture(const Point3& position) const = 0;
};

class Uniform_Texture : public Texture_Material {
    public:
        struct Material material;

        Uniform_Texture()= default;
        Uniform_Texture(Color color, Texture texture);
        Uniform_Texture(Texture texture, Color color);
        Uniform_Texture(Color color, double kd, double ks, double ns);

    struct Material get_texture(const Point3& position) const override;
};

namespace basic
{
    namespace texture
    {
        static struct Texture basic = {0.5, 0.5, 1};
        static struct Texture simple = {0.9, 0.1, 1};
        static struct Texture plastic = {0.8, 0.5, 10};
        static struct Texture metal = {0, 1, 150};
    }
}
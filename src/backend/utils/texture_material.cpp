#include "texture_material.hh"

Uniform_Texture::Uniform_Texture(Color color, Texture texture)
{
    material.color = color;
    material.texture = texture;
}

Uniform_Texture::Uniform_Texture(Texture texture, Color color)
{
    material.color = color;
    material.texture = texture;
}

Uniform_Texture::Uniform_Texture(Color color, double kd, double ks, double ns)
{
    material.color = color;
    material.texture = {kd, ks, ns};
}

struct Material Uniform_Texture::get_texture(const Point3& position) const { return material; }
#include "texture_material.hh"

Uniform_Texture::Uniform_Texture(Color color, Texture texture)
{
    mat.color = color;
    mat.texture = texture;
}

Uniform_Texture::Uniform_Texture(Texture texture, Color color)
{
    mat.color = color;
    mat.texture = texture;
}

Uniform_Texture::Uniform_Texture(Color color, double kd, double ks, double ns)
{
    mat.color = color;
    mat.texture = {kd, ks, ns};
}

// struct material *Uniform_Texture::get_texture() { return &mat; }
struct Material Uniform_Texture::get_texture(const Point3& position) const { return mat; }
#include "texture_material.hh"

Uniform_Texture::Uniform_Texture(Color color, double kd, double ks, double ns)
{
    mat.color = color;
    mat.kd = kd;
    mat.ks = ks;
    mat.ns = ns;
}

struct material *Uniform_Texture::get_texture() { return &mat; }
struct material *Uniform_Texture::get_texture(const Point3& position) { return &mat; }
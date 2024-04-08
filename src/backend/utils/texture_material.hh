#pragma once

#include "color.hh"

class Texture_Material {
    public:
        Color color= Color(0, 0, 0);
        double ns = 0;
        double ks = 0;

        Texture_Material()= default;
};

class Uniform_Texture : Texture_Material {
    public:
        Color color= Color(0, 0, 0);
        double ns = 0;
        double ks = 0;

        Uniform_Texture()= default;
        explicit Uniform_Texture(Color color_, double ns_, double ks_) {
            color = color_;
            ns = ns_;
            ks = ks_;
        };
};

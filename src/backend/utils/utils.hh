#pragma once

//#include "texture_material.hh"

inline double sqr(double x) { return x * x; }
inline double discr(double a, double b, double c) { return sqr(b) - (4 * a * c); }
inline double abs_(double x) { return x < 0 ? -x : x; }

/* namespace utils
{
    namespace color 
    {
        // Basic
        const Color red = Color(1.0, 0.0, 0.0);
        const Color green = Color(0.0, 1.0, 0.0);
        const Color blue = Color(0.0, 0.0, 1.0);
        const Color cyan = Color(0.0, 1.0, 1.0);
        const Color magenta = Color(1.0, 0.0, 1.0);
        const Color yellow = Color(1.0, 1.0, 0.0);
        const Color white = Color(1.0, 1.0, 1.0);
        const Color gray = Color(0.5, 0.5, 0.5);
        const Color black = Color(0.0, 0.0, 0.0);
        const Color purple = Color(1.0, 0.0, 0.5);
        const Color turquoise = Color(0.0, 1.0, 0.5);
        const Color orange = Color(1, 0.5, 0);

        // Experimental
        const Color smooth_bleu = Color(0.3, 0.1, 0.7);
        const Color gray_sky = Color(0.4, 0.4, 0.5);
    }

    namespace texture
    {
        static Uniform_Texture basic(Color color) {
            return Uniform_Texture(color, 0, 0);
        }
        static Uniform_Texture simple(Color color) {
            return Uniform_Texture(color, 1, 0.1);
        }
        static Uniform_Texture plastic(Color color) {
            return Uniform_Texture(color, 10, 0.5);
        }
        static Uniform_Texture metal(Color color) {
            return Uniform_Texture(color, 50, 1);
        }
    }
} */

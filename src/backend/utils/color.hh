#pragma once

#include <ostream>

inline void cap_color(double *value)
{
    (*value) = (*value) > 1.0 ? 1.0: (*value);
    (*value) = (*value) < 0.0 ? 0.0: (*value);
}

class Color {
    public :
        double r, g, b;

        Color()= default;
        Color (double r_, double g_, double b_) : r(r_), g(g_), b(b_) {};

        void cap()
        {
            cap_color(&r);
            cap_color(&g);
            cap_color(&b);
        }

    // Operator
    Color& operator=(const Color& col);
    Color& operator+=(const Color& col);
    Color& operator/=(const Color& col);
};

// Overloaded operators
inline std::ostream& operator<<(std::ostream& out, const Color& col);

inline bool operator==(const Color &c1, const Color &c2) {
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

inline bool operator!=(const Color &c1, const Color &c2) {
    return !(c1 == c2);
}

inline Color operator+(const Color& c1, const Color& c2)
{
    return Color(c1.r + c2.r,
                 c1.g + c2.g,
                 c1.b + c2.b);
}

inline Color operator+(const Color& c, float f)
{
    return Color(c.r + f,
                 c.g + f,
                 c.b + f);
}

inline Color operator+(float f, const Color& c)
{
    return Color(c.r + f,
                 c.g + f,
                 c.b + f);
}

inline Color operator-(const Color& c1, const Color& c2)
{
    return Color(c1.r - c2.r,
                 c1.g - c2.g,
                 c1.b - c2.b);
}

inline Color operator-(const Color& c, float f)
{
    return Color(c.r - f,
                 c.g - f,
                 c.b - f);
}

inline Color operator-(float f, const Color& c)
{
    return Color(f - c.r,
                 f - c.g,
                 f - c.b);
}

inline Color operator*(const Color& c1, const Color& c2)
{
    return Color(c1.r * c2.r,
                 c1.g * c2.g,
                 c1.b * c2.b);
}

inline Color operator*(const Color& c, float f)
{
    return Color(c.r * f,
                 c.g * f,
                 c.b * f);
}

inline Color operator*(float f, const Color& c)
{
    return Color(f * c.r,
                 f * c.g,
                 f * c.b);
}


inline Color operator/(const Color& c1, const Color& c2)
{
    return Color(c1.r / c2.r,
                 c1.g / c2.g,
                 c1.b / c2.b);
}

inline Color operator/(const Color& c, float f)
{
    return Color(c.r / f,
                 c.g / f,
                 c.b / f);
}

inline Color operator/(float f, const Color& c)
{
    return Color(f / c.r,
                 f / c.g,
                 f / c.b);
}

inline Color cap(const Color &c) {
    Color color(c.r, c.g, c.b);

    if (color.r > 1.0)
        color.r = 1.0;
    if (color.g > 1.0)
        color.g = 1.0;
    if (color.b > 1.0)
        color.b = 1.0;

    if (color.r < 0.0)
        color.r = 0.0;
    if (color.g < 0.0)
        color.g = 0.0;
    if (color.b < 0.0)
        color.b = 0.0;

    return color;
}

namespace basic
{
    namespace color
    {
        // Basic
        static const Color red(1, 0, 0);
        static const Color green(0, 1, 0);
        static const Color blue(0, 0, 1);
        static const Color magenta(1, 0, 1);
        static const Color cyan(0, 1, 1);
        static const Color yellow(1, 1, 0);
        static const Color black(0, 0, 0);
        static const Color white(1, 1, 1);
        const Color purple(1.0, 0.0, 0.5);
        const Color turquoise(0.0, 1.0, 0.5);
        const Color orange(1, 0.5, 0);

        
        // Experimental
        static const Color smooth_blue(0.3, 0.1, 0.7);
        static const Color gray_sky(0.4, 0.4, 0.5);
        static const Color brown(0.8, 0.5, 0.1);
        static const Color light_gray(0.75, 0.75, 0.75);
        static const Color gray(0.5, 0.5, 0.5);
        static const Color dark_gray(0.25, 0.25, 0.25);
        static const Color dark_dark_gray(0.1, 0.1, 0.1);
        static const Color light_blue(0.75, 0.75, 1);
        static const Color dark_yellow(0.5, 0.5, 0);
        static const Color dark_blue(0, 0, 0.25);
        static const Color dark_magenta(0.25, 0, 0.25);
        static const Color background_blue(0.2, 0.2, 0.3);
    }
}

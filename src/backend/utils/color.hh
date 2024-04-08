#pragma once

class Color {
    public :
        Color()= default;
        Color (double r_, double g_, double b_) : r(r_), g(g_), b(b_) {};
        double r;
        double g;
        double b;
};

inline bool operator==(const Color &c1, const Color &c2) {
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

inline bool operator!=(const Color &c1, const Color &c2) {
    return !(c1 == c2);
}

inline Color operator*(double l, const Color &c) {
    return {c.r * l, c.g * l, c.b * l};
}

inline Color operator*(const Color &c, double l) {
    return l * c;
}

inline Color operator+(const Color &c1, const Color &c2) {
    return {c1.r + c2.r, c1.g + c2.g, c1.b + c2.b};
}

inline Color operator-(const Color &c1, const Color &c2) {
    return {c1.r - c2.r, c1.g - c2.g, c1.b - c2.b};
}

inline Color operator*(const Color &c1, const Color &c2) {
    return {c1.r * c2.r, c1.g * c2.g, c1.b * c2.b};
}

inline Color operator/(const Color &c, const double &l) {
    return {c.r / l, c.g / l, c.b / l};
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
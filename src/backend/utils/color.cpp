#include "color.hh"

//Operator
Color& Color::operator=(const Color& col)
{
    r = col.r;
    g = col.g;
    b = col.b;

    return *this;
}

Color& Color::operator+=(const Color& col)
{
    r += col.r;
    g += col.g;
    b += col.b;

    return *this;
}

Color& Color::operator/=(const Color& col)
{
    r /= col.r;
    g /= col.g;
    b /= col.b;

    return *this;
} 

std::ostream& operator<<(std::ostream& out, const Color& col)
{
    return out << '(' << col.r << 
                  ", " << col.g <<
                  ", " << col.b << ")" << std::endl;
}
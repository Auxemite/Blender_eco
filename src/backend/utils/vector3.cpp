#include <iostream>
#include <cmath>
#include <string>

#include "utils.hh"
#include "vector3.hh"


static std::string vector3_msg_create = "Create Vector3 : ";


    // Constructor & Destructor
Vector3::Vector3(): x(0), y(0), z(0) {}
Vector3::Vector3(double f): x(f), y(f), z(f) {}
Vector3::Vector3(double x, double y, double z): x(x), y(y), z(z) {}
Vector3::Vector3(const Vector3& v): x(v.x), y(v.y), z(v.z) {}


    // Methods
double Vector3::length_sqr()
{
    return sqr(x) + sqr(y) + sqr(z);
}

double Vector3::length()
{
    return std::sqrt(length_sqr());
}

double Vector3::dot(const Vector3& v) const
{
    return x * v.x + y * v.y + z * v.z;
}

double Vector3::normalize()
{
    double len = length();

    *this /= len;

    return len;
}

Vector3 Vector3::norm() const
{
    Vector3 v(*this);
    v.normalize();

    return v;
}

Vector3 Vector3::cross(const Vector3& v) const
{
    return Vector3(y * v.z - z * v.y,
                   z * v.x - x * v.z,
                   x * v.y - y * v.x);
}

std::string Vector3::to_string() const
{
    return '(' + std::to_string(x)
    + ", " + std::to_string(y)
    + ", " + std::to_string(z) + ')';
};


    // Usefull Operators on Vectors
double dot(const Vector3& v1, const Vector3& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 cross(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.y * v2.z - v1.z * v2.y,
                   v1.z * v2.x - v1.x * v2.z,
                   v1.x * v2.y - v1.y * v2.x);
}

std::ostream& operator<<(std::ostream& out, const Vector3& vect)
{
    out << '(' << std::to_string(vect.x);
    out << ", " << std::to_string(vect.y);
    out << ", " << std::to_string(vect.z) << ')';

    return out;
}



    // Operators
Vector3& Vector3::operator=(const Vector3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;

    return *this;
}

Vector3& Vector3::operator+=(const Vector3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;

    return *this;
}

Vector3 Vector3::operator-() const
{
    return Vector3(-x, -y, -z);
}


Vector3& Vector3::operator*=(const double& f)
{
    x *= f;
    y *= f;
    z *= f;

    return *this;
}

Vector3& Vector3::operator/=(const double& f)
{
    x /= f;
    y /= f;
    z /= f;

    return *this;
}

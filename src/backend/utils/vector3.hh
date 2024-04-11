#pragma once

#include <iostream>
#include "utils.hh"

struct Vector3
{
    double x;
    double y;
    double z;

    // Constructor & Destructor
    Vector3();
    Vector3(double f);
    Vector3(double x, double y, double z);
    Vector3(const Vector3& v);

    ~Vector3() = default;

    // Methods
    double length_sqr();
    double length();

    double normalize();
    Vector3 norm();

    double dot(const Vector3& v) const;
    Vector3 cross(const Vector3& v) const;

    // Operators
    Vector3& operator=(const Vector3& v);
    Vector3& operator+=(const Vector3& v);
    Vector3& operator-=(const Vector3& v);
    Vector3 operator-() const;
    Vector3& operator*=(const double& f);
    Vector3& operator/=(const double& f);
};

typedef Vector3 Point3;

// Usefull operators on Vectors
double dot(const Vector3& v1, const Vector3& v2);
Vector3 cross(const Vector3& v1, const Vector3& v2);
std::ostream& operator<<(std::ostream& out, const Vector3& vect);

// Overloaded operators
inline Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.x + v2.x,
                   v1.y + v2.y,
                   v1.z + v2.z);
}

inline Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.x - v2.x,
                   v1.y - v2.y,
                   v1.z - v2.z);
}

inline Vector3 operator*(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.x * v2.x,
                   v1.y * v2.y,
                   v1.z * v2.z);
}

inline Vector3 operator*(const Vector3& v, double f)
{
    return Vector3(v.x * f,
                   v.y * f,
                   v.z * f);
}

inline Vector3 operator*(double f, const Vector3& v)
{
    return Vector3(f * v.x,
                   f * v.y,
                   f * v.z);
}

inline Vector3 operator/(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.x / v2.x,
                   v1.y / v2.y,
                   v1.z / v2.z);
}

inline Vector3 operator/(const Vector3& v, double f)
{
    return Vector3(v.x / f,
                   v.y / f,
                   v.z / f);
}

inline Vector3 operator/(double f, const Vector3& v)
{
    return Vector3(f / v.x,
                   f / v.y,
                   f / v.z);
}

inline bool operator==(const Vector3 &v1, const Vector3 &v2) 
{
    static const double delta = 0.001;
    return abs_(v1.x - v2.x) < delta &&
           abs_(v1.y - v2.y) < delta &&
           abs_(v1.z - v2.z) < delta;
}

inline bool operator!=(const Vector3 &v1, const Vector3 &v2) 
{
    return ! (v1 == v2);
}
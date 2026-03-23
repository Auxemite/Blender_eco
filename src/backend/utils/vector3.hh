#pragma once

#include <iostream>
#include "utils.hh"

struct Vector3
{
    float x;
    float y;
    float z;

    // Constructor & Destructor
    Vector3();
    Vector3(float f);
    Vector3(float x, float y, float z);
    Vector3(const Vector3& v);

    ~Vector3() = default;

    // Methods
    float length_sqr();
    float length();

    float normalize();
    Vector3 norm() const;

    float dot(const Vector3& v) const;
    Vector3 cross(const Vector3& v) const;

    std::string to_string() const;

    // Operators
    Vector3& operator=(const Vector3& v);
    Vector3& operator+=(const Vector3& v);
    Vector3& operator-=(const Vector3& v);
    Vector3 operator-() const;
    Vector3& operator*=(const float& f);
    Vector3& operator/=(const float& f);
};

typedef Vector3 Point3;

// Usefull operators on Vectors
float dot(const Vector3& v1, const Vector3& v2);
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
     return Vector3(v1.y * v2.z - v1.z * v2.y,
                    v1.z * v2.x - v1.x * v2.z,
                    v1.x * v2.y - v1.y * v2.x);
}

inline Vector3 operator*(const Vector3& v, float f)
{
    return Vector3(v.x * f,
                   v.y * f,
                   v.z * f);
}

inline Vector3 operator*(float f, const Vector3& v)
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

inline Vector3 operator/(const Vector3& v, float f)
{
    return Vector3(v.x / f,
                   v.y / f,
                   v.z / f);
}

inline Vector3 operator/(float f, const Vector3& v)
{
    return Vector3(f / v.x,
                   f / v.y,
                   f / v.z);
}

inline bool operator==(const Vector3 &v1, const Vector3 &v2) 
{
    static const float delta = 0.001;
    return abs_(v1.x - v2.x) < delta &&
           abs_(v1.y - v2.y) < delta &&
           abs_(v1.z - v2.z) < delta;
}

inline bool operator!=(const Vector3 &v1, const Vector3 &v2) 
{
    return ! (v1 == v2);
}
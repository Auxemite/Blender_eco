//
// Created by ernes on 15/03/2024.
//

#ifndef RAYTRACER_VECTOR3_HH
#define RAYTRACER_VECTOR3_HH

#include <iostream>
#include <valarray>

#define PI 3.14159265

using namespace std;

class Vector3 {

    public :
        Vector3() : x(0), y(0), z(0) {};
        Vector3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {};

        double x;
        double y;
        double z;

        [[nodiscard]] double norme_squared() const {
            return x*x + y*y + z*z;
        };
        [[nodiscard]] double norme() const {
            return sqrt(norme_squared());
        };

//        Vector3& operator=(const Vector3 &v) {
//            x = v.x;
//            y = v.y;
//            z = v.z;
//        }
};

inline Vector3 operator*(double l, const Vector3 &v) {
    return {v.x * l, v.y * l, v.z * l};
}

inline Vector3 operator*(const Vector3 &v, double l) {
    return l * v;
}

inline Vector3 operator+(const Vector3 &v1, const Vector3 &v2) {
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

inline Vector3 operator-(const Vector3 &v1, const Vector3 &v2) {
    return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

inline Vector3 operator/(const Vector3 &v, const double &l) {
    return {v.x / l, v.y / l, v.z / l};
}

inline Vector3 operator-(const Vector3 &v) {
    return {-1 * v.x, -1 * v.y, -1 * v.z};
}

inline bool operator==(const Vector3 &v1, const Vector3 &v2) {
    Vector3 diff = v1 - v2;
    return static_cast<int>(diff.x * 10000) == 0
    && static_cast<int>(diff.y * 10000) == 0
    && static_cast<int>(diff.z * 10000) == 0;
}

inline bool operator!=(const Vector3 &v1, const Vector3 &v2) {
    return !(v1 == v2);
}

inline double dot(const Vector3 &u, const Vector3 &v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline Vector3 unit_vector(Vector3 v) {
    return v / v.norme();
}

inline Vector3 operator*(Vector3 v1, Vector3 v2) {
    return {v1.y*v2.z - v1.z*v2.y,
            v1.z*v2.x - v1.x*v2.z,
            v1.x*v2.y - v1.y*v2.x};
}

inline void print(Vector3 v1) {
    cout << "(" << v1.x << ", " << v1.y << ", " << v1.z << ")\n";
}

typedef Vector3 Point3;

#endif //RAYTRACER_VECTOR3_HH

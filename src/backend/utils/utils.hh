#pragma once

inline double sqr(double x) { return x * x; }
inline double discr(double a, double b, double c) { return sqr(b) - (4 * a * c); }
inline double abs_(double x) { return x < 0 ? -x : x; }

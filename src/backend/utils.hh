#pragma once

inline float sqr(float x) { return x * x; }
inline float discr(float a, float b, float c) { return sqr(b) - (4 * a * c); }
inline float abs_(float x) { return x < 0 ? -x : x; }


#ifndef CENG477_RASTERIZER_DOUBLE4_H
#define CENG477_RASTERIZER_DOUBLE4_H

#include <string>

using namespace std;

namespace Rasterizer {

    struct double4 {
        double x;
        double y;
        double z;
        double w;

        __attribute__((always_inline))
        double4() {
            this->x = 0;
            this->y = 0;
            this->z = 0;
            this->w = 0;
        }

        __attribute__((always_inline))
        double4(double x, double y, double z, double w) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        __attribute__((always_inline))
        double4 operator+(double4 other) const {
            return double4(x + other.x, y + other.y, z + other.z, w + other.w);
        }

        __attribute__((always_inline))
        double4 operator-(double4 other) const {
            return double4(x - other.x, y - other.y, z - other.z, w - other.w);
        }

        __attribute__((always_inline))
        double4 operator*(double4 other) const {
            return double4(x * other.x, y * other.y, z * other.z, x * other.w);
        }

        __attribute__((always_inline))
        double4 operator*(float other) const {
            return double4(x * other, y * other, z * other, w * other);
        }

        __attribute__((always_inline))
        double4 operator/(double4 other) const {
            return double4(x / other.x, y / other.y, z / other.z, w / other.w);
        }

        __attribute__((always_inline))
        double4 operator/(float other) const {
            return double4(x / other, y / other, z / other, w / other);
        }

        __attribute__((always_inline))
        double4 operator-() const {
            return double4(-x, -y, -z, -w);
        }

        __attribute__((always_inline))
        string ToString() const {
            return "(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ", " + to_string(w) + ")";
        }
    };

    __attribute__((always_inline))
    static double4 operator/(float f, double4 v) {
        return double4(f / v.x, f / v.y, f / v.z, f / v.w);
    }

    __attribute__((always_inline))
    static double4 operator*(float f, double4 v) {
        return double4(f * v.x, f * v.y, f * v.z, f * v.w);
    }

} // Rasterizer

#endif //CENG477_RASTERIZER_DOUBLE4_H

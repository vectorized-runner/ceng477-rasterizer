
#ifndef CENG477_RAYTRACER_FLOAT3_H
#define CENG477_RAYTRACER_FLOAT3_H

#include <string>

using namespace std;


namespace Rasterizer {


    struct float3 {
        float x;
        float y;
        float z;

        __attribute__((always_inline))
        float3() {
            this->x = 0;
            this->y = 0;
            this->z = 0;
        }

        __attribute__((always_inline))
        float3(float a) {
            this->x = a;
            this->y = a;
            this->z = a;
        }

        __attribute__((always_inline))
        float3(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        __attribute__((always_inline))
        float3 yzx() const {
            return float3(y, z, x);
        }

        __attribute__((always_inline))
        float3 operator+(float3 other) const {
            return float3(x + other.x, y + other.y, z + other.z);
        }

        __attribute__((always_inline))
        float3 operator-(float3 other) const {
            return float3(x - other.x, y - other.y, z - other.z);
        }

        __attribute__((always_inline))
        float3 operator*(float3 other) const {
            return float3(x * other.x, y * other.y, z * other.z);
        }

        __attribute__((always_inline))
        float3 operator*(float other) const {
            return float3(x * other, y * other, z * other);
        }

        __attribute__((always_inline))
        float3 operator/(float3 other) const {
            return float3(x / other.x, y / other.y, z / other.z);
        }

        __attribute__((always_inline))
        float3 operator/(float other) const {
            return float3(x / other, y / other, z / other);
        }

        __attribute__((always_inline))
        float3 operator-() const {
            return float3(-x, -y, -z);
        }

        __attribute__((always_inline))
        string ToString() const {
            return "(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")";
        }
    };

    __attribute__((always_inline))
    static float3 operator/(float f, float3 v) {
        return float3(f / v.x, f / v.y, f / v.z);
    }

    __attribute__((always_inline))
    static float3 operator*(float f, float3 v) {
        return float3(f * v.x, f * v.y, f * v.z);
    }

} // Rasterizer

#endif //CENG477_RAYTRACER_FLOAT3_H

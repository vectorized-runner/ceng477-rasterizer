
#ifndef CENG477_RAYTRACER_MATH_H
#define CENG477_RAYTRACER_MATH_H

#include "float3.h"
#include <cmath>
#include <limits>

namespace Rasterizer {

    struct Math {

        constexpr static const float FloatMax = numeric_limits<float>::max();
        constexpr static const float FloatMin = numeric_limits<float>::min();
        constexpr static const float Epsilon = 0.0001f;

        __attribute__((always_inline))
        static float3 Cross(float3 x, float3 y) {
            return (x * y.yzx() - x.yzx() * y).yzx();
        }

        __attribute__((always_inline))
        static float Dot(float3 x, float3 y) {
            return x.x * y.x + x.y * y.y + x.z * y.z;
        }

        __attribute__((always_inline))
        static float Min(float x, float y) {
            return x < y ? x : y;
        }

        __attribute__((always_inline))
        static float3 Min(float3 x, float3 y) {
            return float3(Min(x.x, y.x), Min(x.y, y.y), Min(x.z, y.z));
        }

        __attribute__((always_inline))
        static float Max(float x, float y) {
            return x > y ? x : y;
        }

        __attribute__((always_inline))
        static float3 Max(float3 x, float3 y) {
            return float3(Max(x.x, y.x), Max(x.y, y.y), Max(x.z, y.z));
        }

        __attribute__((always_inline))
        static float3 Rcp(float3 x) {
            return 1.0f / x;
        }

        __attribute__((always_inline))
        static float Length(float3 x) {
            return sqrt(Dot(x, x));
        }

        __attribute__((always_inline))
        static float LengthSq(float3 v) {
            return Dot(v, v);
        }

        __attribute__((always_inline))
        static float DistanceSq(float3 x, float3 y) {
            return LengthSq(y - x);
        }

        __attribute__((always_inline))
        static bool IsLengthEqual(float3 v, float length) {
            return abs(LengthSq(v) - length * length) < Epsilon;
        }

        __attribute__((always_inline))
        static bool IsNormalized(float3 v) {
            return IsLengthEqual(v, 1.0f);
        }

        __attribute__((always_inline))
        static float3 Normalize(float3 x) {
            return Rsqrt(Dot(x, x)) * x;
        }

        __attribute__((always_inline))
        static float Rsqrt(float x) {
            return 1.0f / sqrt(x);
        }

        __attribute__((always_inline))
        static float Degrees(float x) {
            return x * 57.295779513f;
        }

        __attribute__((always_inline))
        static float Acos(float x) {
            return acos(x);
        }

        __attribute__((always_inline))
        static bool IsNonNegative(float3 x) {
            return x.x >= 0 && x.y >= 0 && x.z >= 0;
        }

        __attribute__((always_inline))
        static float Clamp(float x, float a, float b) {
            return Max(a, Min(b, x));
        }


    };

} // RayTracer

#endif //CENG477_RAYTRACER_MATH_H

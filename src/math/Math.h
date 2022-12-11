
#ifndef CENG477_RAYTRACER_MATH_H
#define CENG477_RAYTRACER_MATH_H

#include "double4x4.h"
#include "double3.h"
#include <cmath>
#include <limits>

namespace Rasterizer {

    struct Math {

        constexpr static const float FloatMax = numeric_limits<float>::max();
        constexpr static const float FloatMin = numeric_limits<float>::min();
        constexpr static const float Epsilon = 0.0001f;

        __attribute__((always_inline))
        static double3 Cross(double3 x, double3 y) {
            return (x * y.yzx() - x.yzx() * y).yzx();
        }

        __attribute__((always_inline))
        static float Dot(double3 x, double3 y) {
            return x.x * y.x + x.y * y.y + x.z * y.z;
        }

        __attribute__((always_inline))
        static float Min(float x, float y) {
            return x < y ? x : y;
        }

        __attribute__((always_inline))
        static double3 Min(double3 x, double3 y) {
            return double3(Min(x.x, y.x), Min(x.y, y.y), Min(x.z, y.z));
        }

        __attribute__((always_inline))
        static float Max(float x, float y) {
            return x > y ? x : y;
        }

        __attribute__((always_inline))
        static double3 Max(double3 x, double3 y) {
            return double3(Max(x.x, y.x), Max(x.y, y.y), Max(x.z, y.z));
        }

        __attribute__((always_inline))
        static double3 Rcp(double3 x) {
            return 1.0f / x;
        }

        __attribute__((always_inline))
        static float Length(double3 x) {
            return sqrt(Dot(x, x));
        }

        __attribute__((always_inline))
        static float LengthSq(double3 v) {
            return Dot(v, v);
        }

        __attribute__((always_inline))
        static float DistanceSq(double3 x, double3 y) {
            return LengthSq(y - x);
        }

        __attribute__((always_inline))
        static bool IsLengthEqual(double3 v, float length) {
            return abs(LengthSq(v) - length * length) < Epsilon;
        }

        __attribute__((always_inline))
        static bool IsNormalized(double3 v) {
            return IsLengthEqual(v, 1.0f);
        }

        __attribute__((always_inline))
        static double3 Normalize(double3 x) {
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
        static bool IsNonNegative(double3 x) {
            return x.x >= 0 && x.y >= 0 && x.z >= 0;
        }

        __attribute__((always_inline))
        static float Clamp(float x, float a, float b) {
            return Max(a, Min(b, x));
        }

        static double4x4 Mul(double4x4 a, double4x4 b)
        {
            return double4x4(
                    a.c0 * b.c0.x + a.c1 * b.c0.y + a.c2 * b.c0.z + a.c3 * b.c0.w,
                    a.c0 * b.c1.x + a.c1 * b.c1.y + a.c2 * b.c1.z + a.c3 * b.c1.w,
                    a.c0 * b.c2.x + a.c1 * b.c2.y + a.c2 * b.c2.z + a.c3 * b.c2.w,
                    a.c0 * b.c3.x + a.c1 * b.c3.y + a.c2 * b.c3.z + a.c3 * b.c3.w);
        }

        static double4x4 TranslationMatrix(double3 translate){
            return double4x4(double4(1.0, 0.0, 0.0, 0.0),
                             double4(0.0, 1.0, 0.0, 0.0),
                             double4(0.0, 0.0, 1.0, 0.0),
                             double4(translate.x, translate.y, translate.z, 1.0));
        }

        static double4x4 RotationMatrix(double3 radians){
            return Mul(RotationX(radians.x), Mul(RotationY(radians.y), RotationZ(radians.z)));
        }

        // Remember: Our matrices are column-based!
        static double4x4 RotationX(double radX){
            return double4x4(double4(1.0, 0.0, 0.0, 0.0),
                             double4(0.0, cos(radX), sin(radX), 0.0),
                             double4(0.0, -sin(radX), cos(radX), 0.0 ),
                             double4(0.0, 0.0, 0.0, 1.0));
        }

        // Remember: Our matrices are column-based!
        static double4x4 RotationY(double radY){

        }

        // Remember: Our matrices are column-based!
        static double4x4 RotationZ(double radZ){

        }

        static double4x4 ScaleMatrix(double3 scale){

        }

        static double4x4 TRS(double3 position, double3 rotationRads, double3 scale){
            return Math::Mul(TranslationMatrix(position),
                             Math::Mul(RotationMatrix(rotationRads), ScaleMatrix(scale)));
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_MATH_H

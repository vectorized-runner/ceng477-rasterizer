
#ifndef CENG477_RAYTRACER_MATH_H
#define CENG477_RAYTRACER_MATH_H

#include "double4x4.h"
#include "double3.h"
#include "int2.h"
#include "../Debug.h"
#include <cmath>
#include <limits>

namespace Rasterizer {

    struct Math {

        constexpr static const double Epsilon = 0.0001;

        __attribute__((always_inline))
        static double3 Cross(double3 x, double3 y) {
            return (x * y.yzx() - x.yzx() * y).yzx();
        }

        __attribute__((always_inline))
        static double Dot(double3 x, double3 y) {
            return x.x * y.x + x.y * y.y + x.z * y.z;
        }

        static int Dot(int2 x, int2 y) {
            return x.x * y.x + x.y * y.y;
        }

        __attribute__((always_inline))
        static double Min(double x, double y) {
            return x < y ? x : y;
        }

        __attribute__((always_inline))
        static int Min(int x, int y) {
            return x < y ? x : y;
        }

        __attribute__((always_inline))
        static double3 Min(double3 x, double3 y) {
            return double3(Min(x.x, y.x), Min(x.y, y.y), Min(x.z, y.z));
        }

        __attribute__((always_inline))
        static double Max(double x, double y) {
            return x > y ? x : y;
        }

        __attribute__((always_inline))
        static int Max(int x, int y) {
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
        static double Length(double3 x) {
            return sqrt(Dot(x, x));
        }

        __attribute__((always_inline))
        static double LengthSq(double3 v) {
            return Dot(v, v);
        }

        __attribute__((always_inline))
        static double DistanceSq(double3 x, double3 y) {
            return LengthSq(y - x);
        }

        __attribute__((always_inline))
        static bool IsLengthEqual(double3 v, double length) {
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
        static double Rsqrt(double x) {
            return 1.0 / sqrt(x);
        }

        __attribute__((always_inline))
        static double Degrees(double x) {
            return x * 57.295779513;
        }

        __attribute__((always_inline))
        static double Radians(double x) {
            return x * 0.0174532925;
        }

        __attribute__((always_inline))
        static bool IsNonNegative(double3 x) {
            return x.x >= 0 && x.y >= 0 && x.z >= 0;
        }

        __attribute__((always_inline))
        static double Clamp(double x, double a, double b) {
            return Max(a, Min(b, x));
        }

        __attribute__((always_inline))
        static int Clamp(int x, int a, int b) {
            return Max(a, Min(b, x));
        }

        static double4x4 Mul(double4x4 a, double4x4 b) {
            return double4x4(
                    a.c0 * b.c0.x + a.c1 * b.c0.y + a.c2 * b.c0.z + a.c3 * b.c0.w,
                    a.c0 * b.c1.x + a.c1 * b.c1.y + a.c2 * b.c1.z + a.c3 * b.c1.w,
                    a.c0 * b.c2.x + a.c1 * b.c2.y + a.c2 * b.c2.z + a.c3 * b.c2.w,
                    a.c0 * b.c3.x + a.c1 * b.c3.y + a.c2 * b.c3.z + a.c3 * b.c3.w);
        }

        static double4 Mul(double4x4 a, double4 b) {
            return a.c0 * b.x + a.c1 * b.y + a.c2 * b.z + a.c3 * b.w;
        }

        static double3 TransformPoint(double4x4 matrix, double3 point) {
            return Math::Mul(matrix, double4(point, 1.0)).xyz();
        }

        static double4x4 TranslationMatrix(double3 translate) {
            return double4x4(double4(1.0, 0.0, 0.0, 0.0),
                             double4(0.0, 1.0, 0.0, 0.0),
                             double4(0.0, 0.0, 1.0, 0.0),
                             double4(translate.x, translate.y, translate.z, 1.0));
        }

        static double4x4 RotationMatrix(double3 radians) {
            return Mul(RotationX(radians.x), Mul(RotationY(radians.y), RotationZ(radians.z)));
        }

        static bool AreEqual(double4x4 a, double4x4 b) {
            return AreEqual(a.c0, b.c0) && AreEqual(a.c1, b.c1) && AreEqual(a.c2, b.c2) && AreEqual(a.c3, b.c3);
        }

        static bool AreEqual(double4 a, double4 b) {
            return AreEqual(a.x, b.x) && AreEqual(a.y, b.y) && AreEqual(a.z, b.z) && AreEqual(a.w, b.w);
        }

        static bool AreEqual(double a, double b) {
            return abs(a - b) < Epsilon;
        }

        // Remember: Our matrices are column-based!
        static double4x4 RotationX(double radX) {
            return double4x4(double4(1.0, 0.0, 0.0, 0.0),
                             double4(0.0, cos(radX), sin(radX), 0.0),
                             double4(0.0, -sin(radX), cos(radX), 0.0),
                             double4(0.0, 0.0, 0.0, 1.0));
        }

        // Remember: Our matrices are column-based!
        static double4x4 RotationY(double radY) {
            return double4x4(double4(cos(radY), 0.0, -sin(radY), 0.0),
                             double4(0.0, 1.0, 0.0, 0.0),
                             double4(sin(radY), 0.0, cos(radY), 0.0),
                             double4(0.0, 0.0, 0.0, 1.0));
        }

        // Remember: Our matrices are column-based!
        static double4x4 RotationZ(double radZ) {
            return double4x4(double4(cos(radZ), sin(radZ), 0.0, 0.0),
                             double4(-sin(radZ), cos(radZ), 0.0, 0.0),
                             double4(0.0, 0.0, 1.0, 0.0),
                             double4(0.0, 0.0, 0.0, 1.0));
        }

        static double4x4 ScaleMatrix(double3 scale) {
            return double4x4(double4(scale.x, 0.0, 0.0, 0.0),
                             double4(0.0, scale.y, 0.0, 0.0),
                             double4(0.0, 0.0, scale.z, 0.0),
                             double4(0.0, 0.0, 0.0, 1.0));
        }

        static double3 ScalePoint(double3 point, double3 scale) {
            auto matrix = Math::ScaleMatrix(scale);
            return Math::Mul(matrix, double4(point, 1.0)).xyz();
        }

        static double3 TranslatePoint(double3 point, double3 translation) {
            auto matrix = Math::TranslationMatrix(translation);
            return Math::Mul(matrix, double4(point, 1.0)).xyz();
        }

        static double4x4 TRS(double3 translation, double3 rotationRads, double3 scale) {
            return Math::Mul(TranslationMatrix(translation),
                             Math::Mul(RotationMatrix(rotationRads), ScaleMatrix(scale)));
        }

        static double3 LocalToWorld(double3 position, double3 translation, double3 rotation, double3 scale) {
            auto res = Math::Mul(TRS(translation, rotation, scale), double4(position.x, position.y, position.z, 1.0));
            return double3(res.x, res.y, res.z);
        }

        static bool IsZero(double v) {
            return AreEqual(v, 0.0);
        }

        static double4x4 RotateDegreesAroundAxis(double3 axis, double angle) {
            auto rads = Math::Radians(angle);
            auto v = Normalize(axis);
            auto u = Normalize(double3(-v.y, v.x, 0));
            auto w = Normalize(Math::Cross(u, v));

            auto mInverse = double4x4(
                    double4(u.x, u.y, u.z, 0.0),
                    double4(v.x, v.y, v.z, 0.0),
                    double4(w.x, w.y, w.z, 0.0),
                    double4(0.0, 0.0, 0.0, 1.0));

            auto m = double4x4(
                    double4(u.x, v.x, w.x, 0.0),
                    double4(u.y, v.y, w.y, 0.0),
                    double4(u.z, v.z, w.z, 0.0),
                    double4(0.0, 0.0, 0.0, 1.0));

            return Mul(mInverse, Mul(RotationY(rads), m));
        }

        static double Remap(double x, double oldMin, double oldMax, double newMin, double newMax) {
            return newMin + (x - oldMin) / (oldMax - oldMin) * (newMax - newMin);
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_MATH_H


#ifndef CENG477_RASTERIZER_RENDER_H
#define CENG477_RASTERIZER_RENDER_H

#include "code_template/Color.h"
#include "math/int2.h"
#include "math/double2.h"
#include "math/double3.h"
#include "math/double4x4.h"
#include "math/Math.h"
#include "Debug.h"

using namespace std;

namespace Rasterizer {

    struct Render {

        static void DrawTriangle(vector<vector<Color>>& output, int2 screen0, int2 screen1, int2 screen2, double3 color0, double3 color1, double3 color2, int2 resolution){

            auto minX = Math::Min(screen0.x, Math::Min(screen1.x, screen2.x));
            auto maxX = Math::Max(screen0.x, Math::Max(screen1.x, screen2.x));
            auto minY = Math::Min(screen0.y, Math::Min(screen1.y, screen2.y));
            auto maxY = Math::Max(screen0.y, Math::Max(screen1.y, screen2.y));

            for (int y = minY; y < maxY; ++y) {
                for (int x = minX; x < maxX; ++x) {
                    auto point = int2(x, y);
                    auto v0 = screen1 - screen0;
                    auto v1 = screen2 - screen0;
                    auto v2 = point - screen0;
                    auto d00 = (float)Math::Dot(v0, v0);
                    auto d01 = (float)Math::Dot(v0, v1);
                    auto d11 = (float)Math::Dot(v1, v1);
                    auto d20 = (float)Math::Dot(v2, v0);
                    auto d21 = (float)Math::Dot(v2, v1);
                    auto denom = d00 * d11 - d01 * d01;
                    auto beta = (d11 * d20 - d01 * d21) / denom;
                    auto gamma = (d00 * d21 - d01 * d20) / denom;
                    auto alpha = 1.0f - beta - gamma;

                    if(alpha >= 0 && beta >= 0 && gamma >= 0){
                        auto color = alpha * color0 + beta * color1 + gamma * color2;
                        DrawColor(output, int2(x, y), color, resolution);
                    }
                }
            }
        }

        static double3 GetTriangleNormal(double3 p0, double3 p1, double3 p2) {
            auto v = Math::Cross(p2 - p0, p1 - p0);
            return -v / Math::Length(v);
        }

        static double4x4 GetOrthographic(double r, double l, double t, double b, double f, double n) {
            return double4x4(
                    double4(2 / (r - l), 0, 0, 0),
                    double4(0, 2 / (t - b), 0, 0),
                    double4(0, 0, -2 / (f - n), 0),
                    double4(-(r + l) / (r - l), -(t + b) / (t - b), -(f + n) / (f - n), 1)
            );
        }

        static double4x4 GetPerspective(double r, double l, double t, double b, double f, double n) {
            return double4x4(
                    double4((2 * n) / (r - l), 0, 0, 0),
                    double4(0, (2 * n) / (t - b), 0, 0),
                    double4((r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n), -1),
                    double4(0, 0, (-2 * f * n) / (f - n), 0));
        }

        static double4x4 GetWorldToViewMatrix(double3 pos, double3 u, double3 v, double3 w) {
            auto translation = Math::TranslationMatrix(-pos);
            auto rotation = double4x4(
                    double4(u.x, v.x, w.x, 0),
                    double4(u.y, v.y, w.y, 0),
                    double4(u.z, v.z, w.z, 0),
                    double4(0, 0, 0, 1));

            return Math::Mul(rotation, translation);
        }

        static double2 WorldToViewportOrtho(double3 worldPosition, double3 cameraPos, double3 u, double3 v, double3 w,
                                            double r, double l, double t, double b, double f, double n) {
            auto ortho = GetOrthographic(r, l, t, b, f, n);
            auto cam = GetWorldToViewMatrix(cameraPos, u, v, w);
            auto vec = Math::Mul(ortho, Math::Mul(cam, double4(worldPosition, 1.0)));

            return double2(vec.x, vec.y);
        }

        static double4x4 P2O(double f, double n){
            return double4x4(
                    double4(n, 0.0, 0.0, 0.0),
                    double4(0.0, n, 0.0, 0.0),
                    double4(0.0, 0.0, f + n, -1),
                    double4(0.0, 0.0, f * n, 0.0));
        }

        static double2 WorldToViewportPerspective(double3 worldPosition, double3 cameraPos, double3 u, double3 v, double3 w,
                                   double r, double l, double t, double b, double f, double n) {

            auto per = GetPerspective(r, l, t, b, f, n);
            auto cam = GetWorldToViewMatrix(cameraPos, u, v, w);
            auto vec = Math::Mul(per, Math::Mul(cam, double4(worldPosition, 1.0)));
            // Perspective divide
            vec = vec / vec.w;

            return double2(vec.x, vec.y);
        }

        static int2 ViewportToScreenPoint(double2 viewport, int2 res) {
            auto screenX = Math::Remap(viewport.x, -1.0, 1.0, -0.5, res.x - 0.5);
            auto screenY = Math::Remap(viewport.y, -1.0, 1.0, -0.5, res.y - 0.5);

            return int2((int)round(screenX), (int)round(screenY));
        }

        static double4x4 GetLocalToWorldMatrix(
                const vector<int>& transformationIds,
                const vector<char>& transformationTypes,
                const vector<Scaling>& scalings,
                const vector<Rotation>& rotations,
                const vector<Translation>& translations) {

            auto transformationCount = transformationIds.size();
            auto result = double4x4::identity();

            for (int i = 0; i < transformationCount; ++i) {
                auto type = transformationTypes[i];
                auto id = transformationIds[i];

                switch (type) {
                    case 'r': {
                        auto item = rotations[id - 1];
                        result = Math::RotateDegreesAroundAxis(double3(item.ux, item.uy, item.uz), item.angle);
                        break;
                    }
                    case 't': {
                        auto item = translations[id - 1];
                        auto translationMatrix = Math::TranslationMatrix(double3(item.tx, item.ty, item.tz));
                        result = Math::Mul(translationMatrix, result);
                        break;
                    }
                    case 's': {
                        auto item = scalings[id - 1];
                        auto scaleMatrix = Math::ScaleMatrix(double3(item.sx, item.sy, item.sz));
                        result = Math::Mul(scaleMatrix, result);
                        break;
                    }
                    default: {
                        Debug::Log("Error: Unexpected Type Received.");
                        break;
                    }
                }
            }

            return result;
        }

        static bool IsBackfacingTriangle(double3 triangleNormal, double3 triangleCenter, double3 cameraPosition) {
            auto v = triangleCenter - cameraPosition;
            return Math::Dot(triangleNormal, v) > 0;
        }

        static void DrawLineLow(vector<vector<Color>>& output, double3 color0, double3 color1, int x0, int y0, int x1, int y1, int2 resolution) {
            auto dx = x1 - x0;
            auto dy = y1 - y0;
            auto yi = 1;
            auto dc = (color1 - color0) / dx;

            if (dy < 0) {
                yi = -1;
                dy = -dy;
            }
            auto d = (2 * dy) - dx;
            auto y = y0;
            auto c = color0;

            for (auto x = x0; x <= x1; x++) {
                DrawColor(output, int2(x, y), c, resolution);
                if (d > 0) {
                    y = y + yi;
                    d = d + (2 * (dy - dx));
                } else {
                    d = d + 2 * dy;
                }

                c = c + dc;
            }
        }

        static void DrawLineHigh(vector<vector<Color>>& output, double3 color0, double3 color1, int x0, int y0, int x1, int y1, int2 resolution) {
            auto dx = x1 - x0;
            auto dy = y1 - y0;
            auto xi = 1;
            auto dc = (color1 - color0) / dy;

            if (dx < 0) {
                xi = -1;
                dx = -dx;
            }

            auto d = (2 * dx) - dy;
            auto x = x0;
            auto c = color0;

            for (auto y = y0; y <= y1; y++) {
                DrawColor(output, int2(x, y), c, resolution);
                if (d > 0) {
                    x = x + xi;
                    d = d + (2 * (dx - dy));
                } else {
                    d = d + 2 * dx;
                }

                c = c + dc;
            }
        }

        static void
        DrawLine(vector<vector<Color>>& output, int2 p0, int2 p1, double3 color0, double3 color1, int2 resolution) {
            auto x0 = p0.x;
            auto x1 = p1.x;
            auto y0 = p0.y;
            auto y1 = p1.y;
            if (abs(y1 - y0) < abs(x1 - x0)) {
                if (x0 > x1) {
                    DrawLineLow(output, color1, color0, x1, y1, x0, y0, resolution);
                } else {
                    DrawLineLow(output, color0, color1, x0, y0, x1, y1, resolution);
                }
            } else {
                if (y0 > y1) {
                    DrawLineHigh(output, color1, color0, x1, y1, x0, y0, resolution);
                } else {
                    DrawLineHigh(output, color0, color1, x0, y0, x1, y1, resolution);
                }
            }
        }

        static void DrawColor(vector<vector<Color>>& output, int2 screenPos, double3 color, int2 resolution) {
            // Cull
            if(screenPos.x < 0 || screenPos.x >= resolution.x)
                return;
            if(screenPos.y < 0 || screenPos.y >= resolution.y)
                return;

            output[screenPos.x][screenPos.y] = Color(color.x, color.y, color.z);
        }
    };

} // Rasterizer

#endif //CENG477_RASTERIZER_RENDER_H

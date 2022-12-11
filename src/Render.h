
#ifndef CENG477_RASTERIZER_RENDER_H
#define CENG477_RASTERIZER_RENDER_H

#include "code_template/Color.h"
#include "math/int2.h"
#include "math/double2.h"
#include "math/double3.h"
#include "math/double4x4.h"
#include "math/Math.h"
#include "Debug.h"
#include "triangle.h"
#include "cam.h"

using namespace std;

namespace Rasterizer {

    struct Render {

        static void DrawTriangle(vector<vector<Color>>& output, triangle tri, cam cam, int resX, int resY) {
            auto viewportP0 = Render::WorldToViewportPerspective(tri.p0, cam.position, cam.u, cam.v, cam.w, cam.r,
                                                                 cam.l, cam.t, cam.b, cam.f, cam.n);
            auto viewportP1 = Render::WorldToViewportPerspective(tri.p1, cam.position, cam.u, cam.v, cam.w, cam.r,
                                                                 cam.l, cam.t, cam.b, cam.f, cam.n);
            auto viewportP2 = Render::WorldToViewportPerspective(tri.p2, cam.position, cam.u, cam.v, cam.w, cam.r,
                                                                 cam.l, cam.t, cam.b, cam.f, cam.n);

            auto screenP0 = Render::ViewportToScreenPoint(viewportP0, resX, resY);
            auto screenP1 = Render::ViewportToScreenPoint(viewportP1, resX, resY);
            auto screenP2 = Render::ViewportToScreenPoint(viewportP2, resX, resY);

            cout << screenP0.ToString() << endl;
            //cout << screenP1.ToString() << endl;
            cout << screenP2.ToString() << endl;

            Render::DrawLine(output, screenP0, screenP1, tri.c0, tri.c1);
            Render::DrawLine(output, screenP1, screenP2, tri.c1, tri.c2);
            Render::DrawLine(output, screenP0, screenP2, tri.c0, tri.c2);
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
                    double4(2 * n / (r - l), 0, 0, 0),
                    double4(0, 2 * n / (t - b), 0, 0),
                    double4((r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n), -1),
                    double4(0, 0, -2 * f * n / (f - n), 0));
        }

        static double4x4 GetWorldToCameraMatrix(double3 pos, double3 u, double3 v, double3 w) {
            auto translation = Math::TranslationMatrix(-pos);
            auto rotation = double4x4(
                    double4(u.x, v.x, w.x, 0),
                    double4(u.y, v.y, w.y, 0),
                    double4(u.z, v.z, w.z, 0),
                    double4(0, 0, 0, 1));

            return Math::Mul(rotation, translation);
        }

        static double4x4 GetVP(int resX, int resY) {
            return double4x4(
                    double4(resX / 2.0, 0.0, 0.0, 0.0),
                    double4(0.0, resY / 2.0, 0.0, 0.0),
                    double4(0.0, 0.0, 0.5, 0.0),
                    double4((resX - 1) / 2.0, (resY - 1) / 2.0, 0.5, 0.0)
            );
        }

        static double2 WorldToViewportOrtho(double3 worldPosition, double3 cameraPos, double3 u, double3 v, double3 w,
                                            double r, double l, double t, double b, double f, double n) {
            auto ortho = GetOrthographic(r, l, t, b, f, n);
            auto cam = GetWorldToCameraMatrix(cameraPos, u, v, w);
            auto vec = Math::Mul(ortho, Math::Mul(cam, double4(worldPosition, 1.0)));

            return double2(vec.x, vec.y);
        }

        // No way this works
        static double2
        WorldToViewportPerspective(double3 worldPosition, double3 cameraPos, double3 u, double3 v, double3 w,
                                   double r, double l, double t, double b, double f, double n) {

            auto per = GetPerspective(r, l, t, b, f, n);
            auto cam = GetWorldToCameraMatrix(cameraPos, u, v, w);
            auto vec = Math::Mul(per, Math::Mul(cam, double4(worldPosition, 1.0)));
            // Perspective divide
            vec = vec / vec.w;

            // I've added extra negative sign, doesn't work without it
            return double2(-vec.x, -vec.y);
        }

        // TODO: Consider doing with Matrix
        // TODO: Is clamping by casting the right way?
        static int2 ViewportToScreenPoint(double2 viewport, int resX, int resY) {
            Debug::Assert(abs(viewport.x) <= 1.0, "Viewport X error.");
            Debug::Assert(abs(viewport.y) <= 1.0, "Viewport Y error.");

            auto screenX = Math::Remap(viewport.x, -1.0, 1.0, -0.5, resX - 0.5);
            auto screenY = Math::Remap(viewport.y, -1.0, 1.0, -0.5, resY - 0.5);

            // TODO: Is this the right clamping method?
            return int2((int)screenX, (int)screenY);
        }

        static double4x4 GetLocalToWorldMatrix(
                const vector<int>& transformationIds,
                const vector<char>& transformationTypes,
                const vector<Scaling>& scalings,
                const vector<Rotation>& rotations,
                const vector<Translation>& translations) {

            auto transformationCount = transformationIds.size();

            auto translationMatrix = double4x4::identity();
            auto rotationMatrix = double4x4::identity();
            auto scaleMatrix = double4x4::identity();

            for (int i = 0; i < transformationCount; ++i) {
                auto type = transformationTypes[i];
                auto id = transformationIds[i];

                switch (type) {
                    case 'r': {
                        auto item = rotations[id - 1];
                        // TODO: I don't trust this works
                        rotationMatrix = double4x4::identity();
                        // rotationMatrix = Math::RotateDegreesAroundAxis(double3(item.ux, item.uy, item.uz), item.angle);
                        break;
                    }
                    case 't': {
                        auto item = translations[id - 1];
                        translationMatrix = Math::TranslationMatrix(double3(item.tx, item.ty, item.tz));
                        break;
                    }
                    case 's': {
                        auto item = scalings[id - 1];
                        scaleMatrix = Math::ScaleMatrix(double3(item.sx, item.sy, item.sz));
                        break;
                    }
                    default: {
                        Debug::Log("Error: Unexpected Type Received.");
                        break;
                    }
                }
            }

            // TRS
            return Math::Mul(translationMatrix, Math::Mul(rotationMatrix, scaleMatrix));
        }

        static bool ShouldTriangleBeCulled(double3 triangleNormal, double3 cameraForward) {
            return Math::Dot(triangleNormal, cameraForward) < 0;
        }

        static void PlotLow(vector<vector<Color>>& output, double3 color0, double3 color1, int x0, int y0, int x1, int y1) {
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
                DrawColor(output, int2(x, y), c);
                if (d > 0) {
                    y = y + yi;
                    d = d + (2 * (dy - dx));
                } else {
                    d = d + 2 * dy;
                }

                c = c + dc;
            }
        }

        static void Plot(vector<vector<Color>>& output, double3 color0, double3 color1, int2 p0, int2 p1) {
            auto x0 = p0.x;
            auto x1 = p1.x;
            auto y0 = p0.y;
            auto y1 = p1.y;
            if (abs(y1 - y0) < abs(x1 - x0)) {
                if (x0 > x1) {
                    PlotLow(output, color1, color0, x1, y1, x0, y0);
                } else {
                    PlotLow(output, color0, color1, x0, y0, x1, y1);
                }
            } else {
                if (y0 > y1) {
                    PlotHigh(output, color1, color0, x1, y1, x0, y0);
                } else {
                    PlotHigh(output, color0, color1, x0, y0, x1, y1);
                }
            }
        }

        static void PlotHigh(vector<vector<Color>>& output, double3 color0, double3 color1, int x0, int y0, int x1, int y1) {
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
                DrawColor(output, int2(x, y), c);
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
        DrawLine(vector<vector<Color>>& output, int2 screenPos0, int2 screenPos1, double3 color0, double3 color1) {
            return Plot(output, color0, color1, screenPos0, screenPos1);
        }

        static void DrawColor(vector<vector<Color>>& output, int2 screenPos, double3 color) {
            output[screenPos.x][screenPos.y] = Color(color.x, color.y, color.z);
        }
    };

} // Rasterizer

#endif //CENG477_RASTERIZER_RENDER_H

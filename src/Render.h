
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

        static double3 GetTriangleNormal(double3 p0, double3 p1, double3 p2) {
            auto v = Math::Cross(p2 - p0, p1 - p0);
            return -v / Math::Length(v);
        }

        static double4x4 GetOrthographic(double r, double l, double t, double b, double f, double n){
            return double4x4(
                    double4(2 / (r - l), 0, 0, 0),
                    double4(0, 2 / (t - b), 0, 0),
                    double4(0, 0, -2 / (f - n), 0),
                    double4(-(r + l) / (r - l), -(t + b) / (t - b), - (f + n) / (f - n), 1)
                    );
        }

        static double4x4 GetPerspectiveToOrthographic(double r, double l, double t, double b, double f, double n){
            return double4x4(
                    double4(2 * n / (r - l), 0, 0, 0),
                    double4(0, 2 * n / (t - b), 0, 0),
                    double4((r + l) / (r - l), (t + b) / (t - b), - (f + n) / (f - n), -1),
                    double4(0, 0, -2 * f * n / (f - n), 0));
        }

        static double4x4 GetWorldToCameraMatrix(double3 pos, double3 u, double3 v, double3 w){
            auto translation = Math::TranslationMatrix(-pos);
            auto rotation = double4x4(
                    double4(u.x, v.x, w.x, 0),
                    double4(u.y, v.y, w.y, 0),
                    double4(u.z, v.z, w.z, 0),
                    double4(0, 0, 0, 1));

            return Math::Mul(rotation, translation);
        }

        static double4x4 GetVP(int resX, int resY){
            return double4x4(
                    double4(resX / 2.0, 0.0, 0.0, 0.0),
                    double4(0.0, resY / 2.0, 0.0, 0.0),
                    double4(0.0, 0.0, 0.5, 0.0),
                    double4((resX - 1) / 2.0, (resY - 1) / 2.0, 0.5, 0.0)
                    );
        }

        // No way this works
        static double2 WorldToViewportPoint(double3 worldPosition, double3 cameraPos, double3 u, double3 v, double3 w,
                                               double r, double l, double t, double b, double f, double n, int resX, int resY){
            auto ortho = GetOrthographic(r, l, t, b, f, n);
            auto p2o = GetPerspectiveToOrthographic(r, l, t, b, f, n);
            auto cam = GetWorldToCameraMatrix(cameraPos, u, v, w);

            auto vec = Math::Mul(ortho, Math::Mul(p2o ,Math::Mul(cam, double4(worldPosition, 1.0))));
            // Perspective divide
            vec = vec / vec.w;

            return double2(vec.x, vec.y);
        }

        // TODO: Consider doing with Matrix
        static int2 ViewportToScreenPoint(double2 viewport, int resX, int resY){
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

                switch(type){
                    case 'r':
                    {
                        auto item = rotations[id - 1];
                        rotationMatrix = Math::RotateDegreesAroundAxis(double3(item.ux, item.uy, item.uz), item.angle);
                        break;
                    }
                    case 't':{
                        auto item = translations[id - 1];
                        translationMatrix = Math::TranslationMatrix(double3(item.tx, item.ty, item.tz));
                        break;
                    }
                    case 's':
                    {
                        auto item = scalings[id - 1];
                        scaleMatrix = Math::ScaleMatrix(double3(item.sx, item.sy, item.sz));
                        break;
                    }
                    default:
                    {
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

        static void
        DrawLine(vector <vector<Color>>& output, int2 screenPos0, int2 screenPos1, double3 color0, double3 color1) {
            auto x0 = screenPos0.x;
            auto y0 = screenPos0.y;
            auto x1 = screenPos1.x;
            auto y1 = screenPos1.y;
            auto d = 2 * (y0 - y1) + (x1 - x0);
            auto y = y0;
            auto c = color0;
            auto dc = (color1 - color0) / (x1 - x0);

            for (int x = x0; x <= x1; x++) {
                DrawColor(output, int2(x, y), c);

                if (d < 0) {
                    y = y + 1;
                    d += 2 * (y0 - y1 + x1 - x0);
                } else {
                    d += 2 * (y0 - y1);
                }

                c = c + dc;
            }
        }

        static void DrawColor(vector <vector<Color>>& output, int2 screenPos, double3 color) {
            output[screenPos.x][screenPos.y] = Color(color.x, color.y, color.z);
        }
    };

} // Rasterizer

#endif //CENG477_RASTERIZER_RENDER_H

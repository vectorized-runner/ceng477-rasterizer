
#ifndef CENG477_RASTERIZER_RENDER_H
#define CENG477_RASTERIZER_RENDER_H

#include "code_template/Color.h"
#include "math/int2.h"
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

        static double4x4 GetLocalToWorldMatrix(
                const vector<int>& transformationIds,
                const vector<char>& transformationTypes,
                const vector<Scaling>& scalings,
                const vector<Rotation>& rotations,
                const vector<Translation>& translations) {

            auto transformationCount = transformationIds.size();

            double3 translation = double3(0, 0, 0);
            double3 rotation = double3(0, 0, 0);
            double3 scale = double3(1, 1, 1);

            for (int i = 0; i < transformationCount; ++i) {
                auto type = transformationTypes[i];
                auto id = transformationIds[i];

                switch(type){
                    case 'r':
                    {
                        // TODO:
                        auto item = rotations[id - 1];
                        break;
                    }
                    case 't':{
                        auto item = translations[id - 1];
                        translation = translation + double3(item.tx, item.ty, item.tz);
                        break;
                    }
                    case 's':
                    {
                        auto item = scalings[id - 1];
                        scale = scale * double3(item.sx, item.sy, item.sz);
                        break;
                    }
                    default:
                    {
                        Debug::Log("Error: Unexpected Type Received.");
                        break;
                    }
                }
            }

            auto rotationRads = double3(
                    Math::Radians(rotation.x),
                    Math::Radians(rotation.y),
                    Math::Radians(rotation.z));

            return Math::TRS(translation, rotationRads, scale);
        }

//        static double4x4 GetRotationMatrix(){
//            return double4x4(0, 0, 0, 0);
//        }
//
//        static double4x4 GetTranslationMatrix(){
//
//        }
//

//
//        // Apply transformations
//        static double3 GetWorldPosition(double3 point){
//
//        }

        static bool ShouldTriangleBeCulled(double3 v0, double3 v1, double3 v2) {
            return false;
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


#ifndef CENG477_RASTERIZER_RENDER_H
#define CENG477_RASTERIZER_RENDER_H

#include "code_template/Color.h"
#include "math/int2.h"

using namespace std;

namespace Rasterizer {

    struct Render {

        static void DrawLine(vector<vector<Color>>& output, int2 screenPos0, int2 screenPos1, Color color) {
            auto x0 = screenPos0.x;
            auto y0 = screenPos0.y;
            auto x1 = screenPos1.x;
            auto y1 = screenPos1.y;
            auto d = 2 * (y0 - y1) + (x1 - x0);
            auto y = y0;

            for (int x = x0; x <= x1; x++) {
                DrawColor(output, int2(x, y), color);

                if (d < 0) {
                    y = y + 1;
                    d += 2 * (y0 - y1 + x1 - x0);
                } else {
                    d += 2 * (y0 - y1);
                }
            }
        }

        static void DrawColor(vector<vector<Color>>& output, int2 screenPos, Color color) {
            output[screenPos.x][screenPos.y] = color;
        }
    };

} // Rasterizer

#endif //CENG477_RASTERIZER_RENDER_H

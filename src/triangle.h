
#ifndef CENG477_RASTERIZER_TRIANGLE_H
#define CENG477_RASTERIZER_TRIANGLE_H

#include "math/double3.h"

namespace Rasterizer {

    struct triangle {
        double3 p0;
        double3 p1;
        double3 p2;
        double3 c0;
        double3 c1;
        double3 c2;
    };

} // Rasterizer

#endif //CENG477_RASTERIZER_TRIANGLE_H

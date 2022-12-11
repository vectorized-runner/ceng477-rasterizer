
#ifndef CENG477_RASTERIZER_CAM_H
#define CENG477_RASTERIZER_CAM_H

#include "math/double3.h"

namespace Rasterizer {

    struct cam {
        double3 position;
        double3 u;
        double3 v;
        double3 w;
        double n;
        double f;
        double t;
        double b;
        double l;
        double r;
    };

} // Rasterizer

#endif //CENG477_RASTERIZER_CAM_H

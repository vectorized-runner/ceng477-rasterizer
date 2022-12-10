
#ifndef CENG477_RASTERIZER_DOUBLE4_H
#define CENG477_RASTERIZER_DOUBLE4_H

namespace Rasterizer {

    class double4 {
        double x;
        double y;
        double z;
        double w;

        __attribute__((always_inline))
        double4() {
            this->x = 0;
            this->y = 0;
            this->z = 0;
            this->w = 0;
        }

        __attribute__((always_inline))
        double4(double x, double y, double z, double w) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }
    };

} // Rasterizer

#endif //CENG477_RASTERIZER_DOUBLE4_H

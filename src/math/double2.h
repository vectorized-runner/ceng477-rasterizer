
#ifndef CENG477_RASTERIZER_DOUBLE2_H
#define CENG477_RASTERIZER_DOUBLE2_H

namespace Rasterizer {

    struct double2 {
        double x;
        double y;

        __attribute__((always_inline))
        double2() {
            this->x = 0;
            this->y = 0;
        }

        __attribute__((always_inline))
        double2(double x, double y) {
            this->x = x;
            this->y = y;
        }
    };

} // Rasterizer

#endif //CENG477_RASTERIZER_DOUBLE2_H

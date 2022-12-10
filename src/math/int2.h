
#ifndef CENG477_RASTERIZER_INT2_H
#define CENG477_RASTERIZER_INT2_H

namespace Rasterizer {

    struct int2 {
        int x;
        int y;

        __attribute__((always_inline))
        int2() {
            this->x = 0;
            this->y = 0;
        }

        __attribute__((always_inline))
        int2(int x, int y) {
            this->x = x;
            this->y = y;
        }
    };

} // Rasterizer

#endif //CENG477_RASTERIZER_INT2_H

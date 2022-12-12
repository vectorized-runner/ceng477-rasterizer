
#ifndef CENG477_RASTERIZER_INT2_H
#define CENG477_RASTERIZER_INT2_H

#include "double2.h"

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

        string ToString() const {
            return "(" + to_string(x) + ", " + to_string(y) + ")";
        }

        double2 ToDouble2(){
           return double2(x, y);
        }
    };

} // Rasterizer

#endif //CENG477_RASTERIZER_INT2_H


#ifndef CENG477_RASTERIZER_DOUBLE4X4_H
#define CENG477_RASTERIZER_DOUBLE4X4_H

#include "double4.h"

using namespace std;

namespace Rasterizer {

    struct double4x4 {
        double4 c0;
        double4 c1;
        double4 c2;
        double4 c3;

        double4x4(double4 c0, double4 c1, double4 c2, double4 c3) {
            this->c0 = c0;
            this->c1 = c1;
            this->c2 = c2;
            this->c3 = c3;
        }

        static double4x4 identity() {
            return double4x4(
                    double4(1.0, 0.0, 0.0, 0.0),
                    double4(0.0, 1.0, 0.0, 0.0),
                    double4(0.0, 0.0, 1.0, 0.0),
                    double4(0.0, 0.0, 0.0, 1.0));
        }

        void print() {
            cout << "Matrix:" << endl;
            cout << "[" << c0.x << ", " << c1.x << ", " << c2.x << ", " << c3.x << "]" << endl;
            cout << "[" << c0.y << ", " << c1.y << ", " << c2.y << ", " << c3.y << "]" << endl;
            cout << "[" << c0.z << ", " << c1.z << ", " << c2.z << ", " << c3.z << "]" << endl;
            cout << "[" << c0.w << ", " << c1.w << ", " << c2.w << ", " << c3.w << "]" << endl;
        }
    };

} // Rasterizer

#endif //CENG477_RASTERIZER_DOUBLE4X4_H

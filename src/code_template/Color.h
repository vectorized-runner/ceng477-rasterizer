#ifndef __COLOR_H__
#define __COLOR_H__

#include <iostream>
#include "../math/double3.h"

using namespace Rasterizer;
class Color
{
public:
    double r, g, b;

    Color();
    Color(double r, double g, double b);
    Color(const Color &other);
    friend std::ostream& operator<<(std::ostream& os, const Color& c);

    double3 ToDouble3() const{
        return double3(r, g, b);
    }
};

#endif
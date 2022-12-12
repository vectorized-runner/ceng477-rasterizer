#ifndef __VEC3_H__
#define __VEC3_H__

#include <iostream>
#include "../math/double3.h"

using namespace std;
using namespace Rasterizer;

class Vec3
{
public:
    double x, y, z;
    int colorId;

    Vec3();
    Vec3(double x, double y, double z, int colorId);

    friend std::ostream& operator<<(std::ostream& os, const Vec3& v);

    double3 GetPos() const{
        return double3(x, y, z);
    }

    void SetPos(double3 value){
        x = value.x;
        y = value.y;
        z = value.z;
    }
};

#endif
#include "Vec3.h"
#include <iomanip>

using namespace std;

Vec3::Vec3()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->colorId = -1;
}

Vec3::Vec3(double x, double y, double z, int colorId)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->colorId = colorId;
}

ostream& operator<<(ostream& os, const Vec3& v) {
    
    os << fixed << setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << "]";

    return os;
}
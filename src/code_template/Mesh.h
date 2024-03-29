#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include "Triangle.h"
#include <iostream>

using namespace std;

class Mesh
{

public:
    int meshId;
    int type; // 0 for wireframe, 1 for solid
    vector<int> transformationIds;
    vector<char> transformationTypes;
    vector<Triangle> triangles;

    Mesh();
    Mesh(int meshId, int type,
          vector<int> transformationIds,
          vector<char> transformationTypes,
          vector<Triangle> triangles);

    friend ostream &operator<<(ostream &os, const Mesh &m);
};

#endif
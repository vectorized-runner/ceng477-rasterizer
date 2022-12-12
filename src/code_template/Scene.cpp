#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Mesh.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"
#include <iomanip>
#include <cmath>

#include "../Debug.h"
#include "../Render.h"
#include "../math/Math.h"

using namespace tinyxml2;
using namespace std;
using namespace Rasterizer;

/*
	Transformations, clipping, culling, rasterization are done here.
	You may define helper functions.
*/
void Scene::forwardRenderingPipeline(const Camera& camera) {

    auto resolution = int2(camera.horRes, camera.verRes);

    // This is test drawing method
//    auto mytri1 = triangle();
//    mytri1.p0 = double3(-0.5, 0.0, 0.0);
//    mytri1.c0 = double3(255, 0, 0);
//    mytri1.p1 = double3(0.5, 0.0, 0.0);
//    mytri1.c1 = double3(0, 255, 0);
//    mytri1.p2 = double3(0.0, 1.0, 0.0);
//    mytri1.c2 = double3(0, 0, 255);
//
//    mytri1.p0 = Math::ScalePoint(double3(2.0, 2.0, 2.0), mytri1.p0);
//    mytri1.p1 = Math::ScalePoint(double3(2.0, 2.0, 2.0), mytri1.p1);
//    mytri1.p2 = Math::ScalePoint(double3(2.0, 2.0, 2.0), mytri1.p2);
//
//    auto mytri2 = triangle();
//    mytri2.p0 = double3(-0.5, 0.0, 0.0);
//    mytri2.c0 = double3(255, 0, 0);
//    mytri2.p1 = double3(0.5, 0.0, 0.0);
//    mytri2.c1 = double3(0, 255, 0);
//    mytri2.p2 = double3(0.0, -1.0, 0.0);
//    mytri2.c2 = double3(255, 255, 0);
//
//    auto mycam = cam();
//    mycam.u = double3(1, 0, 0);
//    mycam.v = double3(0, 1, 0);
//    mycam.w = double3(0, 0, 1);
//    mycam.position = double3(0, 0, -1);
//    mycam.n = 1;
//    mycam.f = 10;
//    mycam.t = 2;
//    mycam.b = -2;
//    mycam.l = -2;
//    mycam.r = 2;
//
//    Render::DrawTriangle(image, mytri1, mycam, resolution);
//    Render::DrawTriangle(image, mytri2, mycam, resolution);
//    return;

    // Remember: Gaze = -w = Forward
    auto meshCount = meshes.size();
    auto cameraPos = camera.pos.GetPos();
    auto u = camera.u.GetPos();
    auto v = camera.v.GetPos();
    auto w = camera.w.GetPos();
    auto n = camera.near;
    auto f = camera.far;
    auto l = camera.left;
    auto r = camera.right;
    auto b = camera.bottom;
    auto t = camera.top;

     for (int i = 0; i < meshCount; ++i) {
        const auto& mesh = meshes[i];

        Debug::Assert(mesh.transformationTypes.size() == mesh.transformationIds.size(),
                      "Transformation size doesn't match id size.");
        Debug::Assert(mesh.transformationTypes.size() > 0, "No Transformation found.");
        Debug::Assert(mesh.transformationTypes.size() <= 3, "There are more than 3 transformations, what to do?");

        auto localToWorld = Render::GetLocalToWorldMatrix(
                mesh.transformationIds,
                mesh.transformationTypes,
                scalings,
                rotations,
                translations);

        if (mesh.type == 0) {
            // Wireframe
            auto triangleCount = mesh.triangles.size();
            for (int j = 0; j < triangleCount; ++j) {
                const auto& tri = mesh.triangles[j];

                auto v0 = vertices[tri.vertexIds[0] - 1];
                auto v1 = vertices[tri.vertexIds[1] - 1];
                auto v2 = vertices[tri.vertexIds[2] - 1];

                auto worldP0 = Math::TransformPoint(localToWorld, v0.GetPos());
                auto worldP1 = Math::TransformPoint(localToWorld, v1.GetPos());
                auto worldP2 = Math::TransformPoint(localToWorld, v2.GetPos());

                auto triNormal = Render::GetTriangleNormal(worldP0, worldP1, worldP2);
                auto triCenter = (worldP0 + worldP1 + worldP2) / 3.0;

                if (cullingEnabled && Render::IsBackfacingTriangle(triNormal, triCenter, cameraPos)) {
                    continue;
                }

                auto color0 = colorsOfVertices[v0.colorId - 1].ToDouble3();
                auto color1 = colorsOfVertices[v1.colorId - 1].ToDouble3();
                auto color2 = colorsOfVertices[v2.colorId - 1].ToDouble3();

                auto viewportP0 = double2(0, 0);
                auto viewportP1 = double2(0, 0);
                auto viewportP2 = double2(0, 0);

                if(camera.projectionType == 0){
                    // Ortho
                    viewportP0 = Render::WorldToViewportOrtho(worldP0, cameraPos, u, v, w, r, l, t, b, f, n);
                    viewportP1 = Render::WorldToViewportOrtho(worldP1, cameraPos, u, v, w, r, l, t, b, f, n);
                    viewportP2 = Render::WorldToViewportOrtho(worldP2, cameraPos, u, v, w, r, l, t, b, f, n);
                }
                else if(camera.projectionType == 1){
                    // Perspective
                    viewportP0 = Render::WorldToViewportPerspective(worldP0, cameraPos, u, v, w, r, l, t, b, f, n);
                    viewportP1 = Render::WorldToViewportPerspective(worldP1, cameraPos, u, v, w, r, l, t, b, f, n);
                    viewportP2 = Render::WorldToViewportPerspective(worldP2, cameraPos, u, v, w, r, l, t, b, f, n);
                }
                else{
                    Debug::Log("Unexpected projection type.");
                }

                auto screenP0 = Render::ViewportToScreenPoint(viewportP0, resolution);
                auto screenP1 = Render::ViewportToScreenPoint(viewportP1, resolution);
                auto screenP2 = Render::ViewportToScreenPoint(viewportP2, resolution);

                // TODO: Apply clipping

                Render::DrawLine(image, screenP0, screenP1, color0, color1, resolution);
                Render::DrawLine(image, screenP1, screenP2, color1, color2, resolution);
                Render::DrawLine(image, screenP2, screenP0, color2, color0, resolution);
            }
        } else if (mesh.type == 1) {
            // TODO: Solid mesh drawing
            cout << "Solid mesh not implemented yet." << endl;
        } else {
            cout << "Error, Unexpected Mesh Type: " << mesh.type << endl;
        }
    }
}

/*
	Parses XML file
*/
Scene::Scene(const char* xmlPath) {
    const char* str;
    XMLDocument xmlDoc;
    XMLElement* pElement;

    xmlDoc.LoadFile(xmlPath);

    XMLNode* pRoot = xmlDoc.FirstChild();

    // read background color
    pElement = pRoot->FirstChildElement("BackgroundColor");
    str = pElement->GetText();
    sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

    // read culling
    pElement = pRoot->FirstChildElement("Culling");
    if (pElement != nullptr) {
        str = pElement->GetText();

        if (strcmp(str, "enabled") == 0) {
            cullingEnabled = true;
        } else {
            cullingEnabled = false;
        }
    }

    // read cameras
    pElement = pRoot->FirstChildElement("Cameras");
    XMLElement* pCamera = pElement->FirstChildElement("Camera");
    XMLElement* camElement;
    while (pCamera != nullptr) {
        Camera cam = Camera();

        pCamera->QueryIntAttribute("id", &cam.cameraId);

        // read projection type
        str = pCamera->Attribute("type");

        if (strcmp(str, "orthographic") == 0) {
            cam.projectionType = 0;
        } else {
            cam.projectionType = 1;
        }

        camElement = pCamera->FirstChildElement("Position");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf", &cam.pos.x, &cam.pos.y, &cam.pos.z);

        camElement = pCamera->FirstChildElement("Gaze");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf", &cam.gaze.x, &cam.gaze.y, &cam.gaze.z);

        camElement = pCamera->FirstChildElement("Up");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf", &cam.v.x, &cam.v.y, &cam.v.z);

        cam.gaze = normalizeVec3(cam.gaze);
        cam.u = crossProductVec3(cam.gaze, cam.v);
        cam.u = normalizeVec3(cam.u);

        cam.w = inverseVec3(cam.gaze);
        cam.v = crossProductVec3(cam.u, cam.gaze);
        cam.v = normalizeVec3(cam.v);

        camElement = pCamera->FirstChildElement("ImagePlane");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
               &cam.left, &cam.right, &cam.bottom, &cam.top,
               &cam.near, &cam.far, &cam.horRes, &cam.verRes);

        camElement = pCamera->FirstChildElement("OutputName");
        str = camElement->GetText();
        cam.outputFileName = string(str);

        cameras.push_back(cam);

        pCamera = pCamera->NextSiblingElement("Camera");
    }

    // read vertices
    pElement = pRoot->FirstChildElement("Vertices");
    XMLElement* pVertex = pElement->FirstChildElement("Vertex");
    int vertexId = 1;

    while (pVertex != nullptr) {
        Vec3 vertex = Vec3();
        Color color = Color();

        vertex.colorId = vertexId;

        str = pVertex->Attribute("position");
        sscanf(str, "%lf %lf %lf", &vertex.x, &vertex.y, &vertex.z);

        str = pVertex->Attribute("color");
        sscanf(str, "%lf %lf %lf", &color.r, &color.g, &color.b);

        vertices.push_back(vertex);
        colorsOfVertices.push_back(color);

        pVertex = pVertex->NextSiblingElement("Vertex");

        vertexId++;
    }

    // read translations
    pElement = pRoot->FirstChildElement("Translations");
    XMLElement* pTranslation = pElement->FirstChildElement("Translation");
    while (pTranslation != nullptr) {
        Translation translation = Translation();

        pTranslation->QueryIntAttribute("id", &translation.translationId);

        str = pTranslation->Attribute("value");
        sscanf(str, "%lf %lf %lf", &translation.tx, &translation.ty, &translation.tz);

        translations.push_back(translation);

        pTranslation = pTranslation->NextSiblingElement("Translation");
    }

    // read scalings
    pElement = pRoot->FirstChildElement("Scalings");
    XMLElement* pScaling = pElement->FirstChildElement("Scaling");
    while (pScaling != nullptr) {
        Scaling scaling = Scaling();

        pScaling->QueryIntAttribute("id", &scaling.scalingId);
        str = pScaling->Attribute("value");
        sscanf(str, "%lf %lf %lf", &scaling.sx, &scaling.sy, &scaling.sz);

        scalings.push_back(scaling);

        pScaling = pScaling->NextSiblingElement("Scaling");
    }

    // read rotations
    pElement = pRoot->FirstChildElement("Rotations");
    XMLElement* pRotation = pElement->FirstChildElement("Rotation");
    while (pRotation != nullptr) {
        Rotation rotation = Rotation();

        pRotation->QueryIntAttribute("id", &rotation.rotationId);
        str = pRotation->Attribute("value");
        sscanf(str, "%lf %lf %lf %lf", &rotation.angle, &rotation.ux, &rotation.uy, &rotation.uz);

        rotations.push_back(rotation);

        pRotation = pRotation->NextSiblingElement("Rotation");
    }

    // read meshes
    pElement = pRoot->FirstChildElement("Meshes");

    XMLElement* pMesh = pElement->FirstChildElement("Mesh");
    XMLElement* meshElement;
    while (pMesh != nullptr) {
        Mesh mesh = Mesh();

        pMesh->QueryIntAttribute("id", &mesh.meshId);

        // read projection type
        str = pMesh->Attribute("type");

        if (strcmp(str, "wireframe") == 0) {
            mesh.type = 0;
        } else {
            mesh.type = 1;
        }

        // read mesh transformations
        XMLElement* pTransformations = pMesh->FirstChildElement("Transformations");
        XMLElement* pTransformation = pTransformations->FirstChildElement("Transformation");

        while (pTransformation != nullptr) {
            char transformationType;
            int transformationId;

            str = pTransformation->GetText();
            sscanf(str, "%c %d", &transformationType, &transformationId);

            mesh.transformationTypes.push_back(transformationType);
            mesh.transformationIds.push_back(transformationId);

            pTransformation = pTransformation->NextSiblingElement("Transformation");
        }

        // read mesh faces
        char* row;
        char* clone_str;
        int v1, v2, v3;
        XMLElement* pFaces = pMesh->FirstChildElement("Faces");
        str = pFaces->GetText();
        clone_str = strdup(str);

        row = strtok(clone_str, "\n");
        while (row != nullptr) {
            int result = sscanf(row, "%d %d %d", &v1, &v2, &v3);

            if (result != EOF) {
                mesh.triangles.push_back(Triangle(v1, v2, v3));
            }
            row = strtok(nullptr, "\n");
        }
        meshes.push_back(mesh);

        pMesh = pMesh->NextSiblingElement("Mesh");
    }
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(const Camera& camera) {
    if (this->image.empty()) {
        for (int i = 0; i < camera.horRes; i++) {
            vector<Color> rowOfColors;

            for (int j = 0; j < camera.verRes; j++) {
                rowOfColors.push_back(this->backgroundColor);
            }

            this->image.push_back(rowOfColors);
        }
    } else {
        for (int i = 0; i < camera.horRes; i++) {
            for (int j = 0; j < camera.verRes; j++) {
                this->image[i][j].r = this->backgroundColor.r;
                this->image[i][j].g = this->backgroundColor.g;
                this->image[i][j].b = this->backgroundColor.b;
            }
        }
    }
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value) {
    if (value >= 255.0)
        return 255;
    if (value <= 0.0)
        return 0;
    return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(const Camera& camera) {
    cout << "Write PPM begin" << endl;

    ofstream fout;

    cout << "Output File Name: " << camera.outputFileName.c_str() << endl;

    fout.open(camera.outputFileName.c_str());

    fout << "P3" << endl;
    fout << "# " << camera.outputFileName << endl;
    fout << camera.horRes << " " << camera.verRes << endl;
    fout << "255" << endl;

    for (int j = camera.verRes - 1; j >= 0; j--) {
        for (int i = 0; i < camera.horRes; i++) {
            fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
                 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
                 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
        }
        fout << endl;
    }
    fout.close();
}

#pragma clang diagnostic pop
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

#include "../Render.h"
#include "../math/Math.h"

using namespace tinyxml2;
using namespace std;
using namespace Rasterizer;

/*
	Transformations, clipping, culling, rasterization are done here.
	You may define helper functions.
*/

void Scene::forwardRenderingPipeline(const Camera& camera)
{
    Render::DrawLine(image, int2(0, 0), int2(500, 500), double3(0, 0, 255), double3(255, 0, 0));

    auto meshCount = meshes.size();
    for (int i = 0; i < meshCount; ++i) {
        const auto& mesh = meshes[i];

        if(mesh.type == 0){
            // Wireframe

            auto triangleCount = mesh.triangles.size();
            for (int j = 0; j < triangleCount; ++j) {
                const auto& tri = mesh.triangles[j];
                auto v0 = vertices[tri.vertexIds[0] - 1];
                auto v1 = vertices[tri.vertexIds[1] - 1];
                auto v2 = vertices[tri.vertexIds[2] - 1];
                auto p0 = v0.GetPos();
                auto p1 = v1.GetPos();
                auto p2 = v2.GetPos();

                // Culling
                if(cullingEnabled && Render::ShouldTriangleBeCulled(p0, p1, p2)){
                    continue;
                }

                // TODO: Transform vertices to camera space

                // auto v0 = vertices[]

                // TODO: Fetch vertices
                // TODO: Apply transformations
                // TODO: Draw
                // TODO: Profit
            }
        }
        else if(mesh.type == 1){
            // TODO:
            cout << "Solid mesh not implemented yet." << endl;
        }
        else{
            cout << "Error, Unexpected Mesh Type: " << mesh.type << endl;
        }

    }

//    for(int x = 0; x < 300; x++){
//        Render::DrawColor(image, int2(x, 100), Color(255, 0, 0));
//    }
//
//    for(int y = 600; y > 300; y--){
//        Render::DrawColor(image, int2(500, y), Color(255, 255, 0));
//    }

    // TODO: Change pointers to non-pointer ones.

    cout << "Gaze: " << camera.gaze << endl;
    cout << "u: " << camera.u << endl;
    cout << "v: " << camera.v << endl;
    cout << "w: " << camera.w << endl;
    cout << "HorizontalResolution: " << camera.horRes << endl;
    cout << "VerticalResolution: " << camera.verRes << endl;
    cout << "Projection:" << camera.projectionType << endl;
    cout << "Far: " << camera.far << endl;
    cout << "Near: " << camera.near << endl;

    cout << "CullingEnabled: " << cullingEnabled << endl;
    cout << "VertexCount: " << vertices.size() << endl;
    cout << "MeshCount:" << meshes.size() << endl;

	// TODO: Implement this function.
    cout << "Forward Rendering Begin" << endl;

    cout << "Forward Rendering End" << endl;
}

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *pElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

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
		}
		else {
			cullingEnabled = false;
		}
	}

	// read cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while (pCamera != nullptr)
	{
		Camera cam = Camera();

		pCamera->QueryIntAttribute("id", &cam.cameraId);

		// read projection type
		str = pCamera->Attribute("type");

		if (strcmp(str, "orthographic") == 0) {
			cam.projectionType = 0;
		}
		else {
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
	XMLElement *pVertex = pElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (pVertex != nullptr)
	{
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
	XMLElement *pTranslation = pElement->FirstChildElement("Translation");
	while (pTranslation != nullptr)
	{
		Translation translation = Translation();

		pTranslation->QueryIntAttribute("id", &translation.translationId);

		str = pTranslation->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation.tx, &translation.ty, &translation.tz);

		translations.push_back(translation);

		pTranslation = pTranslation->NextSiblingElement("Translation");
	}

	// read scalings
	pElement = pRoot->FirstChildElement("Scalings");
	XMLElement *pScaling = pElement->FirstChildElement("Scaling");
	while (pScaling != nullptr)
	{
		Scaling scaling = Scaling();

		pScaling->QueryIntAttribute("id", &scaling.scalingId);
		str = pScaling->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling.sx, &scaling.sy, &scaling.sz);

		scalings.push_back(scaling);

		pScaling = pScaling->NextSiblingElement("Scaling");
	}

	// read rotations
	pElement = pRoot->FirstChildElement("Rotations");
	XMLElement *pRotation = pElement->FirstChildElement("Rotation");
	while (pRotation != nullptr)
	{
		Rotation rotation = Rotation();

		pRotation->QueryIntAttribute("id", &rotation.rotationId);
		str = pRotation->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation.angle, &rotation.ux, &rotation.uy, &rotation.uz);

		rotations.push_back(rotation);

		pRotation = pRotation->NextSiblingElement("Rotation");
	}

	// read meshes
	pElement = pRoot->FirstChildElement("Meshes");

	XMLElement *pMesh = pElement->FirstChildElement("Mesh");
	XMLElement *meshElement;
	while (pMesh != nullptr)
	{
		Mesh mesh = Mesh();

		pMesh->QueryIntAttribute("id", &mesh.meshId);

		// read projection type
		str = pMesh->Attribute("type");

		if (strcmp(str, "wireframe") == 0) {
			mesh.type = 0;
		}
		else {
			mesh.type = 1;
		}

		// read mesh transformations
		XMLElement *pTransformations = pMesh->FirstChildElement("Transformations");
		XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

		while (pTransformation != nullptr)
		{
			char transformationType;
			int transformationId;

			str = pTransformation->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			mesh.transformationTypes.push_back(transformationType);
			mesh.transformationIds.push_back(transformationId);

			pTransformation = pTransformation->NextSiblingElement("Transformation");
		}

		// read mesh faces
		char *row;
		char *clone_str;
		int v1, v2, v3;
		XMLElement *pFaces = pMesh->FirstChildElement("Faces");
        str = pFaces->GetText();
		clone_str = strdup(str);

		row = strtok(clone_str, "\n");
		while (row != nullptr)
		{
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
void Scene::initializeImage(const Camera& camera)
{
    cout << "Initialize Image" << endl;

	if (this->image.empty())
	{
		for (int i = 0; i < camera.horRes; i++)
		{
			vector<Color> rowOfColors;

			for (int j = 0; j < camera.verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
			}

			this->image.push_back(rowOfColors);
		}
	}
	else
	{
		for (int i = 0; i < camera.horRes; i++)
		{
			for (int j = 0; j < camera.verRes; j++)
			{
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
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(const Camera& camera)
{
    cout << "Write PPM begin" << endl;

	ofstream fout;

    cout << "Output File Name: " << camera.outputFileName.c_str() << endl;

	fout.open(camera.outputFileName.c_str());

	fout << "P3" << endl;
	fout << "# " << camera.outputFileName << endl;
	fout << camera.horRes << " " << camera.verRes << endl;
	fout << "255" << endl;

	for (int j = camera.verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera.horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

#pragma clang diagnostic pop
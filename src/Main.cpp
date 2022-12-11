#include <iostream>
#include <string>
#include <vector>
#include "code_template/Scene.h"
#include "code_template/Matrix4.h"
#include "code_template/Helpers.h"
#include "math/double4x4.h"
#include "math/Math.h"

using namespace std;

Scene *scene;

void TestTranslation(){
    auto a = Math::TranslationMatrix(double3(1, 2, 3));
    auto b = Math::TranslationMatrix(double3(30, 20, 10));

    a.print();
    b.print();

    auto position = double4(0, 0, 0, 1);

    cout << Math::Mul(Math::Mul(a, b), position).ToString() << endl;
}

void TestScale(){
    auto position = double4(1, 1.5, 0, 1);
    auto scale = double3(2, 3, 1);
    cout << Math::Mul(Math::ScaleMatrix(scale), position).ToString() << endl;
}

void TestPositionAndScale(){
    auto position = double4(10, 10, 10, 1);
    auto translate = double3(-15, 20, 0);
    auto scale = double3(1, 2, 3);

    auto result = Math::Mul(Math::TranslationMatrix(translate), Math::Mul(Math::ScaleMatrix(scale), position));

    cout << result.ToString() << endl;
}

void TestRotation(){
    auto position = double4(10, 10, 10, 1);
    auto result = Math::Mul(Math::RotationMatrix(double3(0, 0, 0)), position);
    cout << result.ToString() << endl;
}

void TestRotation_2(){
    auto position = double4(10, 10, 10, 1);
    auto result = Math::Mul(Math::RotationMatrix(double3(1, 1, 1) * 2 * 3.14), position);
    cout << result.ToString() << endl;
}

void TestRotation_3(){
    auto position = double4(1, 1, 1, 1);
    auto result = Math::Mul(Math::RotationMatrix(double3(1, 1, 1)), position);
    cout << result.ToString() << endl;
}

void TestTRS(){
    auto res = Math::LocalToWorld(double3(5, 5, 5), double3(1, 0, -1), double3(0, 0, 0), double3(1, 1, 1));
    cout << res.ToString() << endl;
}

int main(int argc, char *argv[])
{
    TestTRS();

    return 0;

    cout << "Start Running Rasterizer" << endl;

    if (argc != 2)
    {
        cout << "Please run the rasterizer as:" << endl
             << "\t./rasterizer <input_file_name>" << endl;
        return 1;
    }
    else
    {
        const char *xmlPath = argv[1];

        scene = new Scene(xmlPath);

        for (int i = 0; i < scene->cameras.size(); i++)
        {
            cout << "Running for Camera Id: " << i << endl;

            // initialize image with basic values
            scene->initializeImage(scene->cameras[i]);

            // do forward rendering pipeline operations
            scene->forwardRenderingPipeline(scene->cameras[i]);

            // generate PPM file
            scene->writeImageToPPMFile(scene->cameras[i]);
        }

        cout << "Stop Running Rasterizer" << endl;

        return 0;
    }
}
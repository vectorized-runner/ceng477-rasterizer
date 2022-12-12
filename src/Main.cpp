#include <iostream>
#include <vector>
#include "code_template/Scene.h"

using namespace std;

Scene* scene;

int main(int argc, char* argv[]) {
    cout << "Start Running Rasterizer" << endl;

    if (argc != 2) {
        cout << "Please run the rasterizer as:" << endl
             << "\t./rasterizer <input_file_name>" << endl;
        return 1;
    } else {
        const char* xmlPath = argv[1];

        scene = new Scene(xmlPath);

        for (int i = 0; i < scene->cameras.size(); i++) {
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
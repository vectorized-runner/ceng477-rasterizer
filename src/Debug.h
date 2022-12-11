
#ifndef CENG477_RAYTRACER_DEBUG_H
#define CENG477_RAYTRACER_DEBUG_H

#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

#ifndef Self_Debug
#define Self_Debug
#endif

namespace Rasterizer {

    struct Debug {
        static void LogElapsed(const string& message, steady_clock::time_point begin, steady_clock::time_point end) {
            cout << message << duration_cast<milliseconds>(end - begin).count() << " ms." << endl;
        }

        __attribute__((always_inline))
        static void Assert(bool condition, const string& message) {
#ifdef Self_Debug
            if (!condition) {
                Log("Assertion Failed: " + message);
            }
#endif
        }

        __attribute__((always_inline))
        static void Log(const string& message) {
#ifdef Self_Debug
            cout << message << endl;
#endif
        }

        __attribute__((always_inline))
        static void Log(const string& message, int i) {
#ifdef Self_Debug
            cout << message << i << endl;
#endif
        }

        __attribute__((always_inline))
        static void Log(const string& message, float f) {
#ifdef Self_Debug
            cout << message << f << endl;
#endif
        }

        __attribute__((always_inline))
        static void Log(const string& message, char* c) {
#ifdef Self_Debug
            cout << message << c << endl;
#endif
        }
    };

} // RayTracer

#endif //CENG477_RAYTRACER_DEBUG_H

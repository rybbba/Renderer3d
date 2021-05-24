#pragma once

#include "Eigen/Core"
#include "Screen.h"
#include "Primitive.h"
#include "Camera.h"
#include "Scene.h"

namespace Renderer3d {

class Renderer {
public:
    explicit Renderer(Screen &out);

    Screen &render(const Scene &scene);

    void clearScreen();

private:
    void draw_triangle(const Array3<Vector3f> &p, const Array3i &color);

    Screen &screen;

    Index width, height;
    ArrayXXd z_buf;
};

}  // namespace Renderer3d

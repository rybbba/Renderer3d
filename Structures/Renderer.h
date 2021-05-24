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
    void draw_triangle(const Eigen::Array3<Eigen::Vector3f> &p, const Eigen::Array3i &color);

    Screen &screen;

    Eigen::Index width, height;
    Eigen::ArrayXXd z_buf;
};

}  // namespace Renderer3d

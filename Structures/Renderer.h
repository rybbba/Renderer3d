#pragma once
#include "Eigen/Core"
#include "Screen.h"
#include "Primitive.h"
#include "Camera.h"
#include "Scene.h"

class Renderer {
public:
    explicit Renderer(Screen &out);
    Screen &render(const Camera &cam, const Scene &scene);
    void clear();

private:
    void draw_triangle(const Array3<Vector3f> &p, const Array3i &color);

    Screen &screen;

    Index width, height;
    ArrayXX<Array3i> image;
    ArrayXXd z_buf;
};
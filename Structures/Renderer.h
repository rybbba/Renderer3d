#pragma once
#include "Eigen/Core"
#include "Screen.h"
#include "Primitive.h"
#include "Camera.h"

class Renderer {
public:
    explicit Renderer(Screen &out);
    Screen &render(const Camera &cam, std::vector<const Primitive *> objects, std::vector<Properties> properties);
    void clear();

private:
    void draw_line(const Array2<Vector3d> &p, const Array3i &color);
    void draw_triangle(const Array3<Vector3d> &p, const Array3i &color);

    Screen &screen;

    Index width, height;
    ArrayXX<Array3i> image;
    ArrayXXd z_buf;
};
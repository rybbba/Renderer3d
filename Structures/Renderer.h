#pragma once
#include "Eigen/Core"
#include "Screen.h"
#include "Primitive.h"

class Renderer {
public:
    explicit Renderer(Screen &out);
    Screen &render(std::vector<const Primitive *> objects, std::vector<Position> positions);
    void clear();

private:
    void draw_line(const Array2<Vector3d> &p, const Array3i &color);
    void draw_triangle(const Array3<Vector3d> &points, const Array3i &color);

    Screen &screen;

    Index width, height;
    ArrayXX<Array3i> image;
    ArrayXXd z_buf;
};
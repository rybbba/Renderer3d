#pragma once
#include "Eigen/Core"

namespace Renderer3d {

struct Camera {
    float n, f; // near and far planes distance
    float l, r; // near plane left and right edges coordinates
    float b, t; // near plane bottom and top edges coordinates

    Vector3f position = {0, 0, 0};
    Vector3f angle = {0, 0, 0};
};

}  // namespace Renderer3d

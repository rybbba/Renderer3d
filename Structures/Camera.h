#pragma once

#include "Eigen/Core"
#include <Eigen/Geometry>

namespace Renderer3d {

struct Camera {
    float n, f; // near and far planes distance
    float l, r; // near plane left and right edges coordinates
    float b, t; // near plane bottom and top edges coordinates

    Vector3f position = {0, 0, 0};
    Vector3f angle = {0, 0, 0};

    Vector3f get_direction() {
        Vector3f direction{0, 0, -1};
        direction = AngleAxisf(angle.x(), Vector3f::UnitX()) * direction;
        direction = AngleAxisf(angle.y(), Vector3f::UnitY()) * direction;
        direction = AngleAxisf(angle.z(), Vector3f::UnitZ()) * direction;
        return direction;
    }
};

}  // namespace Renderer3d

#pragma once

#include "Eigen/Core"
#include <Eigen/Geometry>

namespace Renderer3d {

struct Camera {
    float n, f; // near and far planes distance
    float l, r; // near plane left and right edges coordinates
    float b, t; // near plane bottom and top edges coordinates

    Eigen::Vector3f position = {0, 0, 0};
    Eigen::Vector3f angle = {0, 0, 0};

    Eigen::Vector3f get_direction() {
        Eigen::Vector3f direction{0, 0, -1};
        direction = Eigen::AngleAxisf(angle.x(), Eigen::Vector3f::UnitX()) * direction;
        direction = Eigen::AngleAxisf(angle.y(), Eigen::Vector3f::UnitY()) * direction;
        direction = Eigen::AngleAxisf(angle.z(), Eigen::Vector3f::UnitZ()) * direction;
        return direction;
    }
};

}  // namespace Renderer3d

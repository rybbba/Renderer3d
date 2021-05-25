#include "Camera.h"

namespace Renderer3d {

Eigen::Vector3f Camera::get_direction() {
    Eigen::Vector3f direction{0, 0, -1};
    direction = Eigen::AngleAxisf(angle.x(), Eigen::Vector3f::UnitX()) * direction;
    direction = Eigen::AngleAxisf(angle.y(), Eigen::Vector3f::UnitY()) * direction;
    direction = Eigen::AngleAxisf(angle.z(), Eigen::Vector3f::UnitZ()) * direction;
    return direction;
}

}  // namespace Renderer3d

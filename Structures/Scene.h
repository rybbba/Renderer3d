#pragma once

#include "Primitive.h"
#include "Camera.h"

class Scene {
public:
    Scene() = default;

    void setScene(const std::vector<const Primitive *> &obj, const std::vector<Properties> &prop);
    void setCamera(const Camera &cam);

    [[nodiscard]] const std::vector<const Primitive *> & getObjects() const;
    [[nodiscard]] const std::vector<Properties> &getProperties() const;
    [[nodiscard]] const Camera &getCamera() const;

private:
    Camera camera;
    Properties camera_pos;
    std::vector<const Primitive *> objects;
    std::vector<Properties> properties;

};

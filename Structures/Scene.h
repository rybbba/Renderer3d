#pragma once

#include "Primitive.h"

class Scene {
public:
    Scene() = default;

    void setScene(const std::vector<const Primitive *> &obj, const std::vector<Properties> &prop);

    [[nodiscard]] const std::vector<const Primitive *> & getObjects() const;
    [[nodiscard]] const std::vector<Properties> &getProperties() const;

private:
    std::vector<const Primitive *> objects;
    std::vector<Properties> properties;

};

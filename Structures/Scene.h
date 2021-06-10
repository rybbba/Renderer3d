#pragma once

#include <list>

#include "Primitive.h"
#include "Camera.h"

namespace Renderer3d {

struct Entity {
    Properties properties;
    std::list<Triangle>::iterator begin_iterator;
    std::list<Triangle>::iterator end_iterator;

    [[nodiscard]] std::list<Triangle>::iterator begin() const {
        return begin_iterator;
    }
    [[nodiscard]] std::list<Triangle>::iterator end() const {
        return end_iterator;
    }
};

class Scene {
public:
    Scene() = default;

    void addObject(const Primitive *primitive, const Properties &properties);
    void addObject(const std::vector<Triangle> &object_new, const Properties &properties);

    void clear();

    void setCamera(const Camera &cam);

    [[nodiscard]] const std::vector<Entity> &getObjects() const;

    [[nodiscard]] const Camera &getCamera() const;

    [[nodiscard]] size_t countTriangles() const;

private:
    Camera camera;
    std::vector<Entity> objects;
    std::list<Triangle> triangles;
};

}  // namespace Renderer3d

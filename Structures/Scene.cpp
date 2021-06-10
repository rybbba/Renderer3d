#include "Scene.h"

namespace Renderer3d {

void Scene::addObject(const Primitive *primitive, const Properties &properties) {
    objects.push_back({});
    auto &obj = objects.back();

    obj.properties = properties;

    obj.end_iterator = triangles.begin();
    for (auto &triangle : primitive->simplify()) {
        triangles.push_front(triangle);
    }
    obj.begin_iterator = triangles.begin();
}

void Scene::addObject(const std::vector<Triangle> &object_new, const Properties &properties) {
    objects.push_back({});
    auto &obj = objects.back();

    obj.properties = properties;

    obj.end_iterator = triangles.begin();
    for (auto &triangle : object_new) {
        triangles.push_front(triangle);
    }
    obj.begin_iterator = triangles.begin();
}

void Scene::clear() {
    objects.clear();
    triangles.clear();
}

void Scene::setCamera(const Camera &cam) {
    camera = cam;
}

const std::vector<Entity> &Scene::getObjects() const {
    return objects;
}

const Camera &Scene::getCamera() const {
    return camera;
}

size_t Scene::countTriangles() const {
    return triangles.size();
}

}  // namespace Renderer3d

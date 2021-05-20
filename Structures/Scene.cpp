#include "Scene.h"

namespace Renderer3d {

void Scene::setScene(const std::vector<const Primitive *> &obj, const std::vector<Properties> &prop) {
        objects = obj;
        properties = prop;
}

void Scene::setCamera(const Camera &cam) {
    camera = cam;
}

const std::vector<const Primitive *> & Scene::getObjects() const {
    return objects;
}

const std::vector<Properties> &Scene::getProperties() const  {
    return properties;
}

const Camera &Scene::getCamera() const {
    return camera;
}

}  // namespace Renderer3d

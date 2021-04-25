#include "Scene.h"

void Scene::setScene(const std::vector<const Primitive *> &obj, const std::vector<Properties> &prop) {
        objects = obj;
        properties = prop;
}

const std::vector<const Primitive *> & Scene::getObjects() const {
    return objects;
}

const std::vector<Properties> &Scene::getProperties() const  {
    return properties;
}

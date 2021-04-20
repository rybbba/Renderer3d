#include "Primitive.h"


Triangle::Triangle(const Vector4d &p1, const Vector4d &p2, const Vector4d &p3){
    points.col(0) = p1;
    points.col(1) = p2;
    points.col(2) = p3;
}

std::vector<Triangle> Triangle::simplify() const {
    return {*this};
}

void Triangle::translate(const Vector3d &pos) {
    for (auto col : points.colwise()) {
        col.head(3) += pos;
    }
}

void Triangle::rotateX(double angle) {
    points.topLeftCorner(3,3) = AngleAxisd(angle, Vector3d::UnitX()) * points.topLeftCorner(3,3);
}
void Triangle::rotateY(double angle) {
    points.topLeftCorner(3,3) = AngleAxisd(angle, Vector3d::UnitY()) * points.topLeftCorner(3,3);
}
void Triangle::rotateZ(double angle) {
    points.topLeftCorner(3,3) = AngleAxisd(angle, Vector3d::UnitZ()) * points.topLeftCorner(3,3);
}

void Triangle::rotate(Vector3d angle) {
    rotateX(angle.x());
    rotateY(angle.y());
    rotateZ(angle.z());
}

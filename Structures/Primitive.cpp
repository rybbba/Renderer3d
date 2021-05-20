#include "Primitive.h"

namespace Renderer3d {

Triangle::Triangle(const Vector4f &p1, const Vector4f &p2, const Vector4f &p3) {
    points.col(0) = p1;
    points.col(1) = p2;
    points.col(2) = p3;
}

std::vector<Triangle> Triangle::simplify() const {
    return {*this};
}

void Triangle::translate(const Vector3f &pos) {
    for (auto col : points.colwise()) {
        col.head(3) += pos;
    }
}

void Triangle::rotateX(float angle) {
    points.topLeftCorner(3, 3) = AngleAxisf(angle, Vector3f::UnitX()) * points.topLeftCorner(3, 3);
}

void Triangle::rotateY(float angle) {
    points.topLeftCorner(3, 3) = AngleAxisf(angle, Vector3f::UnitY()) * points.topLeftCorner(3, 3);
}

void Triangle::rotateZ(float angle) {
    points.topLeftCorner(3, 3) = AngleAxisf(angle, Vector3f::UnitZ()) * points.topLeftCorner(3, 3);
}

void Triangle::rotate(const Vector3f &angle) {
    rotateX(angle.x());
    rotateY(angle.y());
    rotateZ(angle.z());
}

void Triangle::transform(const Matrix4f &m) {
    points = m * points;
}

void Triangle::scale(const Vector3f &coefficients) {
    DiagonalMatrix<float, 4> m(coefficients.x(), coefficients.y(), coefficients.z(), 1);
    points = m * points;
}

}  // namespace Renderer3d

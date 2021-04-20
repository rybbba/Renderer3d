#pragma once
#include "Eigen/Core"
#include <Eigen/Geometry>

using namespace Eigen;

struct Position {
    Vector3d coordinates = {0, 0, 0};
    Vector3d angle = {0, 0, 0};
    Vector3d scale = {1, 1, 1};
};

struct Triangle;

struct Primitive {
    virtual void translate(const Vector3d &pos) = 0;
    virtual void rotateX(double angle) = 0;
    virtual void rotateY(double angle) = 0;
    virtual void rotateZ(double angle) = 0;

    [[nodiscard]] virtual std::vector<Triangle> simplify() const = 0;
};


struct Triangle : Primitive {
    Triangle(const Vector4d &p1, const Vector4d &p2, const Vector4d &p3);

    void translate(const Vector3d &pos) override;
    void rotateX(double angle) override;
    void rotateY(double angle) override;
    void rotateZ(double angle) override;

    [[nodiscard]] std::vector<Triangle> simplify() const override;

    Matrix<double, 4, 3> points;
};


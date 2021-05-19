#pragma once
#include "Eigen/Core"
#include "Eigen/Geometry"

using namespace Eigen;

struct Properties {
    Vector3f coordinates = {0, 0, 0};
    Vector3f angle = {0, 0, 0};
    Vector3f scale = {1, 1, 1};
    Array3i color = {255, 255, 255};
};

struct Triangle;

struct Primitive {
    virtual void translate(const Vector3f &pos) = 0;

    virtual void rotateX(float angle) = 0;
    virtual void rotateY(float angle) = 0;
    virtual void rotateZ(float angle) = 0;
    virtual void rotate(const Vector3f &angle) = 0;

    virtual void transform(const Matrix4f &m) = 0;

    virtual void scale(const Vector3f &coefficients) = 0;

    [[nodiscard]] virtual std::vector<Triangle> simplify() const = 0;
};


struct Triangle : Primitive {
    Triangle(const Vector4f &p1, const Vector4f &p2, const Vector4f &p3);

    void translate(const Vector3f &pos) override;
    void rotateX(float angle) override;
    void rotateY(float angle) override;
    void rotateZ(float angle) override;

    // Rotates triangle around X, Y and then Z axes.
    void rotate(const Vector3f &angle) override;

    void transform(const Matrix4f &m) override;

    void scale(const Vector3f &coefficients) override;

    [[nodiscard]] std::vector<Triangle> simplify() const override;

    Matrix<float, 4, 3> points;
};


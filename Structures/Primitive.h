#pragma once

#include "Eigen/Core"
#include "Eigen/Geometry"


namespace Renderer3d {

struct Properties {
    Eigen::Vector3f coordinates = {0, 0, 0};
    Eigen::Vector3f angle = {0, 0, 0};
    Eigen::Vector3f scale = {1, 1, 1};
    Eigen::Array3i color = {255, 255, 255};
};

struct Triangle;

struct Primitive {
    [[nodiscard]] virtual std::vector<Triangle> simplify() const = 0;

    virtual void translate(const Eigen::Vector3f &pos) = 0;

    virtual void rotateX(float angle) = 0;

    virtual void rotateY(float angle) = 0;

    virtual void rotateZ(float angle) = 0;

    virtual void rotate(const Eigen::Vector3f &angle) = 0;

    virtual void transform(const Eigen::Matrix4f &m) = 0;

    virtual void scale(const Eigen::Vector3f &coefficients) = 0;
};


struct Triangle : Primitive {
    Triangle(const Eigen::Vector4f &p1, const Eigen::Vector4f &p2, const Eigen::Vector4f &p3);

    [[nodiscard]] std::vector<Triangle> simplify() const override;

    void translate(const Eigen::Vector3f &pos) override;

    void rotateX(float angle) override;

    void rotateY(float angle) override;

    void rotateZ(float angle) override;

    // Rotates triangle around X, Y and then Z axes.
    void rotate(const Eigen::Vector3f &angle) override;

    void transform(const Eigen::Matrix4f &m) override;

    void scale(const Eigen::Vector3f &coefficients) override;

    Eigen::Matrix<float, 4, 3> points;
};

}  // namespace Renderer3d

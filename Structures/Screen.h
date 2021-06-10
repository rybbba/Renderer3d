#pragma once

#include "Eigen/Core"


namespace Renderer3d {

class Screen {
public:
    Screen(Eigen::Index w, Eigen::Index h);

    Eigen::Array3i &operator()(Eigen::Index i, Eigen::Index j);

    [[nodiscard]] Eigen::Index get_w() const;

    [[nodiscard]] Eigen::Index get_h() const;

    [[nodiscard]] Eigen::Array2<Eigen::Index> shape() const;

    void clear();
    void setImage(const Eigen::ArrayXX<Eigen::Array3i> &image);

private:
    Eigen::Index width, height;
    Eigen::Matrix<Eigen::Array3i, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> canvas;
};

}  // namespace Renderer3d

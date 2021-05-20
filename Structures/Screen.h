#pragma once

#include "Eigen/Core"

using namespace Eigen;

namespace Renderer3d {

class Screen {
public:
    Screen(Index w, Index h);

    Array3i &operator()(Index i, Index j);

    [[nodiscard]] Index get_w() const;

    [[nodiscard]] Index get_h() const;

    [[nodiscard]] Array2<Index> shape() const;

    void setImage(const ArrayXX<Array3i> &image);

private:
    Index width, height;
    Matrix<Array3i, Dynamic, Dynamic, RowMajor> canvas;
};

}  // namespace Renderer3d

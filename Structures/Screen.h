#pragma once
#include "Eigen/Core"

using namespace Eigen;

class Screen {
public:
    Screen(Index w, Index h);
    Array3i &operator()(Index i, Index j);

    [[nodiscard]] Index get_w() const;
    [[nodiscard]] Index get_h() const;

    void set_image(const ArrayXX<Array3i>& image);
private:
    Index width, height;
    Matrix<Array3i, Dynamic, Dynamic, RowMajor> canvas;
};


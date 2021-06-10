#include "Screen.h"

namespace Renderer3d {

Screen::Screen(Eigen::Index w, Eigen::Index h) : width(w), height(h) {
    canvas.resize(w, h);
}

Eigen::Array3i &Screen::operator()(Eigen::Index i, Eigen::Index j) {
    return canvas(i, j);
}

Eigen::Index Screen::get_w() const {
    return width;
}

Eigen::Index Screen::get_h() const {
    return height;
}

Eigen::Array2<Eigen::Index> Screen::shape() const {
    return {width, height};
}

void Screen::clear() {
    canvas.setConstant(Eigen::Array3i::Zero());
}

void Screen::setImage(const Eigen::ArrayXX<Eigen::Array3i> &image) {
    canvas = image;
}

}  // namespace Renderer3d

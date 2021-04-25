#include "Screen.h"

Screen::Screen(Index w, Index h) : width(w), height(h) {
    canvas.resize(w, h);
}

Array3i &Screen::operator()(Index i, Index j) {
    return canvas(i, j);
}

Index Screen::get_w() const {
    return width;
}
Index Screen::get_h() const {
    return height;
}

Array2<Index> Screen::shape() const {
    return {width, height};
}

void Screen::setImage(const ArrayXX<Array3i>& image) {
    canvas = image;
}
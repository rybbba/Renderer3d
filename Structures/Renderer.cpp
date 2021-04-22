#include "Renderer.h"

void Renderer::clear() {
    image.setConstant(Array3i::Zero());
    z_buf.setConstant(2);
}

Renderer::Renderer(Screen &out) : screen(out) {
    width = screen.get_w();
    height = screen.get_h();
    image.resize(width, height);
    z_buf.resize(width, height);
    clear();

}

void Renderer::draw_line(const Array2<Vector3d> &p, const Array3i &color) {

    Array2<Vector2i> dp; // line ends coordinates on screen
    Array2d z {p[0].z(), p[1].z()};

    for (int i = 0; i < 2; ++i) {
        dp[i] = p[i].head(2).cast<int>();
    }

    Array2i diff = (dp[0] - dp[1]).array().abs();

    Array2<Index> shape {width, height};
    Index iter_axis, sec_axis;

    if (diff.x() > diff.y()) {
        iter_axis = 0;
        sec_axis = 1;
    } else {
        iter_axis = 1;
        sec_axis = 0;
    }
    if (dp[1][iter_axis] < dp[0][iter_axis]) {
        std::swap(dp[0], dp[1]);
        std::swap(z[0], z[1]);
    }

    for (int it = std::max(0, dp[0][iter_axis]); it <= std::min((int)width - 1, dp[1][iter_axis]); ++it) {
        int sec = (int)(dp[0][sec_axis] * (dp[1][iter_axis] - it) / (double)diff[iter_axis] + dp[1][sec_axis] * (it - dp[0][iter_axis]) / (double)diff[iter_axis]);
        double zp = (z[0] * (dp[1][iter_axis] - it) / (double)diff[iter_axis] + z[1] * (it - dp[0][iter_axis]) / (double)diff[iter_axis]);

        Array2i res;
        res[iter_axis] = it;
        res[sec_axis] = sec;
        if (res[sec_axis] >= 0 && res[sec_axis] < shape[sec_axis] && (zp > -1 && zp < 1) && zp < z_buf(res.x(), res.y())) {
            image(res.x(), res.y()) = color;
            z_buf(res.x(), res.y()) = zp;
        }
    }
}

void Renderer::draw_triangle(const Array3<Vector3d> &points, const Array3i &color) {
    draw_line({points[0], points[1]}, color);
    draw_line({points[1], points[2]}, color);
    draw_line({points[2], points[0]}, color);
}


Screen &Renderer::render(std::vector<const Primitive *> objects, std::vector<Position> positions) {
    double n = 0.001, f = 100, l = -0.001, r = 0.001, b = -0.001, t = 0.001;
    Matrix<double, 4, 4> proj {
            {2*n/(r-l), 0, (r+l)/(r-l), 0},
            {0, 2*n/(t-b), (t+b)/(t-b), 0},
            {0, 0, -(f+n)/(f-n), -2*n*f/(f-n)},
            {0, 0, -1, 0}
    };

    for (int ind = 0; ind < objects.size(); ++ind) {
        std::vector<Triangle> triangles = objects[ind]->simplify();
        for (auto &triangle : triangles) {
            triangle.scale(positions[ind].scale);
            triangle.rotate(positions[ind].angle);
            triangle.translate(positions[ind].coordinates);

            triangle.points = proj * triangle.points;
            for (int i = 0; i < 3; ++i) {
                triangle.points.col(i).head(3) /= triangle.points(3, i);
            }

            triangle.translate(Vector3d{1, 1, 0});
            triangle.scale(Vector3d{((double)width)/2, ((double)height)/2, 1});

            Matrix<double, 4, 3> &global = triangle.points;

            draw_triangle(
                    { global.col(0).head(3)
                            , global.col(1).head(3)
                            , global.col(2).head(3)}
                    , {255, 255, 255});
        }
    }

    screen.set_image(image);

    return screen;
}
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

    Array2<Vector2i> dp;
    Array2d z {p[0].z(), p[1].z()};

    for (int i = 0; i < 2; ++i) {
        dp[i] = ((p[i].head(2) - Vector2d{-1, -1}).array() * Array2d{width, height} / 2).cast<int>();
    }

    Array2i diff = (dp[0] - dp[1]).array().abs();
    int dx = diff.x();
    int dy = diff.y();

    if (diff.x() > diff.y()) {
        if (dp[1].x() < dp[0].x()) {
            std::swap(dp[0], dp[1]);
            std::swap(z[0], z[1]);
        }
        for (int x = std::max(0, dp[0].x()); x <= std::min((int)width - 1, dp[1].x()); ++x) {
            int y = (int)(dp[0].y() * (dp[1].x() - x)/(double)dx + dp[1].y() * (x - dp[0].x())/(double)dx);
            double zp = (z[0] * (dp[1].x() - x)/(double)dx + z[1] * (x - dp[0].x())/(double)dx);
            if (y >= 0 && y < height && (zp > -1 && zp < 1) && zp < z_buf(x, y)) {
                image(x, y) = color;
                z_buf(x, y) = zp;
            }
        }
    } else {
        if (dp[1].y() < dp[0].y()) {
            std::swap(dp[0], dp[1]);
            std::swap(z[0], z[1]);
        }
        for (int y = std::max(0, dp[0].y()); y <= std::min((int)height - 1, dp[1].y()); ++y) {
            int x = (int)(dp[0].x() * (dp[1].y() - y)/(double)dy + dp[1].x() * (y - dp[0].y())/(double)dy);
            double zp = (z[0] * (dp[1].y() - y)/(double)dy + z[1] * (y - dp[0].y())/(double)dy);
            if (x >= 0 && x < width && (zp > -1 && zp < 1) && zp < z_buf(x, y)) {
                image(x, y) = color;
                z_buf(x, y) = zp;
            }
        }
    }
}

void Renderer::draw_triangle(const Array3<Vector3d> &points, const Array3i &color) {
    draw_line({points[0], points[1]}, color);
    draw_line({points[1], points[2]}, color);
    draw_line({points[2], points[0]}, color);
}


Screen &Renderer::render(std::vector<const Primitive *> objects, std::vector<Vector3d> positions) {
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
            triangle.translate(positions[ind]);
            Matrix<double, 4, 3> global = triangle.points;

            global = proj * global;

            draw_triangle(
                    { global.col(0).head(3) / global(3, 0)
                            , global.col(1).head(3) / global(3, 1)
                            , global.col(2).head(3) / global(3, 2)}
                    , {255, 255, 255});
        }
    }

    screen.set_image(image);

    return screen;
}
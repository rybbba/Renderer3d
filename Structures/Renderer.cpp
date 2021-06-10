#include "Renderer.h"

namespace Renderer3d {

Renderer::Renderer(Screen &out) : screen(out) {
    width = screen.get_w();
    height = screen.get_h();
    z_buf.resize(width, height);
}

Eigen::Vector4f inter_z(Eigen::Vector4f now, Eigen::Vector4f next, float z) {
    if ((now.z() < z && next.z() > z) || (now.z() > z && next.z() < z)) {
        Eigen::Vector4f v = next - now;
        v = now + v * (z - now.z()) / v.z();
        return v;
    }
    return {0, 0, 0, 0};
}

std::vector<Triangle> clip_z(const Triangle &triangle, float near_z, float far_z) {

    std::vector<Eigen::Vector4f> points;

    for (int i = 0; i < 3; ++i) {
        Eigen::Vector4f now = triangle.points.col(i);
        Eigen::Vector4f next = triangle.points.col((i + 1) % 3);

        if (near_z > now.z() && now.z() > far_z) {
            points.emplace_back(now);
        }

        float fz, sz;
        if (abs(near_z - now.z()) < abs(far_z - now.z())) {
            fz = near_z;
            sz = far_z;
        } else {
            fz = far_z;
            sz = near_z;
        }

        auto v = std::move(inter_z(now, next, fz));
        if (v.w() == 1) {
            points.emplace_back(std::move(v));
        }
        v = std::move(inter_z(now, next, sz));
        if (v.w() == 1) {
            points.emplace_back(std::move(v));
        }
    }
    size_t p_size = points.size();

    std::vector<Triangle> ans;
    for (size_t i = 1; i + 1 < p_size; i += 1) {
        ans.emplace_back(points[0], points[i], points[i + 1]);
    }

    return ans;
}

Screen &Renderer::render(const Scene &scene) {
    z_buf.setConstant(2);
    const auto &objects = scene.getObjects();
    const auto &cam = scene.getCamera();
    Eigen::Matrix<float, 4, 4> proj{
            {2 * cam.n / (cam.r - cam.l), 0,                           (cam.r + cam.l) / (cam.r - cam.l),  0},
            {0,                           2 * cam.n / (cam.t - cam.b), (cam.t + cam.b) / (cam.t - cam.b),  0},
            {0,                           0,                           -(cam.f + cam.n) / (cam.f - cam.n), -2 * cam.n * cam.f / (cam.f - cam.n)},
            {0,                           0,                           -1,                                 0}
    };

    for (auto &object : objects) {
        for (auto triangle : object) {
            triangle.scale(object.properties.scale);
            triangle.rotate(object.properties.angle);
            triangle.translate(object.properties.coordinates);

            triangle.translate(-cam.position);
            triangle.rotateZ(-cam.angle.z());
            triangle.rotateY(-cam.angle.y());
            triangle.rotateX(-cam.angle.x());

            for (auto &sub_triangle : clip_z(triangle, -cam.n, -cam.f)) {
                sub_triangle.points = proj * sub_triangle.points;
                for (int i = 0; i < 3; ++i) {
                    sub_triangle.points.col(i).head(3) /= sub_triangle.points(3, i);
                }

                sub_triangle.translate(Eigen::Vector3f{1, 1, 0});
                sub_triangle.scale(Eigen::Vector3f{((float) width) / 2, ((float) height) / 2, 1});

                Eigen::Matrix<float, 4, 3> &global = sub_triangle.points;

                draw_triangle(
                        {global.col(0).head(3),
                         global.col(1).head(3),
                         global.col(2).head(3)},
                        object.properties.color);
            }
        }
    }
    return screen;
}

void Renderer::clearScreen() {
    screen.clear();
}

float cross2(const Eigen::Vector2f &a, const Eigen::Vector2f &b) {
    return a.x() * b.y() - a.y() * b.x();
}

// Returns y-coordinate of point on segment by x-coordinate of it and {0, false} if point can't be on segment
std::pair<float, bool> get_segment_point_y(float x, const Eigen::Vector2f &p1, const Eigen::Vector2f &p2){
    if ((x <= p1.x() && x <= p2.x()) || (x >= p1.x() && x >= p2.x())) {
        return {-1, false};
    }

    float y_res = (p2.x() - x) * p1.y() + (x - p1.x()) * p2.y();
    y_res /= p2.x() - p1.x();

    return  {y_res, true};
};

std::vector<float> get_triangle_points_x(float x, const Eigen::Vector2f &p1, const Eigen::Vector2f &p2, const Eigen::Vector2f &p3) {
    std::vector<float> res;
    {
        auto [y, between] = get_segment_point_y(x, p1, p2);
        if (between) {
            res.push_back(y);
        }
    }
    {
        auto [y, between] = get_segment_point_y(x, p1, p3);
        if (between) {
            res.push_back(y);
        }
    }
    {
        auto [y, between] = get_segment_point_y(x, p2, p3);
        if (between) {
            res.push_back(y);
        }
    }
    return res;
}

void Renderer::draw_triangle(const Eigen::Array3<Eigen::Vector3f> &p, const Eigen::Array3i &color) {
    Eigen::Array3<Eigen::Vector2f> flat_p;
    Eigen::Array3f z;
    for (int i = 0; i < 3; ++i) {
        flat_p[i] = p[i].head(2);
        z[i] = p[i].z();
    }

    float abc2 = cross2(flat_p[1] - flat_p[0], flat_p[2] - flat_p[0]);
    if (abc2 <= 0) return;

    for (int x = 0; x < width; ++x) {
        float x_real = 0.5f + (float) x;
        auto ys = get_triangle_points_x(x_real, flat_p[0], flat_p[1], flat_p[2]);
        if (ys.empty()) {
            continue;
        }

        std::sort(ys.begin(), ys.end());
        for (int y = std::max(0, (int)ys.front()); y <= std::min((int)ys.back(), (int)height - 1); ++y) {
            Eigen::Vector2f coords = {(float) x + 0.5f, (float) y + 0.5f};

            float pbc2 = cross2(flat_p[1] - coords, flat_p[2] - coords);
            float pca2 = cross2(flat_p[2] - coords, flat_p[0] - coords);
            Eigen::Array3f bari;
            bari[0] = pbc2 / abc2;
            bari[1] = pca2 / abc2;
            bari[2] = 1 - bari[0] - bari[1];

            float zp = 0;
            bool bad = false;
            for (int i = 0; i < 3; ++i) {
                if (bari[i] < 0) {
                    bad = true;
                }
                zp += z[i] * bari[i];
            }
            if (zp <= 1 && zp >= -1 && !bad && zp < z_buf(x, y)) {
                screen(x, y) = color;
                z_buf(x, y) = zp;
            }
        }
    }
}

}  // namespace Renderer3d

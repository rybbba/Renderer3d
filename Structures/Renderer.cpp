#include "Renderer.h"

namespace Renderer3d {

void Renderer::clearScreen() {
    screen.clear();
}

Renderer::Renderer(Screen &out) : screen(out) {
    width = screen.get_w();
    height = screen.get_h();
    z_buf.resize(width, height);
}

float cross2(const Eigen::Vector2f &a, const Eigen::Vector2f &b) {
    return a.x() * b.y() - a.y() * b.x();
}

void Renderer::draw_triangle(const Eigen::Array3<Eigen::Vector3f> &p, const Eigen::Array3i &color) {
    Eigen::Array3<Eigen::Vector2f> flat_p;
    Eigen::Array3f z;
    for (int i = 0; i < 3; ++i) {
        flat_p[i] = p[i].head(2);
        z[i] = p[i].z();
    }

    float abc2 = cross2(flat_p[1] - flat_p[0], flat_p[2] - flat_p[0]);
    if (abc2 < 0) return;

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            Eigen::Vector2f coords = {(float) x + 0.5, (float) y + 0.5};

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
    const auto &properties = scene.getProperties();
    const auto &cam = scene.getCamera();
    Eigen::Matrix<float, 4, 4> proj{
            {2 * cam.n / (cam.r - cam.l), 0,                           (cam.r + cam.l) / (cam.r - cam.l),  0},
            {0,                           2 * cam.n / (cam.t - cam.b), (cam.t + cam.b) / (cam.t - cam.b),  0},
            {0,                           0,                           -(cam.f + cam.n) / (cam.f - cam.n), -2 * cam.n * cam.f / (cam.f - cam.n)},
            {0,                           0,                           -1,                                 0}
    };

    for (int ind = 0; ind < objects.size(); ++ind) {
        std::vector<Triangle> triangles = objects[ind]->simplify();
        for (auto &triangle : triangles) {
            triangle.scale(properties[ind].scale);
            triangle.rotate(properties[ind].angle);
            triangle.translate(properties[ind].coordinates);

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
                        properties[ind].color);
            }
        }
    }
    return screen;
}

}  // namespace Renderer3d

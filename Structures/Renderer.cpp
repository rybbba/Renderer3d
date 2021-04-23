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

float cross2(const Vector2f &a, const Vector2f &b) {
    return a.x()*b.y() - a.y()*b.x();
}

void Renderer::draw_triangle(const Array3<Vector3f> &p, const Array3i &color) {
    Array3<Vector2f> flat_p;
    Array3f z;
    for (int i = 0; i < 3; ++i) {
        flat_p[i] = p[i].head(2);
        z[i] = p[i].z();
    }

    float abc2 = cross2(flat_p[1]-flat_p[0], flat_p[2]-flat_p[0]);
    if (abc2 < 0) return;

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            Vector2f coords = {(float)x + 0.5, (float)y + 0.5};

            float pbc2 = cross2(flat_p[1]-coords,flat_p[2]-coords);
            float pca2 = cross2(flat_p[2]-coords,flat_p[0]-coords);
            Array3d bari;
            bari[0] = pbc2 / abc2;
            bari[1] = pca2/ abc2;
            bari[2] = 1 - bari[0] - bari[1];

            float zp = 0;
            bool bad = false;
            for (int i = 0; i < 3; ++i) {
                if (bari[i] < 0) {
                    bad = true;
                }
                zp += z[i] * bari[i];
            }
            if (!bad && zp < z_buf(x, y)) {
                image(x, y) = color;
                z_buf(x, y) = zp;
            }
        }
        }
}


Screen &Renderer::render(const Camera &cam, std::vector<const Primitive *> objects, std::vector<Properties> properties) {
    Matrix<float, 4, 4> proj {
            {2*cam.n/(cam.r-cam.l), 0, (cam.r+cam.l)/(cam.r-cam.l), 0},
            {0, 2*cam.n/(cam.t-cam.b), (cam.t+cam.b)/(cam.t-cam.b), 0},
            {0, 0, -(cam.f+cam.n)/(cam.f-cam.n), -2*cam.n*cam.f/(cam.f-cam.n)},
            {0, 0, -1, 0}
    };

    for (int ind = 0; ind < objects.size(); ++ind) {
        std::vector<Triangle> triangles = objects[ind]->simplify();
        for (auto &triangle : triangles) {
            triangle.scale(properties[ind].scale);
            triangle.rotate(properties[ind].angle);
            triangle.translate(properties[ind].coordinates);

            triangle.points = proj * triangle.points;
            for (int i = 0; i < 3; ++i) {
                triangle.points.col(i).head(3) /= triangle.points(3, i);
            }

            triangle.translate(Vector3f{1, 1, 0});
            triangle.scale(Vector3f{((float)width)/2, ((float)height)/2, 1});

            Matrix<float, 4, 3> &global = triangle.points;

            draw_triangle(
                    { global.col(0).head(3)
                            , global.col(1).head(3)
                            , global.col(2).head(3)}
                    , properties[ind].color);
        }
    }

    screen.set_image(image);

    return screen;
}
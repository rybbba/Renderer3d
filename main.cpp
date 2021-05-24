#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Screen.h"
#include "Camera.h"
#include "Scene.h"
#include "Renderer.h"

using namespace std;
using namespace Renderer3d;

int main() {
    Triangle a({1, 0, 0, 1}, {0, 1, 0, 1}, {0, 0, 1, 1});

    const size_t W = 1000;
    const size_t H = 1000;

    Screen res(W, H);

    Scene scene;

    Camera camera{0.001, 100, -0.001, 0.001, -0.001, 0.001, {0, 0, 3}, {0, 0, 0}};

    Renderer renderer(res);


    sf::RenderWindow window(sf::VideoMode(W, H), "3d render");

    sf::Texture texture;
    texture.create(W, H);
    sf::Sprite sprite;

    // sf::Clock timer;
    sf::Clock fps_meter;

    float step = 0.03;
    float x_angle = 0, y_angle = 0;

    // float r_angle = 0;
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            camera.position.x() -= step;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            camera.position.x() += step;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            camera.position.y() += step;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            camera.position.y() -= step;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            camera.position += camera.get_direction() * step;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            camera.position -= camera.get_direction() * step;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) {
            camera.angle.y() = fmodf(camera.angle.y() + step, 2 * M_PI);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)) {
            camera.angle.y() = fmodf(camera.angle.y() - step, 2 * M_PI);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8)) {
            camera.angle.x() = fmodf(camera.angle.x() + step, 2 * M_PI);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) {
            camera.angle.x() = fmodf(camera.angle.x() - step, 2 * M_PI);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            y_angle = fmodf(y_angle - step, 2 * M_PI);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            y_angle = fmodf(y_angle + step, 2 * M_PI);
        }

        renderer.clearScreen();

        // r_angle = fmodf(r_angle + 1.5*timer.getElapsedTime().asSeconds(), 2*M_PI);
        // timer.restart();

        scene.setScene({&a, &a, &a, &a, &a, &a, &a, &a}, {
                {{0, -0.5, 0}, {0,    y_angle,            0}, {1, 1, 1}, {255, 0,   0}},
                {{0, -0.5, 0}, {0,    y_angle + M_PI / 2, 0}, {1, 1, 1}, {0,   255, 0}},
                {{0, -0.5, 0}, {0,    y_angle + M_PI,     0}, {1, 1, 1}, {0,   0,   255}},
                {{0, -0.5, 0}, {0,    y_angle - M_PI / 2, 0}, {1, 1, 1}, {255, 255, 255}},
                {{0, -0.5, 0}, {M_PI, y_angle,            0}, {1, 1, 1}, {255, 0,   0}},
                {{0, -0.5, 0}, {M_PI, y_angle + M_PI / 2, 0}, {1, 1, 1}, {0,   255, 0}},
                {{0, -0.5, 0}, {M_PI, y_angle + M_PI,     0}, {1, 1, 1}, {0,   0,   255}},
                {{0, -0.5, 0}, {M_PI, y_angle - M_PI / 2, 0}, {1, 1, 1}, {255, 255, 255}}
        });

        // camera.position.z() = 3 + r_angle;
        scene.setCamera(camera);

        renderer.render(scene);

        sf::Uint8 pixels[4 * W * H];

        for (int i = H - 1; i >= 0; --i) {
            for (int j = 0; j < W; ++j) {
                auto pix = res(j, i);
                pixels[4 * ((H - 1 - i) * W + j)] = pix[0];
                pixels[4 * ((H - 1 - i) * W + j) + 1] = pix[1];
                pixels[4 * ((H - 1 - i) * W + j) + 2] = pix[2];
                pixels[4 * ((H - 1 - i) * W + j) + 3] = 255;
            }
        }

        texture.update(pixels);
        sprite.setTexture(texture);

        window.draw(sprite);
        window.display();

        cout << "FPS:" << 1.f / fps_meter.restart().asSeconds() << "\n";
    }

    return 0;
}

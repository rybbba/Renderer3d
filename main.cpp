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
    float step = 2;
    const size_t W = 1000;
    const size_t H = 1000;

    sf::Font font;
    if (!font.loadFromFile("../fonts/font_main.ttf")) {
        cout << "Cannot load font";
        return 1;
    }
    sf::Text text_info;
    text_info.setFont(font);
    text_info.setCharacterSize(24);
    text_info.setFillColor(sf::Color::White);

    Screen res(W, H);

    Scene scene;

    Camera camera{0.001, 100, -0.001, 0.001, -0.001, 0.001, {0, 0, 3}, {0, 0, 0}};

    Renderer renderer(res);

    sf::RenderWindow window(sf::VideoMode(W, H), "3d render");

    sf::Texture texture;
    texture.create(W, H);
    sf::Sprite sprite;

    sf::Clock fps_meter;
    sf::Clock timer;


    Triangle a({-1, -1, 0, 1}, {1, -1, 0, 1}, {0, 1, 0, 1});
    for (int iter = 0; iter < 1000; ++iter) {
        auto z = - (float) iter / 10;
        scene.addObject(&a, {{0, 0, z}, {0, 0, 0}, {1, 1, 1}, {(iter % 3 == 0) ? 255 : 0, (iter % 3 == 1) ? 255 : 0, (iter % 3 == 2) ? 255 : 0}});
    }

    auto triangles_count = scene.countTriangles();

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float step_timed  = step * timer.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            step_timed *= 2;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            camera.position += camera.getForwardDirection() * step_timed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            camera.position -= camera.getForwardDirection() * step_timed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            camera.position += camera.getForwardDirection().cross(camera.getTopDirection()) * step_timed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            camera.position -= camera.getForwardDirection().cross(camera.getTopDirection()) * step_timed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            camera.angle.y() = fmodf(camera.angle.y() + step_timed, 2 * M_PI);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            camera.angle.y() = fmodf(camera.angle.y() - step_timed, 2 * M_PI);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            camera.angle.x() = fmodf(camera.angle.x() + step_timed, 2 * M_PI);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            camera.angle.x() = fmodf(camera.angle.x() - step_timed, 2 * M_PI);
        }

        renderer.clearScreen();

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

        std::ostringstream stringStream_info;
        stringStream_info << "FPS: " << 1.f / fps_meter.restart().asSeconds() << "\n";
        stringStream_info << "Triangles on scene: " << triangles_count;
        text_info.setString(stringStream_info.str());
        window.draw(text_info);

        window.display();

    }

    return 0;
}

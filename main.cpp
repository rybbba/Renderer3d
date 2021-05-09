#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Screen.h"
#include "Camera.h"
#include "Scene.h"
#include "Renderer.h"

using namespace std;

int main() {
    Triangle a({1, 0, 0, 1}, {0, 1, 0, 1}, {0, 0, 1, 1});

    const size_t W = 1000;
    const size_t H = 1000;

    Screen res(W, H);

    Scene scene;

    Camera camera {0.001, 100, -0.001, 0.001, -0.001, 0.001};
    scene.setCamera(camera);

    Renderer renderer(res);


    sf::RenderWindow window(sf::VideoMode(W, H), "3d render");

    sf::Texture texture;
    texture.create(W, H);
    sf::Sprite sprite;

    sf::Clock timer;
    sf::Clock fps_meter;

    float r_angle = 0;
    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        renderer.clear();

        r_angle = fmodf(r_angle + 1.5*timer.getElapsedTime().asSeconds(), 2*M_PI);
        timer.restart();

        scene.setScene({&a, &a, &a, &a, &a, &a, &a, &a}, {
                 {{0, -0.5, -3}, {0, r_angle, 0}, {1, 1, 1}, {255, 0, 0}}
                ,{{0, -0.5, -3}, {0, r_angle + M_PI/2, 0}, {1, 1, 1}, {0, 255, 0}}
                ,{{0, -0.5, -3}, {0, r_angle + M_PI, 0}, {1, 1, 1}, {0, 0, 255}}
                ,{{0, -0.5, -3}, {0, r_angle - M_PI/2, 0}, {1, 1, 1}, {255, 255, 255}}
                ,{{0, -0.5, -3}, {M_PI, r_angle, 0}, {1, 1, 1}, {255, 0, 0}}
                ,{{0, -0.5, -3}, {M_PI, r_angle + M_PI/2, 0}, {1, 1, 1}, {0, 255, 0}}
                ,{{0, -0.5, -3}, {M_PI, r_angle + M_PI, 0}, {1, 1, 1}, {0, 0, 255}}
                ,{{0, -0.5, -3}, {M_PI, r_angle - M_PI/2, 0}, {1, 1, 1}, {255, 255, 255}}
        });

        renderer.render(scene);

        sf::Uint8 pixels[4 * W * H];

        for (int i = H - 1; i >= 0; --i) {
            for (int j = 0; j < W; ++j) {
                auto pix = res(j, i);
                pixels[4*((H-1 - i)*W + j)] = pix[0];
                pixels[4*((H-1 - i)*W + j)+1] = pix[1];
                pixels[4*((H-1 - i)*W + j)+2] = pix[2];
                pixels[4*((H-1 - i)*W + j)+3] = 255;
            }
        }

        texture.update(pixels);
        sprite.setTexture(texture);

        window.clear();
        window.draw(sprite);
        window.display();

        cout << "FPS:" << 1.f / fps_meter.restart().asSeconds() << "\n";
    }

    return 0;
}

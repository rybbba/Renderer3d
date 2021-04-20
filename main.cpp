#include <iostream>
#include <SFML/Graphics.hpp>

#include "Screen.h"
#include "Renderer.h"

using namespace std;

int main() {
    Triangle a({0, -0.6, 0, 1}, {-0.5, 0.4, 0, 1}, {0.5, 0.4, 0, 1});
    Triangle b({0, 0.6, 0, 1}, {-0.5, -0.4, 0, 1}, {0.5, -0.4, 0, 1});

    const size_t W = 1000;
    const size_t H = 1000;

    Screen res(W, H);
    Renderer renderer(res);


    sf::RenderWindow window(sf::VideoMode(W, H), "3d render");

    sf::Texture texture;
    texture.create(W, H);
    sf::Sprite sprite;

    sf::Clock timer;
    sf::Clock fps_meter;

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        renderer.clear();

        a.rotateY(2*timer.getElapsedTime().asSeconds());
        b.rotateY(2*timer.getElapsedTime().asSeconds());
        timer.restart();

        renderer.render({&a, &b, &a, &b}, {{-0.3, 0, -2}
                                                            ,{-0.3, 0, -2}
                                                            ,{0.3, 0, -2}
                                                            ,{0.3, 0, -2}});

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

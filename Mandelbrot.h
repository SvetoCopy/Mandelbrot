#include "stdio.h"
#include <SFML/Graphics.hpp>
#include <math.h>

const float HORIZONTAL_SHIFT_AMOUNT = 0.2f;
const float VERTICAL_SHIFT_AMOUNT   = 0.2f;

class Mandelbrot {
public:
    size_t width  = 100;
    size_t height = 800;

    Mandelbrot(size_t win_width, size_t win_height);

    sf::Image   GetPixels();
    void        SetPixel(size_t x, size_t y, sf::Color color);

    void        CalculatePixels(float x_center, float y_center, float scale);
    void        Display();

private:
    sf::Image pixels        = {};
    sf::RenderWindow window = {};
};
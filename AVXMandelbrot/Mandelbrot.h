#include "stdio.h"
#include <SFML/Graphics.hpp>
#include <math.h>

#define range_15_to_0_nums 15, 14, 13, 12, 11, 10, 9, 8, 7, 6 , 5, 4, 3, 2, 1, 0

const float HORIZONTAL_SHIFT_AMOUNT = 0.2f;
const float VERTICAL_SHIFT_AMOUNT   = 0.2f;

const int MULTIPLE_DATA_SIZE = 16;

const float SCALE_DX = 0.1;

const int N_MAX  = 256;
const int R2_MAX = 100.f;

const float MAX_COLOR_NUM = 255.f;

const float INIT_DX = 1/800.f;
const float INIT_DY = 1/800.f;

const float INIT_X_CENTER = 0.f;
const float INIT_Y_CENTER = 0.f;
const float INIT_SCALE    = 1.f;

const size_t INIT_WIDTH  = 100;
const size_t INIT_HEIGHT = 800;

class Mandelbrot {
public:
    size_t width  = INIT_WIDTH;
    size_t height = INIT_HEIGHT;

    Mandelbrot(size_t win_width, size_t win_height);

    sf::Image   GetPixels();
    void        SetPixel(size_t x, size_t y, sf::Color color);

    void        CalculatePixels(float x_center, float y_center, float scale);
    void        Display();

private:
    sf::Image pixels        = {};
    sf::RenderWindow window = {};
};
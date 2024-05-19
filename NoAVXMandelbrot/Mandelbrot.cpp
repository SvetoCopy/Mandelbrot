#include "Mandelbrot.h"

char ColorCalculate(char c) {
    return c % 2 * 64;
}

Mandelbrot::Mandelbrot(size_t win_width, size_t win_height) : 
            height{win_height}, 
            width{win_width}, 
            window{sf::VideoMode(win_width, win_height), "Mandelbrot", sf::Style::Default}
{
    this->pixels.create(win_width, win_height, sf::Color(0, 0, 0));
}

sf::Image Mandelbrot::GetPixels() 
{
    return this->pixels;
}

void Mandelbrot::SetPixel(size_t x, size_t y, sf::Color color)
{
    this->pixels.setPixel(x, y, color);
}

void Mandelbrot::CalculatePixels(float x_center, float y_center, float scale)
{
    const float dx = INIT_DX * scale, dy = INIT_DY * scale;
    
    for (int iy = 0; iy < this->height; iy++)
    {
        float x0 = ((            - (float)this->width)  * dx) + x_center;
        float y0 = (((float)iy   - (float)this->height) * dy) + y_center;

        for (int ix = 0; ix < this->width; ix++, x0 += dx)
        {
            float X = x0;
            float Y = y0;

            int N = 0;
            for (; N < N_MAX; N++)
            {
                float x2 = X*X;
                float y2 = Y*Y;
                float xy = X*Y;

                float r2 = x2 + y2;

                if (r2 >= R2_MAX) break;

                X = x2 - y2 + x0;
                Y = 2*xy    + y0;
            }

            float I = sqrtf(sqrtf ((float)N / (float)N_MAX)) * MAX_COLOR_NUM;

            char c = (char)I;
            sf::Color color(c, c, ColorCalculate(c));
            this->SetPixel(ix, iy, color);
        }
    }

    sf::Texture texture;
    texture.loadFromImage(this->GetPixels());
    
    sf::Sprite sprite;
    sprite.setTexture(texture, true);

    this->window.draw(sprite);
}

void Mandelbrot::Display()
{
    float x_center = 0.f;
    float y_center = 0.f;
    float scale    = 1.f;

    this->CalculatePixels(x_center, y_center, scale);
    this->window.display();
    
    while (this->window.isOpen())
    {
        sf::Event event = {};
        
        while (this->window.pollEvent(event)) 
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Right) 
                    x_center += HORIZONTAL_SHIFT_AMOUNT;

                else if (event.key.code == sf::Keyboard::Left)
                    x_center -= HORIZONTAL_SHIFT_AMOUNT;

                else if (event.key.code == sf::Keyboard::Down)
                    y_center += VERTICAL_SHIFT_AMOUNT;

                else if (event.key.code == sf::Keyboard::Up)
                    y_center -= VERTICAL_SHIFT_AMOUNT;
                
                else if (event.key.code == sf::Keyboard::A)
                    scale += SCALE_DX;
                
                else if (event.key.code == sf::Keyboard::Z)
                    scale -= SCALE_DX;

                else if (event.key.code == sf::Keyboard::Escape)
                    this->window.close();
            }    
        }

        this->CalculatePixels(x_center, y_center, scale);
        this->window.display();
    }
}
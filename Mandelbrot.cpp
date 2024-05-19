#include "Mandelbrot.h"
#include <immintrin.h>

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
    const int   n_max = 256;
    const float dx = 1/800.f * scale, dy = 1/800.f * scale;

    const __m512 r2_max = _mm512_set1_ps(100.f);
    const __m512 N_max  = _mm512_set1_ps(n_max);
    
    for (int iy = 0; iy < this->height; iy++)
    {
        float x0 = ((            - (float)this->width)  * dx) + x_center;
        float y0 = (((float)iy   - (float)this->height) * dy) + y_center;

        for (int ix = 0; ix < this->width; ix += 16, x0 += 16 * dx)
        {
            __m512 range_15_to_0 = _mm512_set_ps(range_15_to_0_nums);
            
            __m512 X0 = _mm512_add_ps(_mm512_set1_ps(x0), _mm512_mul_ps(range_0_to_15, _mm512_set1_ps(dx)));
            __m512 Y0 = _mm512_set1_ps(y0); 

            __m512 X = X0; 
            __m512 Y = Y0; 

            __m512i N = _mm512_setzero_si512();

            for (int n = 0; n < n_max; n++)
            {
                __m512 x2 = _mm512_mul_ps(X, X);
                __m512 y2 = _mm512_mul_ps(Y, Y);
                __m512 xy = _mm512_mul_ps(X, Y);

                __m512 r2 = _mm512_mul_ps(x2, y2);

                __mmask16 cmp = _mm512_cmple_ps_mask(r2, r2_max);
                
                unsigned int mask = _cvtmask16_u32(cmp);
                if (!mask) break;

                __m512i cnst_1 = _mm512_set1_epi32(1);

                N = _mm512_mask_add_epi32(N, cmp, N, cnst_1);
                // for (int i = 0; i < 4; i++) N[i] = N[i] + cmp[i];

                X = _mm512_add_ps(_mm512_sub_ps(x2, y2), X0); // X[i] = x2[i] - y2[i] + X0[i]
                Y = _mm512_add_ps(_mm512_add_ps(xy, xy), Y0); // Y[i] = 2 * xy[i]  + Y0[i]
            }

            __m512 cnst_255 = _mm512_set1_ps(255.f);
            __m512 shade = _mm512_mul_ps (_mm512_sqrt_ps(_mm512_sqrt_ps (_mm512_div_ps (_mm512_cvtepi32_ps (N), N_max))), cnst_255);

            for (int i = 0; i < 16; i++) {

                float* p_shade = (float*)&shade; 
                
                char c = (char)p_shade[i];
                sf::Color color(c, c, c % 2 * 64);
                this->SetPixel(ix + i, iy, color);
            }

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
    float x_center = INIT_X_CENTER;
    float y_center = INIT_Y_CENTER;
    float scale    = INIT_SCALE;

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
                    scale += 0.1;
                
                else if (event.key.code == sf::Keyboard::Z && (scale - 0.1) > 0){
                    scale -= 0.1;
                }

                else if (event.key.code == sf::Keyboard::Escape)
                    this->window.close();
            }    
        }

        this->CalculatePixels(x_center, y_center, scale);
        this->window.display();
    }
}
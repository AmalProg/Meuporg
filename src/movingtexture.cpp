#include "movingtexture.hpp"

MovingTexture::MovingTexture(float switchTime, bool random) : c_TextureState(0),
c_SwitchTime(switchTime), c_Random(random)
{
}

void MovingTexture::addRect(sf::IntRect rect)
{
    c_Rects.push_back(rect);
}

void MovingTexture::update(const sf::Time & elapsed)
{
    c_TextureSwitchTime += elapsed;

    if(c_TextureSwitchTime.asSeconds() > c_SwitchTime)
    {
        if(c_Random)
        {
            uint16_t lastState = c_TextureState;
            do{
            c_TextureState = rand() % (c_Rects.size() + 1);
            }while(lastState == c_TextureState);
        }
        else
            c_TextureState++;
        if(c_TextureState == c_Rects.size())
            c_TextureState = 0;
        c_TextureSwitchTime = sf::Time::Zero;
    }
}

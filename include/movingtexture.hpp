#ifndef MOVINGTEXTURE_HPP
#define MOVINGTEXTURE_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

class MovingTexture
{
    public:
        MovingTexture(float switchTime = 1, bool random = false);

        void addRect(sf::IntRect rect);

        void update(const sf::Time & elapsed);

        void setSwitchTime(float switchTime) { c_SwitchTime = switchTime; }
        void setRandom(bool random) { c_Random = random; }

        sf::IntRect getRect() const { if(!c_Rects.empty())return c_Rects[c_TextureState]; return sf::IntRect(0,0,0,0);}
        float getSwitchTime() const { return c_SwitchTime; }
        bool getRandom() const { return c_Random; }


    private:
        sf::Time c_TextureSwitchTime;
        uint16_t c_TextureState;
        float c_SwitchTime;
        bool c_Random;
        std::vector< sf::IntRect > c_Rects;
};

#endif // MOVINGTEXTURE_HPP

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include "Character.hpp"

#define NBRSLOT 4

class Player : public Character
{
    public:
        Player(const std::string & name = "unknow", float maxLife = 100, Direction dir = DOWN, const sf::Vector2f & pos = sf::Vector2f(0, 0));

        void setShortCut(uint16_t itemIndex, sf::Keyboard::Key key);

        const Item * getItemShortCut(sf::Keyboard::Key key);
        int16_t getItemIndexShortCut(sf::Keyboard::Key key);

    private:
        std::map< sf::Keyboard::Key, const Item * > c_Shortcuts; //
};

#endif // PLAYER_HPP

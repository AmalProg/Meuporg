#include "Player.hpp"

Player::Player(const std::string & name, float maxLife, Direction dir, const sf::Vector2f & pos)
 : Character(PLAYER, name, maxLife, dir, pos)
{
    c_Shape.setFillColor(sf::Color(0, 120, 0));
    c_Speed = 20;
}

void Player::setShortCut(uint16_t itemIndex, sf::Keyboard::Key key)
{
    if(itemIndex < c_Bag->getNbrItems())
    {
        c_Shortcuts[key] = c_Bag->getItem(itemIndex);
    }
}

const Item * Player::getItemShortCut(sf::Keyboard::Key key)
{
    int16_t itemIndex = c_Bag->findItem(c_Shortcuts[key]);
    if(itemIndex != -1)
        return c_Bag->getItem(itemIndex);
    return NULL;
}
int16_t Player::getItemIndexShortCut(sf::Keyboard::Key key)
{
    return c_Bag->findItem(c_Shortcuts[key]);
}


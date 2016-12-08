#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "living.hpp"
class Item;
#include "bag.hpp"

class Character : public Living
{
    public:
        Character(EntityTypeId typeId = CHARACTER, const std::string & name = "unknow", float maxLife = 100, Direction dir = DOWN, float speed = 2,
                  const sf::Vector2f & pos = sf::Vector2f(0, 0));
        virtual ~Character();

        void update(const sf::Time & elapsed);

        bool takeItem(const Item * item, uint16_t nbr); // rammasse un 'nbr' d''item'
        void removeItem(const Item * item, uint16_t nbr); // detruit un 'nbr' d''item'

        void setDirection(Direction dir) { c_TurnTime = sf::Time::Zero; Living::setDirection(dir); }

        const Item * getItem(uint16_t i) const;
        const Bag * getBag() const { return c_Bag; }
        bool isMoveable();

    protected:
        Bag * c_Bag;
        sf::Time c_TurnTime;
};

#endif // CHARACTER_HPP

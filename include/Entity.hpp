#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

enum EntityTypeId{GRASS = 0, WATER, SAND, SOIL, FIRE, ROCK, DOOR, LOCKEDDOOR, STAIRS, LOOTBAG, CHARACTER, SHEEP, WOLF, PLAYER};

class Entity
{
    public:
        Entity(EntityTypeId typeId, const sf::Vector2f & pos);
        ~Entity() {};

        void setPosition(uint16_t i, uint16_t j) {c_Position = sf::Vector2f(i, j);}
        void setPosition(const sf::Vector2f & position) { c_Position = position; }

        sf::Vector2f getPosition() const {return c_Position;}
        EntityTypeId getEntityTypeId() const { return c_EntityTypeId; }
        uint32_t getEntityId() const { return c_EntityId; }

        static uint32_t entityNbr;

    protected:
        sf::Vector2f c_Position;

        EntityTypeId c_EntityTypeId;
        uint32_t c_EntityId;
};

#endif // ENTITY_HPP

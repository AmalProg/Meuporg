#include "Entity.hpp"

uint32_t  Entity::entityNbr = 0;

Entity::Entity(EntityTypeId typeId, const sf::Vector2f & pos) : c_Position(pos)
{
    c_EntityTypeId = typeId;
    c_EntityId = Entity::entityNbr;
    Entity::entityNbr++;
}

#include "Entity.hpp"

uint32_t  Entity::entityNbr = 0;

Entity::Entity(EntityTypeId typeId, const sf::Vector2f & pos) : c_Position(pos)
{
    c_EntityTypeId = typeId;
    c_EntityId = Entity::entityNbr;
    Entity::entityNbr++;
}

sf::Texture Entity::obstacleTexture;
sf::Texture Entity::lootbagTexture;
sf::Texture Entity::playerTextures;
void Entity::initTextures()
{
    Entity::obstacleTexture.loadFromFile("image\\map\\obstacleTextures.png");
    Entity::lootbagTexture.loadFromFile("image\\map\\lootbagTexture.png");
    Entity::playerTextures.loadFromFile("image\\map\\playerTextures.png");
}

void Entity::draw(sf::RenderWindow & app, uint16_t cellSize)
{
    c_Sprite.setScale(cellSize / c_Sprite.getLocalBounds().width, cellSize / c_Sprite.getLocalBounds().height);
    c_Sprite.setPosition(cellSize * c_Position.x, cellSize * c_Position.y);
    app.draw(c_Sprite);
}

#include "living.hpp"

Living::Living(EntityTypeId typeId, const std::string & name, float maxLife, Direction dir, const sf::Vector2f & pos,
               float speed)
: Entity(typeId, pos), c_Name(name), c_MaxLife(maxLife), c_Life(maxLife), c_IsDead(false), c_Killer(NULL)
, c_Direction(dir), c_Speed(speed), c_IsMoveable(true)
{
}

Living::~Living()
{
}

void Living::update(const sf::Time & elapsed)
{
    c_SpeedTime += elapsed;
    if(elapsed.asSeconds() < 1.f / c_Speed && c_SpeedTime.asSeconds() > 1.f / c_Speed)
        c_SpeedTime = sf::seconds(1.f / c_Speed);
}

bool Living::isMoveable()
{
    if(c_SpeedTime.asSeconds() >= 1.f / c_Speed - 0.001) // 0.001 contre les erreurs d'arrondi
    {
        c_IsMoveable = true;
    }

    return c_IsMoveable;
}

void Living::setPosition(uint16_t i, uint16_t j)
{
    c_IsMoveable = false;
    c_SpeedTime -= sf::seconds(1.f / c_Speed);
    Entity::setPosition(i, j);
}
void Living::setPosition(const sf::Vector2f & position)
{
    c_IsMoveable = false;
    c_SpeedTime -= sf::seconds(1.f / c_Speed);
    Entity::setPosition(position);
}
void Living::setDirection(Direction dir)
{
    if(dir == LEFT)
        c_Sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    else if(dir == RIGHT)
        c_Sprite.setTextureRect(sf::IntRect(128, 0, 64, 64));
    else if(dir == UP)
        c_Sprite.setTextureRect(sf::IntRect(192, 0, 64, 64));
    else
        c_Sprite.setTextureRect(sf::IntRect(64, 0, 64, 64));

    c_Direction = dir;
}


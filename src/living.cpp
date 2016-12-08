#include "living.hpp"

Living::Living(EntityTypeId typeId, const std::string & name, float maxLife, Direction dir, const sf::Vector2f & pos,
               float speed)
: Entity(typeId, pos), c_Name(name), c_MaxLife(maxLife), c_Life(maxLife), c_IsDead(false), c_Killer(NULL)
, c_Direction(dir), c_Speed(speed), c_IsMoveable(true)
{
    c_Shape.setFillColor(sf::Color::Blue);
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

void Living::draw(sf::RenderWindow & app, uint16_t cellSize)
{
    c_Shape.setSize(sf::Vector2f(cellSize * 3/4, cellSize * 3/4));
    c_Shape.setPosition(cellSize * c_Position.x + cellSize * 1/8, cellSize * c_Position.y + cellSize * 1/8);
    app.draw(c_Shape);
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


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
    if(c_SpeedTime.asSeconds() > 1.f / c_Speed)
        c_IsMoveable = true;

    int16_t moveOnX;
    int16_t moveOnY;
    int16_t gapX = (c_PosToMove.x - c_Sprite.getPosition().x);
    int16_t gapY = (c_PosToMove.y - c_Sprite.getPosition().y);

    uint16_t k = 5;
    if(gapX != 0)
        moveOnX = gapX / abs(gapX) * (c_CellSize/k * (c_CellSize/k <= abs(gapX)) + 1 * (c_CellSize/k > abs(gapX)));
    if(gapY != 0)
        moveOnY = gapY / abs(gapY) * (c_CellSize/k * (c_CellSize/k <= abs(gapY)) + 1 * (c_CellSize/k > abs(gapY)));

    c_Sprite.move(moveOnX, moveOnY);
}

void Living::moveTo(uint16_t i, uint16_t j, uint16_t cellSize)
{
    c_IsMoveable = false;
    c_PosToMove = sf::Vector2f(i * cellSize, j * cellSize);
    c_CellSize = cellSize;
    c_SpeedTime = sf::Time::Zero;

    Entity::setPosition(i, j);
}

void Living::draw(sf::RenderWindow & app, uint16_t cellSize)
{
    c_Sprite.setScale(cellSize / c_Sprite.getLocalBounds().width, cellSize / c_Sprite.getLocalBounds().height);

    app.draw(c_Sprite);
}

void Living::setPosition(uint16_t i, uint16_t j, uint16_t cellSize)
{
    c_Sprite.setPosition(i * cellSize, j * cellSize);
    c_PosToMove = sf::Vector2f(i * cellSize, j * cellSize);

    Entity::setPosition(i, j);
}
void Living::setPosition(const sf::Vector2f & position, uint16_t cellSize)
{
    c_Sprite.setPosition(position.x * cellSize, position.y * cellSize);
    c_PosToMove = sf::Vector2f(position.x * cellSize, position.y * cellSize);

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


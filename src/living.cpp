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

    int16_t moveOnX = 0;
    int16_t moveOnY = 0;
    int16_t gapX = c_PosToMove.x - c_PosBefMove.x;
    int16_t gapY = c_PosToMove.y - c_PosBefMove.y;
    float distGap = sqrt(gapX * gapX + gapY * gapY);
    int16_t gapLeftX = c_PosToMove.x - c_Sprite.getPosition().x;
    int16_t gapLeftY = c_PosToMove.y - c_Sprite.getPosition().y;

    if(gapLeftX != 0 || gapLeftY != 0)
    {
        c_SmoothMoveTime += elapsed;
        if(c_SmoothMoveTime.asSeconds() * distGap * c_Speed >= 1)
        {
            int16_t pixToMove = trunc(c_SmoothMoveTime.asSeconds() * distGap * c_Speed);
            if(gapLeftX != 0)
                moveOnX = gapLeftX / abs(gapLeftX) * (pixToMove * (pixToMove <= abs(gapLeftX)) + abs(gapLeftX) * (pixToMove > abs(gapLeftX)));
            if(gapLeftY != 0)
                moveOnY = gapLeftY / abs(gapLeftY) * (pixToMove * (pixToMove <= abs(gapLeftY)) + abs(gapLeftY) * (pixToMove > abs(gapLeftY)));

            c_SmoothMoveTime -= sf::seconds(pixToMove/(distGap*c_Speed));
        }
        c_Sprite.move(moveOnX, moveOnY);
    }
    else
        c_SmoothMoveTime = sf::Time::Zero;

    for(std::list< const Effect * >::const_iterator it = c_Effects.begin(); it != c_Effects.end(); it++)
    {
        c_EffectsTime[(*it)->getId()] -= elapsed;
        c_EffectsTickTime[(*it)->getId()] -= elapsed;
    }
}

void Living::moveTo(uint16_t i, uint16_t j, uint16_t cellSize)
{
    c_IsMoveable = false;
    c_PosToMove = sf::Vector2f(i * cellSize, j * cellSize);
    c_PosBefMove = sf::Vector2f(c_Sprite.getPosition().x, c_Sprite.getPosition().y);
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
    c_PosBefMove = sf::Vector2f(i * cellSize, j * cellSize);

    Entity::setPosition(i, j);
}
void Living::setPosition(const sf::Vector2f & position, uint16_t cellSize)
{
    c_Sprite.setPosition(position.x * cellSize, position.y * cellSize);
    c_PosToMove = sf::Vector2f(position.x * cellSize, position.y * cellSize);
    c_PosBefMove = sf::Vector2f(position.x * cellSize, position.y * cellSize);

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

void Living::addEffect(const Effect * effect)
{
    c_Effects.push_back(effect);
    c_EffectsTime[effect->getId()] = sf::seconds(effect->getDuration());
    c_EffectsTickTime[effect->getId()] = sf::Time::Zero;
}

void Living::realTimeAction(Map * m, Player * p)
{
    std::vector< std::list< const Effect * >::iterator > effectsToDestroy;

    for(std::list< const Effect * >::iterator it = c_Effects.begin(); it != c_Effects.end(); it++)
    {
        float valueThisTick = 0;
        if((*it)->getDuration() == 0)
            valueThisTick = (*it)->getValue();
        else if(c_EffectsTickTime[(*it)->getId()].asSeconds() <= 0)
        {
            if((*it)->getTickTime() == 0)
            {
                valueThisTick = (*it)->getValue();
                c_EffectsTickTime[(*it)->getId()] = sf::seconds((*it)->getDuration()*2); // l'effet ne sera pas réappliquer
            }
            else
            {
                valueThisTick = (*it)->getValue() * (*it)->getTickTime() / (*it)->getDuration();
                c_EffectsTickTime[(*it)->getId()] = sf::seconds((*it)->getTickTime());
            }
        }

        bool effectFade = false;
        if(c_EffectsTime[(*it)->getId()].asSeconds() <= 0)
        {
            effectFade = true;
            effectsToDestroy.push_back(it);
        }

        switch((*it)->getType())
        {
        case HEAL:
            heal(valueThisTick);
            break;

        case DAMAGE:
            takeDamages(valueThisTick);
            if(isDead())
                setKiller((*it)->getLauncher());
            break;

        case SPEED:
            c_Speed += valueThisTick;
            if(effectFade)
                c_Speed -= (*it)->getValue();
            break;

        default:
            break;
        }
    }

    for(uint16_t i = 0; i < effectsToDestroy.size(); i++)
        c_Effects.erase(effectsToDestroy[i]);
}

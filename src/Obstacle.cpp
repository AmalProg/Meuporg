#include "Obstacle.hpp"

Obstacle::Obstacle(EntityTypeId typeId, const sf::Vector2f & pos, bool walkable, bool visionBlocking, bool attackBlocking
                   , bool filler, bool cover)
 : Entity(typeId, pos), c_Walkable(walkable), c_VisionBlocking(visionBlocking), c_AttackBlocking(attackBlocking)
 ,  c_IsFiller(filler), c_IsCover(cover)
{
    c_Shape.setFillColor(sf::Color::Black);
}

void Obstacle::draw(sf::RenderWindow & app, uint16_t cellSize)
{
    c_Shape.setSize(sf::Vector2f(cellSize, cellSize));
    c_Shape.setPosition(cellSize * c_Position.x, cellSize * c_Position.y);
    app.draw(c_Shape);
}
/***/
void Fire::realTimeAction(Map * m, Player * p)
{
    float randSwitchTime = 0.20 + (rand() % 31) / 100.f;
    if(c_FirstStateOfFire && c_SwitchTime.asSeconds() > randSwitchTime)
    {
        c_Shape.setFillColor(sf::Color(255, 155, 0));
        c_FirstStateOfFire = false;
        c_SwitchTime -= sf::seconds(randSwitchTime);
    }
    else if(c_SwitchTime.asSeconds() > randSwitchTime)
    {
        c_Shape.setFillColor(sf::Color(200, 100, 0));
        c_FirstStateOfFire = true;
        c_SwitchTime -= sf::seconds(randSwitchTime);
    }

    if(c_LastTickTime.asSeconds() > c_DamageTickTime)
    {
        c_Ticking = true;
        c_LastTickTime -= sf::seconds(c_DamageTickTime);
    }
    else
        c_Ticking = false;
}
void Fire::walkAction(Map * mape, Living * l)
{
    if(c_Ticking)
    {
        l->takeDamages(c_DamagePerTick);
        std::cout << "Fire made " << c_DamagePerTick << " damages done on " << l->getName() << "\n";
        c_Ticking = false;
    }
}
/***/
Door::Door(bool isOpen, EntityTypeId typeId, const sf::Vector2f & pos)
: Obstacle(typeId, pos, isOpen, true, false, true, false), c_IsOpen(isOpen)
{
    if(c_IsOpen)
    {
        c_Shape.setFillColor(sf::Color(200, 150 ,30, 100));
    }
    else
    {
        c_Shape.setFillColor(sf::Color(200, 150 ,30));
    }
}
void Door::speakAction(Map * mape, Player * p)
{
    if(c_IsOpen)
    {
        c_Shape.setFillColor(sf::Color(200, 150 ,30));
        c_IsOpen = false;
        setWalkable(false);
        mape->getCell(c_Position.x, c_Position.y)->stateTest();
        std::cout << "Vous fermez la porte.\n";
    }
    else
    {
        c_Shape.setFillColor(sf::Color(200, 150 ,30, 100));
        c_IsOpen = true;
        setWalkable(true);
        mape->getCell(c_Position.x, c_Position.y)->stateTest();
        std::cout << "Vous ouvrez la porte.\n";
    }
}
void Door::touchAction(Map * mape, Player * p)
{
    if(!c_IsOpen)
    {
        std::cout << "Cette porte est actuellement fermée !\n";
    }
}
/***/
void LockedDoor::speakAction(Map * mape, Player * p)
{
    if(!c_IsLocked)
    {
        Door::speakAction(mape, p);
    }
    else
    {
        const Bag * bag = p->getBag();
        for(uint16_t i = 0; i < bag->getNbrItems(); ++i)
        {
            if(bag->getItem(i) == (Item::getItemFromId(KEY)))
            {
                c_IsLocked = false;
                p->removeItem(Item::getItemFromId(KEY), 1);
                std::cout << "Vous  déverouillez la porte.\n";
            }
        }
    }
}
void LockedDoor::touchAction(Map * mape, Player * p)
{
    if(c_IsLocked)
        std::cout << "Cette porte est actuellement fermée à clef!" << '\n';
    else
        Door::touchAction(mape, p);
}
/***/

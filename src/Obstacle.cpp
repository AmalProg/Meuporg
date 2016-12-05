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
            if(bag->getItem(i) == (Item::key))
            {
                c_IsLocked = false;
                p->removeItem(Item::key, 1);
                std::cout << "Vous  déverouillez la porte.\n";
            }
        }
    }
}

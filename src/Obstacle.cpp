#include "Obstacle.hpp"

Obstacle::Obstacle(const sf::Vector2f & pos, bool walkable, bool visionBlocking, bool attackBlocking
                   , bool filler, bool cover)
 : Entity(pos), c_Walkable(walkable), c_VisionBlocking(visionBlocking), c_AttackBlocking(attackBlocking)
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

#include "Obstacle.hpp"

Obstacle::Obstacle(EntityTypeId typeId, const sf::Vector2f & pos, bool walkable, bool visionBlocking, bool attackBlocking
                   , bool filler, bool cover)
 : Entity(typeId, pos), c_Walkable(walkable), c_VisionBlocking(visionBlocking), c_AttackBlocking(attackBlocking)
 ,  c_IsFiller(filler), c_IsCover(cover)
{
    c_Sprite.setTexture(Entity::obstacleTexture);
}
/***/
void Fire::update(const sf::Time & elapsed)
{
    c_Rect.update(elapsed);
    c_Sprite.setTextureRect(c_Rect.getRect());
}
sf::Time Fire::c_LastTickTime;
float Fire::c_DamageTickTime = 0.5;
bool Fire::c_Ticking = false;
void Fire::updateTick(const sf::Time & elapsed)
{
    c_LastTickTime += elapsed;
    c_Ticking = false;
    if(c_LastTickTime.asSeconds() >= c_DamageTickTime)
    {
        c_Ticking = true;
        c_LastTickTime = sf::Time::Zero;
    }
}
void Fire::walkAction(Map * mape, Living * l)
{
    if(c_Ticking)
    {
        l->takeDamages(c_DamagePerTick);
        std::cout << "Fire made " << c_DamagePerTick << " damages done on " << l->getName() << "\n";
    }
}
/***/
Door::Door(bool isOpen, EntityTypeId typeId, const sf::Vector2f & pos)
: Obstacle(typeId, pos, isOpen, true, false, true, false), c_IsOpen(isOpen)
{
    if(c_IsOpen)
    {
        c_Sprite.setTextureRect(sf::IntRect(0, 1536, 512, 512));
    }
    else
    {
        c_Sprite.setTextureRect(sf::IntRect(512, 1536, 512, 512));
    }
}
void Door::speakAction(Map * mape, Player * p)
{
    if(c_IsOpen)
    {
        c_Sprite.setTextureRect(sf::IntRect(512, 1536, 512, 512));
        c_IsOpen = false;
        setWalkable(false);
        mape->getCell(c_Position.x, c_Position.y)->stateTest();
        std::cout << "Vous fermez la porte.\n";
    }
    else
    {
        c_Sprite.setTextureRect(sf::IntRect(0, 1536, 512, 512));
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

#include "Obstacle.hpp"

Obstacle::Obstacle(EntityTypeId typeId, const sf::Vector2f & pos, bool walkable, bool visionBlocking, bool attackBlocking
                   , bool filler, bool cover)
 : Entity(typeId, pos), c_Walkable(walkable), c_VisionBlocking(visionBlocking), c_AttackBlocking(attackBlocking)
 ,  c_IsFiller(filler), c_IsCover(cover)
{
    c_Sprite.setTexture(Entity::obstacleTexture);
}
/***/
std::map< uint32_t, sf::Time > QuickSand::c_StepTimes;
std::map< uint32_t, bool > QuickSand::c_AreStepping;
float QuickSand::c_TimeToGetStuck = 1.f;
std::map< uint32_t, sf::Time > QuickSand::c_LastTickTime;
float QuickSand::c_DamageTickTime = 0.5;
std::map< uint32_t, bool > QuickSand::c_Ticking;
void QuickSand::updateTick(const sf::Time & elapsed)
{
    for(std::map< uint32_t, sf::Time >::iterator it = c_StepTimes.begin(); it != c_StepTimes.end(); it++)
    {
        if(c_AreStepping[(*it).first])
            (*it).second += elapsed;
        else // si on ne marche pas dessus on reset le timer
            (*it).second = sf::Time::Zero;

        c_AreStepping[(*it).first] = false; // pour vois si on arche toujours dessus au prochain tour d eboucle
    }

    for(std::map< uint32_t, sf::Time >::iterator it = c_StepTimes.begin(); it != c_StepTimes.end(); it++)
    {
        if((*it).second.asSeconds() >= c_TimeToGetStuck)
        {
            c_LastTickTime[(*it).first] += elapsed;
            c_Ticking[(*it).first] = false;
            if(c_LastTickTime[(*it).first].asSeconds() >= c_DamageTickTime)
            {
                c_Ticking[(*it).first] = true;
                c_LastTickTime[(*it).first] = sf::Time::Zero;
            }
        }
    }
}
void QuickSand::firstStepAction(Map * m, Living * l)
{
    c_StepTimes[l->getEntityId()]; // crée un imer pour ce Living si il n'existe pas déjà

    Sand::firstStepAction(m, l);
}
void QuickSand::walkAction(Map * m, Living * l)
{
    c_AreStepping[l->getEntityId()] = true;

    if(c_Ticking[l->getEntityId()])
    {
        l->takeDamages(c_DamagePerTick);
        std::cout << "QuickSand made " << c_DamagePerTick << " damages done on " << l->getName() << "\n";
    }
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
void Fire::walkAction(Map * m, Living * l)
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
void Door::speakAction(Map * m, Player * p)
{
    if(c_IsOpen)
    {
        c_Sprite.setTextureRect(sf::IntRect(512, 1536, 512, 512));
        c_IsOpen = false;
        setWalkable(false);
        m->getCell(c_Position.x, c_Position.y)->stateTest();
        std::cout << "Vous fermez la porte.\n";
    }
    else
    {
        c_Sprite.setTextureRect(sf::IntRect(0, 1536, 512, 512));
        c_IsOpen = true;
        setWalkable(true);
        m->getCell(c_Position.x, c_Position.y)->stateTest();
        std::cout << "Vous ouvrez la porte.\n";
    }
}
void Door::touchAction(Map * m, Player * p)
{
    if(!c_IsOpen)
    {
        std::cout << "Cette porte est actuellement fermÃ©e !\n";
    }
}
/***/
void LockedDoor::speakAction(Map * m, Player * p)
{
    if(!c_IsLocked)
    {
        Door::speakAction(m, p);
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
                std::cout << "Vous  dÃ©verouillez la porte.\n";
            }
        }
    }
}
void LockedDoor::touchAction(Map * m, Player * p)
{
    if(c_IsLocked)
        std::cout << "Cette porte est actuellement fermÃ©e Ã  clef!" << '\n';
    else
        Door::touchAction(m, p);
}
/***/

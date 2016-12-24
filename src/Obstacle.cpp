#include "Obstacle.hpp"

sf::Texture Obstacle::obstacleTexture;
void Obstacle::initTextures()
{
    Obstacle::obstacleTexture.loadFromFile("image\\map\\obstacleTextures.png");
}

Obstacle::Obstacle(EntityTypeId typeId, const sf::Vector2f & pos, bool walkable, bool visionBlocking, bool attackBlocking
                   , bool filler, bool cover)
 : Entity(typeId, pos), c_Walkable(walkable), c_VisionBlocking(visionBlocking), c_AttackBlocking(attackBlocking)
 ,  c_IsFiller(filler), c_IsCover(cover)
{
    c_Sprite.setTexture(obstacleTexture);
}

void Obstacle::draw(sf::RenderWindow & app, uint16_t cellSize)
{
    c_Sprite.setScale(cellSize / c_Sprite.getLocalBounds().width, cellSize / c_Sprite.getLocalBounds().height);
    c_Sprite.setPosition(cellSize * c_Position.x, cellSize * c_Position.y);
    app.draw(c_Sprite);
}
/***/
void Fire::update(const sf::Time & elapsed)
{
    c_LastTickTime += elapsed;
    //c_Rect.setSwitchTime(0.20 + (rand() % 31) / 100.f);
    c_Rect.update(elapsed);
    c_Sprite.setTextureRect(c_Rect.getRect());
}
void Fire::realTimeAction(Map * m, Player * p)
{
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

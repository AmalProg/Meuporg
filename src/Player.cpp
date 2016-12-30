#include "Player.hpp"

Player::Player(const std::string & name, float maxLife, Direction dir, float speed, const sf::Vector2f & pos)
 : Character(PLAYER, name, maxLife, dir, speed, pos), c_WeaponEquipped(NULL), c_CanUseWeapon(true), c_WeaponCDTime(0)
 , c_MaxEquippedItem(NBRSLOT)
{
    c_Sprite.setTexture(Entity::playerTextures);
    setDirection(c_Direction); // pour initialiser le sprite

    for(uint16_t i = 0; i < c_MaxEquippedItem; i++)
        c_EquippedItems.push_back(NULL);
}

bool Player::takeItem(const Item * item, uint16_t nbr)
{
    c_CanUseItem[item->getItemId()] = true;

    return Character::takeItem(item, nbr);
}

void Player::update(const sf::Time & elapsed)
{
    for(uint16_t i = 0; i < c_Bag->getNbrItems(); i++)
    {
        const Item * item = c_Bag->getItem(i);
        c_LastItemUseTimes[item->getItemId()] += elapsed;
        if(c_LastItemUseTimes[item->getItemId()].asSeconds() >= c_ItemsCDTime[item->getItemId()])
        {
            c_CanUseItem[item->getItemId()] = true;
        }
    }

    c_LastWeaponUseTime += elapsed;
    if(c_LastWeaponUseTime.asSeconds() >= c_WeaponCDTime)
    {
        c_CanUseWeapon = true;
    }

    Character::update(elapsed);
}

bool Player::equip(const Item * i)
{
    if(i->getItemType() == EQUIPMENT)
    {
        c_WeaponEquipped = i;
        c_WeaponCDTime = i->getCDTime();
        return true;
    }
    return false;
}
void Player::itemUsed(const Item * item)
{
    c_CanUseItem[item->getItemId()] = false;
    c_LastItemUseTimes[item->getItemId()] = sf::Time::Zero;
    c_ItemsCDTime[item->getItemId()] = item->getCDTime();
}

void Player::removeItem(const Item * item, uint16_t nbr)
{
    if(item == c_WeaponEquipped)
        c_WeaponEquipped = NULL;


    Character::removeItem(item, nbr);
}

void Player::setEquippedItem(const Item * item, uint16_t i)
{
    if(c_Bag->findItem(item) >= 0)
    {
        c_EquippedItems[i] = item;
    }
}

bool Player::canUseItem(const Item * item) const
{
    return c_CanUseItem.at(item->getItemId());
}
float Player::getItemActualCdTime(const Item * item) const
{
    float cd = c_ItemsCDTime.at(item->getItemId()) - c_LastItemUseTimes.at(item->getItemId()).asSeconds();
    if(cd < 0)
        return 0;
    return cd;
}
float Player::getWeaponActualCdTime() const
{
    float cd = c_WeaponCDTime - c_LastWeaponUseTime.asSeconds();
    if(cd < 0)
        return 0;
    return cd;
}

const Item * Player::getEquippedItem(uint16_t i) const
{
    int16_t itemIndex = c_Bag->findItem(c_EquippedItems[i]);
    if(itemIndex != -1)
        return c_Bag->getItem(itemIndex);
    return NULL;
}
int16_t Player::getBagIndexOfEquippedItem(uint16_t i) const
{
    return c_Bag->findItem(c_EquippedItems[i]);
}


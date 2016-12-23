#include "Player.hpp"

Player::Player(const std::string & name, float maxLife, Direction dir, float speed, const sf::Vector2f & pos)
 : Character(PLAYER, name, maxLife, dir, speed, pos), c_WeaponEquipped(NULL), c_CanUseWeapon(true), c_WeaponCDTime(0)
{
    c_Shape.setFillColor(sf::Color(0, 120, 0));
}

bool Player::takeItem(const Item * item, uint16_t nbr)
{
    c_CanUseItem[item->getItemId()] = true;

    Character::takeItem(item, nbr);
}

void Player::update(const sf::Time & elapsed)
{
    for(uint16_t i = 0; i < c_Bag->getNbrItems(); i++)
    {
        const Item * item = c_Bag->getItem(i);
        c_LastItemUseTimes[item->getItemId()] += elapsed;
        if(elapsed.asSeconds() < c_ItemsCDTime[item->getItemId()] && c_LastItemUseTimes[item->getItemId()].asSeconds() > c_ItemsCDTime[item->getItemId()])
        {
            c_LastItemUseTimes[item->getItemId()] = sf::seconds(c_ItemsCDTime[item->getItemId()]);
            c_CanUseItem[item->getItemId()] = true;
        }
    }

    c_LastWeaponUseTime += elapsed;
    if(elapsed.asSeconds() < c_WeaponCDTime && c_LastWeaponUseTime.asSeconds() > c_WeaponCDTime)
    {
        c_LastWeaponUseTime = sf::seconds(c_WeaponCDTime);
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

void Player::setShortCut(const Item * item, sf::Keyboard::Key key)
{
    if(c_Bag->findItem(item) >= 0)
    {
        c_Shortcuts[key] = item;
    }
}

bool Player::canUseItem(const Item * item)
{
    return c_CanUseItem[item->getItemId()];
}

const Item * Player::getItemShortCut(sf::Keyboard::Key key)
{
    int16_t itemIndex = c_Bag->findItem(c_Shortcuts[key]);
    if(itemIndex != -1)
        return c_Bag->getItem(itemIndex);
    return NULL;
}
int16_t Player::getItemIndexShortCut(sf::Keyboard::Key key)
{
    return c_Bag->findItem(c_Shortcuts[key]);
}


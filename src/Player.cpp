#include "Player.hpp"

Player::Player(const std::string & name, float maxLife, Direction dir, float speed, const sf::Vector2f & pos)
 : Character(PLAYER, name, maxLife, dir, speed, pos), c_WeaponEquipped(NULL), c_CanUseWeapon(true), c_WeaponCDTime(0)
{
    c_Sprite.setTexture(Entity::playerTextures);
    setDirection(c_Direction); // pour initialiser le sprite
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

void Player::draw(sf::RenderWindow & app, uint16_t cellSize)
{
    c_Sprite.setScale(cellSize / c_Sprite.getLocalBounds().width, cellSize / c_Sprite.getLocalBounds().height);
    c_Sprite.setPosition(cellSize * c_Position.x, cellSize * c_Position.y);
    app.draw(c_Sprite);
}

void Player::setDirection(Direction dir)
{
    if(dir == LEFT)
        c_Sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    else if(dir == RIGHT)
        c_Sprite.setTextureRect(sf::IntRect(128, 0, 64, 64));
    else if(dir == UP)
        c_Sprite.setTextureRect(sf::IntRect(192, 0, 64, 64));
    else
        c_Sprite.setTextureRect(sf::IntRect(64, 0, 64, 64));

    Character::setDirection(dir);
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

bool Player::canUseItem(const Item * item) const
{
    return c_CanUseItem.at(item->getItemId());
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


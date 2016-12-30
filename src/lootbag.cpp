#include "lootbag.hpp"

LootBag::LootBag(const sf::Vector2f & position) : Bag(0), Entity(LOOTBAG, position)
{
    c_Sprite.setTexture(Entity::lootbagTexture);
}
LootBag::LootBag(const std::vector< BagCell > & items, const sf::Vector2f & position) : Bag(items.size()),
Entity(LOOTBAG, position)
{
    c_Items = items;

    c_Sprite.setTexture(Entity::lootbagTexture);
}
LootBag::~LootBag()
{
}

bool LootBag::addItem(const Item * item, uint16_t nbr)
{
    ++c_NbrSlotsMax;
    return Bag::addItem(item, nbr);
}
void LootBag::removeItem(uint16_t i, uint16_t nbr)
{
    if(c_Items.size() > i)
        --c_NbrSlotsMax;
    Bag::removeItem(i, nbr);
}

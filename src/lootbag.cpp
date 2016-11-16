#include "lootbag.hpp"

LootBag::LootBag(const sf::Vector2f & position) : Bag(0), Entity(position)
{
    c_Shape.setFillColor(sf::Color(30, 30, 30));
}
LootBag::LootBag(const std::vector< BagCell > & items, const sf::Vector2f & position) : Bag(items.size()),
Entity(position)
{
    c_Items = items;

    c_Shape.setFillColor(sf::Color(30, 30, 30));
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

void LootBag::draw(sf::RenderWindow & app, uint16_t cellSize)
{
    c_Shape.setSize(sf::Vector2f(cellSize/4, cellSize/4));
    c_Shape.setPosition(c_Position.x * cellSize, c_Position.y * cellSize);
    app.draw(c_Shape);
}

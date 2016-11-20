#include "item.hpp"

HealPotion * Item::healPotion = new HealPotion();
WoodenSword * Item::woodenSword = new WoodenSword();
StoneSword * Item::stoneSword = new StoneSword();
LongSword * Item::longSword = new LongSword();
Grenade * Item::grenade = new Grenade();
Teeth * Item::teeth = new Teeth();
Key * Item::key = new Key();
Crap * Item::crap = new Crap();

ItemInfos::ItemInfos(ItemType t, uint16_t maxUR, uint16_t minUR)
 : c_ItemType(t), c_MaxUseRange(maxUR), c_MinUseRange(minUR)
{
}

Item::Item(ItemId itemId, const std::string & name, bool isUsable, const ItemInfos & infos )
: c_ItemId(itemId), c_Name(name), c_Infos(infos), c_IsUsable(isUsable)
{
}

std::list< Cell * > Item::getTargetableCells(const Map * m, const Living * liv, uint16_t c, uint16_t l) const
{
    return m->getCellsBetweenDistFromCell(c, l, c_Infos.getMaxUseRange(), c_Infos.getMinUseRange());
}
std::list< Cell * > WoodenSword::getTargetableCells(const Map * m, const Living * liv, uint16_t c, uint16_t l) const
{
    std::list< Cell* > cells;
    Direction d = liv->getDirection();
    Cell * cell = m->getCell(c - (d == LEFT) + (d == RIGHT), l -(d == UP) + (d == DOWN)); // Cell devant le living

    if(cell != NULL)
        cells.push_back(cell);

    return cells;
}

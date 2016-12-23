#include "item.hpp"

std::map< ItemId, Item *> Item::c_Items;

Item::Item(ItemId itemId, const std::string & name, ItemType t, uint16_t maxUR, uint16_t minUR, bool isUsable,
           bool directional, ZoneType zoneType, float cdTime)
: c_ItemId(itemId), c_Name(name), c_ItemType(t), c_MaxUseRange(maxUR), c_MinUseRange(minUR), c_IsUsable(isUsable),
c_Directional(directional), c_ZoneType(zoneType), c_CDTime(cdTime)
{
}

std::list< Cell * > Item::getTargetableCells(const Map * m, const Living * liv, uint16_t c, uint16_t l) const
{
    if(!c_Directional)
    {
        if(c_ZoneType == AOE)
            return m->getCellsBetweenDistFromCell(c, l, getMaxUseRange(), getMinUseRange());
        else if(c_ZoneType == LINE)
            return m->getCellsInLineBetweenDistFromCell(c, l, getMaxUseRange(), getMinUseRange());
    }
    else
    {
        if(c_ZoneType == AOE)
            return m->getCellsBetweenDistFromCellDirection(c, l, getMaxUseRange(), getMinUseRange(), liv->getDirection());
        else if(c_ZoneType == LINE)
            return m->getCellsInLineBetweenDistFromCellDirection(c, l, getMaxUseRange(), getMinUseRange(), liv->getDirection());
    }
}

void Item::initItems()
{
    c_Items[HEALPOTION] = new Item(HEALPOTION, "Heal Potion", CONSUMABLE, 1, 0, true, false, AOE, 0.2);
    c_Items[HEALPOTION]->addEffect(EffectStats(HEAL, 5, 0, 0));

    c_Items[WOODENSWORD] = new Item(WOODENSWORD, "WoodSword", EQUIPMENT, 1, 1, true, true, LINE, 0.5);
    c_Items[WOODENSWORD]->addEffect(EffectStats(DAMAGE, 5, 0, 0));

    c_Items[STONESWORD] = new Item(STONESWORD, "StoneSword", EQUIPMENT, 1, 1, true, true, LINE, 0.7);
    c_Items[STONESWORD]->addEffect(EffectStats(DAMAGE, 10, 0, 0));

    c_Items[LONGSWORD] = new Item(LONGSWORD, "LongSword", EQUIPMENT, 2, 1, true, true, LINE, 1.2);
    c_Items[LONGSWORD]->addEffect(EffectStats(DAMAGE, 10, 0, 0));

    c_Items[GRENADE] = new Item(GRENADE, "Grenade", CONSUMABLE, 3, 0, true, false, AOE, 1.0);
    c_Items[GRENADE]->addEffect(EffectStats(DAMAGE, 10, 2, 0));

    c_Items[KEY] = new Item(KEY, "Key", TOOL);

    c_Items[CRAP] = new Item(CRAP, "Crap", TOOL);

    c_Items[TEETH] = new Item(TEETH, "Teeth", TOOL);
}

const Item * Item::getItemFromId(ItemId id)
{
    return c_Items[id];
}

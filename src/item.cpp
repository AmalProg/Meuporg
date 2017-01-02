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
    c_Items[WOODENSWORD] = new Item(WOODENSWORD, "WoodSword", EQUIPMENT, 1, 1, true, true, LINE, 0.5);
    c_Items[WOODENSWORD]->addEffect(Effect(DAMAGE, 5, 0, 0));

    c_Items[STONESWORD] = new Item(STONESWORD, "StoneSword", EQUIPMENT, 1, 1, true, true, LINE, 0.7);
    c_Items[STONESWORD]->addEffect(Effect(DAMAGE, 10, 0, 0));

    c_Items[LONGSWORD] = new Item(LONGSWORD, "LongSword", EQUIPMENT, 2, 1, true, true, LINE, 1.2);
    c_Items[LONGSWORD]->addEffect(Effect(DAMAGE, 10, 0, 0));

    c_Items[HEALPOTION1] = new Item(HEALPOTION1, "Heal Potion 1", CONSUMABLE, 1, 0, true, false, AOE, 0.2);
    c_Items[HEALPOTION1]->addEffect(Effect(HEAL, 5, 0, 0));
    c_Items[HEALPOTION2] = new Item(HEALPOTION2, "Heal Potion 2", CONSUMABLE, 2, 0, true, false, AOE, 0.75);
    c_Items[HEALPOTION2]->addEffect(Effect(HEAL, 30, 0, 0));
    c_Items[HEALPOTION3] = new Item(HEALPOTION3, "Heal Potion 3", CONSUMABLE, 2, 0, true, false, AOE, 2.0);
    c_Items[HEALPOTION3]->addEffect(Effect(HEAL, 100, 1, 0));

    c_Items[DAMAGEPOTION1] = new Item(DAMAGEPOTION1, "Damage Potion 1", CONSUMABLE, 3, 0, true, false, AOE, 5.0);
    c_Items[DAMAGEPOTION1]->addEffect(Effect(DAMAGE, 10, 1, 0));
    c_Items[DAMAGEPOTION2] = new Item(DAMAGEPOTION2, "Damage Potion 2", CONSUMABLE, 4, 0, true, false, AOE, 5.0);
    c_Items[DAMAGEPOTION2]->addEffect(Effect(DAMAGE, 50, 1, 0));
    c_Items[DAMAGEPOTION3] = new Item(DAMAGEPOTION3, "Damage Potion 3", CONSUMABLE, 4, 0, true, false, AOE, 5.0);
    c_Items[DAMAGEPOTION3]->addEffect(Effect(DAMAGE, 100, 2, 0));

    c_Items[KEY] = new Item(KEY, "Key", TOOL);
    c_Items[BONE] = new Item(BONE, "Bone", TOOL);
    c_Items[TEETH] = new Item(TEETH, "Teeth", TOOL);
}

const Item * Item::getItemFromId(ItemId id)
{
    return c_Items[id];
}

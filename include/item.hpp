#ifndef ITEM_HPP
#define ITEM_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
class Cell;
#include "Map.hpp"
class Map;
#include "living.hpp"

enum EffectType{HEAL, DAMAGE, DEFENSE, ATTACK};
class EffectStats // les stats d'un effet en particulier
{
public:
    EffectStats(EffectType t, float v, uint16_t maxR, uint16_t minR)
    : type(t), value(v), maxEffectRange(maxR), minEffectRange(minR) {}

    EffectType type; // type de l'effet
    float value; // valeur de l'effet
    uint16_t maxEffectRange; // range d'application de l'effet max
    uint16_t minEffectRange; // range d'application de l'effet min
};

enum ItemType{CONSUMABLE, EQUIPMENT, TOOL};
class ItemInfos // infos sur un item
{
    public:
        ItemInfos(ItemType t = CONSUMABLE, uint16_t maxUR = 0, uint16_t minUR = 0);

        void addEffect(const EffectStats & effect) {c_Effects.push_back(effect);}

        void setItemType(const ItemType & type) {c_ItemType = type;}
        void setMaxUseRange(uint16_t maxi) {c_MaxUseRange = maxi;}
        void setMinUseRange(uint16_t mini) {c_MinUseRange = mini;}

        const EffectStats & getEffect(uint16_t i) const {return c_Effects[i];}
        uint16_t getNbrEffect() const {return c_Effects.size();}
        ItemType getItemType() const {return c_ItemType;}
        uint16_t getMaxUseRange() const {return c_MaxUseRange;}
        uint16_t getMinUseRange() const {return c_MinUseRange;}

    private:
        std::vector< EffectStats > c_Effects; // les stats des différdnts effets
        ItemType c_ItemType;
        uint16_t c_MaxUseRange; // range d'utilisation max
        uint16_t c_MinUseRange; // range d'utilisation min
};

class HealPotion;
class WoodenSword;
class StoneSword;
class LongSword;
class Grenade;
class Teeth;
class Key;
class Crap;

enum ItemId {HEALPOTION = 0, WOODENSWORD, STONESWORD, LONGSWORD, GRENADE, TEETH, KEY, CRAP};

class Item
{
    public:
        Item(ItemId itemId, const std::string & name = "unknow", bool isUsable = true, const ItemInfos & infos = ItemInfos());
        virtual ~Item() {};

        void draw(sf::RenderWindow * app) {};

        void setItemInfos(const ItemInfos & infos) { c_Infos = infos; }
        void setIsUsable(bool isUsable) { c_IsUsable = isUsable; }

        ItemId getItemId() const { return c_ItemId; }
        const std::string & getName() const { return c_Name; }
        const ItemInfos & getInfos() const {return c_Infos;}
        virtual std::list< Cell * > getTargetableCells(const Map * m, const Living * liv, uint16_t c, uint16_t l) const;
        bool isUsable() const { return c_IsUsable; }

        static HealPotion * healPotion;
        static WoodenSword * woodenSword;
        static StoneSword * stoneSword;
        static LongSword * longSword;
        static Grenade * grenade;
        static Teeth * teeth;
        static Key * key;
        static Crap * crap;

    protected:
        ItemId c_ItemId;
        std::string c_Name;
        ItemInfos c_Infos;
        bool c_IsUsable;
};

class HealPotion : public Item {
    public:
        HealPotion() : Item(HEALPOTION, "Heal Potion", true, ItemInfos(CONSUMABLE, 1, 0)) {
            c_Infos.addEffect(EffectStats(HEAL, 5, 0, 0)); }
};

class WoodenSword : public Item {
    public:
        WoodenSword() : Item(WOODENSWORD, "WoodenSword", true, ItemInfos(EQUIPMENT, 1, 1)) {
            c_Infos.addEffect(EffectStats(DAMAGE, 5, 0, 0)); }

        std::list< Cell * > getTargetableCells(const Map * m, const Living * liv, uint16_t c, uint16_t l) const;
};

class StoneSword : public Item {
    public:
        StoneSword() : Item(STONESWORD, "StoneSword", true, ItemInfos(EQUIPMENT, 1, 1)) {
            c_Infos.addEffect(EffectStats(DAMAGE, 10, 0, 0)); }
};

class LongSword : public Item {
    public:
        LongSword() : Item(LONGSWORD, "LongSword", true, ItemInfos(EQUIPMENT, 2, 1)) {
            c_Infos.addEffect(EffectStats(DAMAGE, 20, 0, 0)); }
};

class Grenade : public Item {
    public:
        Grenade() : Item(GRENADE, "Grenade", true, ItemInfos(CONSUMABLE, 3, 0)) {
            c_Infos.addEffect(EffectStats(DAMAGE, 10, 2, 0)); }
};

class Teeth : public Item {
    public:
        Teeth() : Item(TEETH, "Teeth", false, ItemType(TOOL)) {
            c_Infos.addEffect(EffectStats(HEAL, 0, 0, 0)); }
};

class Key : public Item {
    public:
        Key() : Item(KEY, "Key", false, ItemType(TOOL)) {}
};

class Crap : public Item {
    public:
        Crap() : Item(CRAP, "Crap", false, ItemType(TOOL)) {}
};

#endif // ITEM_HPP

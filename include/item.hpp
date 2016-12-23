#ifndef ITEM_HPP
#define ITEM_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
class Cell;
#include "Map.hpp"
class Living;

enum EffectType{HEAL, DAMAGE};
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

enum ItemType {CONSUMABLE, EQUIPMENT, TOOL};
enum ItemId {HEALPOTION = 0, WOODENSWORD, STONESWORD, LONGSWORD, GRENADE, TEETH, KEY, CRAP};
enum ZoneType {LINE = 0, AOE};

class Item
{
    public:
        Item(ItemId itemId, const std::string & name = "unknow", ItemType t = CONSUMABLE, uint16_t maxUR = 0, uint16_t minUR = 0,
             bool isUsable = false, bool directional = false, ZoneType zoneType = AOE, float cdTime = 0.0);
        virtual ~Item() {};

        void draw(sf::RenderWindow * app) {};

        void addEffect(const EffectStats & effect) {c_Effects.push_back(effect);}

        void setIsUsable(bool isUsable) { c_IsUsable = isUsable; }
        void setDirectional(bool directional) { c_Directional = directional;}
        void setZoneTyppe(ZoneType zoneType) { c_ZoneType = zoneType; }
        void setItemType(const ItemType & type) {c_ItemType = type;}
        void setMaxUseRange(uint16_t maxi) {c_MaxUseRange = maxi;}
        void setMinUseRange(uint16_t mini) {c_MinUseRange = mini;}
        void setCDTime(float cdTime) { c_CDTime = cdTime; }

        ItemId getItemId() const { return c_ItemId; }
        const std::string & getName() const { return c_Name; }
        virtual std::list< Cell * > getTargetableCells(const Map * m, const Living * liv, uint16_t c, uint16_t l) const;
        bool isUsable() const { return c_IsUsable; }
        const EffectStats & getEffect(uint16_t i) const {return c_Effects[i];}
        uint16_t getNbrEffect() const {return c_Effects.size();}
        ItemType getItemType() const {return c_ItemType;}
        uint16_t getMaxUseRange() const {return c_MaxUseRange;}
        uint16_t getMinUseRange() const {return c_MinUseRange;}
        float getCDTime() const { return c_CDTime; }

        static const Item * getItemFromId(ItemId id);
        static void initItems();

    private:
        static std::map< ItemId, Item *> c_Items;

    protected:
        ItemId c_ItemId;
        std::string c_Name;
        bool c_IsUsable;
        bool c_Directional;
        ZoneType c_ZoneType;

        std::vector< EffectStats > c_Effects; // les stats des différents effets
        ItemType c_ItemType;
        uint16_t c_MaxUseRange; // range d'utilisation max
        uint16_t c_MinUseRange; // range d'utilisation min

        float c_CDTime;
};

#endif // ITEM_HPP

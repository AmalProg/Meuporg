#ifndef ITEM_HPP
#define ITEM_HPP

enum ItemId {WOODENSWORD = 0, STONESWORD, LONGSWORD, HEALPOTION1, HEALPOTION2, HEALPOTION3
, DAMAGEPOTION1, DAMAGEPOTION2, DAMAGEPOTION3,  TEETH, KEY, BONE};
// putain de problèmes d'inclusion multiples donc je le place là

#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
class Cell;
#include "Map.hpp"
#include "effect.hpp"
class Living;

enum ItemType {CONSUMABLE, EQUIPMENT, TOOL};
enum ZoneType {LINE = 0, AOE};

class Item
{
    public:
        Item(ItemId itemId, const std::string & name = "unknow", ItemType t = CONSUMABLE, uint16_t maxUR = 0, uint16_t minUR = 0,
             bool isUsable = false, bool directional = false, ZoneType zoneType = AOE, float cdTime = 0.0);
        virtual ~Item() {};

        void draw(sf::RenderWindow * app) {};

        void addEffect(const Effect & effect) {c_Effects.push_back(effect);}

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
        const Effect & getEffect(uint16_t i) const {return c_Effects[i];}
        uint16_t getNbrEffect() const {return c_Effects.size();}
        ItemType getItemType() const {return c_ItemType;}
        uint16_t getMaxUseRange() const {return c_MaxUseRange;}
        uint16_t getMinUseRange() const {return c_MinUseRange;}
        float getCDTime() const { return c_CDTime; }
        bool isDirectional() const { return c_Directional; }
        ZoneType getZoneType() const { return c_ZoneType; }

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

        std::vector< Effect > c_Effects; // les stats des différents effets
        ItemType c_ItemType;
        uint16_t c_MaxUseRange; // range d'utilisation max
        uint16_t c_MinUseRange; // range d'utilisation min

        float c_CDTime;
};

#endif // ITEM_HPP

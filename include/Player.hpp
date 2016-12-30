#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include "Character.hpp"
#include "item.hpp"

#define NBRSLOT 4

class Player : public Character
{
    public:
        Player(const std::string & name = "unknow", float maxLife = 100, Direction dir = DOWN, float speed = 10.0,
               const sf::Vector2f & pos = sf::Vector2f(0, 0));

        bool takeItem(const Item * item, uint16_t nbr); // rammasse un 'nbr' d''item'
        void update(const sf::Time & elapsed);

        bool equip(const Item * i);
        void removeItem(const Item * item, uint16_t nbr);
        void weaponUsed() { c_CanUseWeapon = false; c_LastWeaponUseTime = sf::Time::Zero; }
        void itemUsed(const Item * item);

        void setEquippedItem(const Item * item, uint16_t i);

        const Item * getEquippedItem(uint16_t i) const;
        int16_t getBagIndexOfEquippedItem(uint16_t i) const;
        const Item * getWeaponEquipped() const { return c_WeaponEquipped; }
        bool canUseWeapon() const { return c_CanUseWeapon; }
        bool canUseItem(const Item * item) const;
        uint16_t getNbrMaxEquippedItems() const { return c_MaxEquippedItem; }
        float getItemActualCdTime(const Item * item) const;
        float getWeaponActualCdTime() const;

    private:
        std::vector< const Item * > c_EquippedItems;
        uint16_t c_MaxEquippedItem;
        const Item * c_WeaponEquipped;
        sf::Time c_LastWeaponUseTime;
        bool c_CanUseWeapon;
        float c_WeaponCDTime;
        std::map< ItemId, sf::Time > c_LastItemUseTimes; // temps depuis la dernière utilisation
        std::map< ItemId, bool > c_CanUseItem;
        std::map< ItemId, float > c_ItemsCDTime;
};

#endif // PLAYER_HPP

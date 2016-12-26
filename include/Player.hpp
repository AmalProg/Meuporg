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
        Player(const std::string & name = "unknow", float maxLife = 100, Direction dir = DOWN, float speed = 3.f,
               const sf::Vector2f & pos = sf::Vector2f(0, 0));

        bool takeItem(const Item * item, uint16_t nbr); // rammasse un 'nbr' d''item'
        void update(const sf::Time & elapsed);

        bool equip(const Item * i);
        void removeItem(const Item * item, uint16_t nbr);
        void weaponUsed() { c_CanUseWeapon = false; c_LastWeaponUseTime = sf::Time::Zero; }
        void itemUsed(const Item * item);

        void setShortCut(const Item * item, sf::Keyboard::Key key);

        const Item * getItemShortCut(sf::Keyboard::Key key);
        int16_t getItemIndexShortCut(sf::Keyboard::Key key);
        const Item * getWeaponEquipped() const { return c_WeaponEquipped; }
        bool canUseWeapon() const { return c_CanUseWeapon; }
        bool canUseItem(const Item * item) const;

    private:
        std::map< sf::Keyboard::Key, const Item * > c_Shortcuts; //
        const Item * c_WeaponEquipped;

        sf::Time c_LastWeaponUseTime;
        bool c_CanUseWeapon;
        float c_WeaponCDTime;
        std::map< ItemId, sf::Time > c_LastItemUseTimes;
        std::map< ItemId, bool > c_CanUseItem;
        std::map< ItemId, float > c_ItemsCDTime;
};

#endif // PLAYER_HPP

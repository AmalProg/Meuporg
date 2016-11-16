#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "item.hpp"
#include "loottable.hpp"
#include "Player.hpp"
class Map;

enum AggroState {AGGRESIVE = 0, PASSIVE};

class Monster : public Living
{
    public:
        Monster(const std::string & name = "unknow", AggroState aggrostate = PASSIVE, uint16_t aggroDist = 5,
                float maxLife = 100, Direction dir = DOWN, const sf::Vector2f & pos = sf::Vector2f(0, 0),
                const sf::Color & color = sf::Color::Black, float speed = 1.5, float delayAtkTime = 1.5);

        virtual void speakAction(); // activé si l'on essaye de "parler" a l'obstacle
        virtual void touchAction(); // activé lorsqu'on essaye de marcher sur l'obstacle
        virtual void realTimeAction(Map * m, Player * p); // activé dans diverses cas en fonction de positions de certains objets etc ...
        virtual void attack(Map * m, Player * p) = 0;

        void setAggroState(AggroState aggroState) { c_AggroState = aggroState; }
        void setAggroDist(uint16_t aggroDist) { c_AggroDist = aggroDist; }
        void setLootTable(const LootTable & lootTable);

        AggroState getAggroState() const { return c_AggroState; }
        uint16_t getAggroDist() const { return c_AggroDist; }
        const std::vector< BagCell > & getLoots() const { return c_Loots; }

    protected:
        AggroState c_AggroState;
        uint16_t c_AggroDist;
        bool c_IsInAggro;
        sf::Clock c_LastAggroTime;

        sf::Clock c_LastAtkTime;
        float c_DelayAtkTime;

        LootTable c_LootTable;
        std::vector< BagCell > c_Loots;
};

class Sheep : public Monster
{
    public:
        Sheep(const std::string & name = "Sheep", AggroState aggroState = PASSIVE, uint16_t aggroDist = 3,
            float maxLife = 5, Direction dir = LEFT, const sf::Vector2f & pos = sf::Vector2f(0, 0),
             const sf::Color & color = sf::Color::White, float speed = 1.2);

        void attack(Map * m, Player * p);
};

class Wolf : public Monster
{
    public:
        Wolf(const std::string & name = "Sheep", AggroState aggroState = AGGRESIVE, uint16_t aggroDist = 5,
            float maxLife = 10, Direction dir = LEFT, const sf::Vector2f & pos = sf::Vector2f(0, 0),
             const sf::Color & color = sf::Color(50, 50, 50), float speed = 2.0);

        void attack(Map * m, Player * p);
};

#endif // MONSTER_HPP

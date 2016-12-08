#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include "Entity.hpp"
class Living;
#include "Player.hpp"
#include "bag.hpp"
#include "item.hpp"
class Map;

class Obstacle : public Entity
{
    public:
        Obstacle(EntityTypeId typeId, const sf::Vector2f & pos = sf::Vector2f(0, 0), bool walkable = false, bool visionBlocking = true,
                 bool attackBlocking = false, bool filler = false, bool cover = false);
        virtual ~Obstacle() {};

        void setWalkable(bool w) { c_Walkable = w; }
        void setVisionBlocking(bool vB) { c_VisionBlocking = vB; }
        void setAttackBlocking(bool aB) { c_AttackBlocking = aB; }

        bool isWalkable() const { return c_Walkable; }
        bool isCover() const { return c_IsCover; }
        bool isFiller() const { return c_IsFiller; }
        bool isVisionBlocking() const { return c_VisionBlocking; }
        bool isAttackBlocking() const { return c_AttackBlocking; }

        virtual void draw(sf::RenderWindow & app, uint16_t cellSize);

        virtual void realTimeAction(Map * m, Player * p) {} // activé dans diverses cas en fonction de positions de certains objets etc ...
        virtual void speakAction(Map * mape, Player * p) {} // activé si l'on essaye de "parler" a l'obstacle
        virtual void touchAction(Map * mape, Player * p) {} // activé lorsqu'on essaye de marcher sur l'obstacle
        virtual void walkAction(Map * mape, Living * l) {} // activé lorsqu'on marche sur l'obstacle
        virtual void firstStepAction(Map * mape, Living * l) {} // activé lorsqu'on pose notre premier pas sur la cell
        virtual void lastStepAction(Map * mape, Living * l) {} // activé lorsqu'on sort juste de la cell

    protected:
        sf::RectangleShape c_Shape;

        bool c_Walkable; // peut on marcher dessus ?
        bool c_IsCover; // recouvre le sol de la cell
        bool c_IsFiller; // prend toute la place de la cell
        bool c_VisionBlocking; // bloque-t-il la vision
        bool c_AttackBlocking; // peut on tirer à travers ?
};

class Grass : public Obstacle
{
public:
    Grass(const sf::Vector2f & pos = sf::Vector2f(0, 0)) : Obstacle(GRASS, pos, true, false, false, false, true)
    {
        c_Shape.setFillColor(sf::Color::Green);
    }
};

class Water : public Obstacle
{
public:
    Water(const sf::Vector2f & pos = sf::Vector2f(0, 0)) : Obstacle(WATER, pos, false, false, true, false, true)
    {
        c_Shape.setFillColor(sf::Color(0, 0, 175));
    }
};

class Sand : public Obstacle
{
public:
    Sand(const sf::Vector2f & pos = sf::Vector2f(0, 0)) : Obstacle(SAND, pos, true, false, false, false, true)
    {
        c_Shape.setFillColor(sf::Color(180, 180, 40));
    }

    virtual void firstStepAction(Map * mape, Living * l) { l->setSpeed(l->getSpeed() * 0.5); }
    virtual void lastStepAction(Map * mape, Living * l) { l->setSpeed(l->getSpeed() * 2); }
};

class Soil : public Obstacle
{
public:
    Soil(const sf::Vector2f & pos = sf::Vector2f(0, 0)) : Obstacle(SOIL, pos, true, false, false, false, true)
    {
        c_Shape.setFillColor(sf::Color(50, 25, 0));
    }
};

class Fire : public Obstacle
{
public:
    Fire(float damageTickTime = 0.5, uint16_t damageDealt = 5, const sf::Vector2f & pos = sf::Vector2f(0, 0))
    : Obstacle(FIRE, pos, true, false, false, false, true), c_DamageTickTime(damageTickTime),
    c_DamagePerTick(damageDealt), c_FirstStateOfFire(true), c_Ticking(false)
    {
        c_Shape.setFillColor(sf::Color(200, 100, 0));
    }

    void update(const sf::Time & elapsed) { c_SwitchTime += elapsed; c_LastTickTime += elapsed; }

    virtual void realTimeAction(Map * m, Player * p);
    virtual void walkAction(Map * mape, Living * l);

    float getDamageTickTime() const { return c_DamageTickTime; }
    uint16_t getDamagePerTick() const { return c_DamagePerTick; }

private:
    sf::Time c_SwitchTime;
    bool c_FirstStateOfFire;

    sf::Time c_LastTickTime; // temps écoulé depuis le denrier tick
    float c_DamageTickTime; // temps entre chaque tick de dégats
    bool c_Ticking;

    uint16_t c_DamagePerTick;
};

class Rock : public Obstacle
{
public:
    Rock(const sf::Vector2f & pos = sf::Vector2f(0, 0)) : Obstacle(ROCK, pos, false, true, true, true, false)
    {
        c_Shape.setFillColor(sf::Color(120, 120 ,120));
    }
};

class Door : public Obstacle
{
public:
    Door(bool isOpen = false, EntityTypeId typeId = DOOR, const sf::Vector2f & pos = sf::Vector2f(0, 0));

    virtual void speakAction(Map * mape, Player * p);
    virtual void touchAction(Map * mape, Player * p);

    bool isOpen() const { return c_IsOpen; }
    bool setOpen(bool isOpen) { c_IsOpen = isOpen; }

private:
    bool c_IsOpen;
};

class LockedDoor : public Door
{
public:
    LockedDoor(bool isLocked = true, bool isOpen = false, const sf::Vector2f & pos = sf::Vector2f(0, 0))
    : Door(isOpen, LOCKEDDOOR, pos), c_IsLocked(isLocked)
    {}

    virtual void touchAction(Map * mape, Player * p);
    virtual void speakAction(Map * mape, Player * p);

    bool isLocked() const { return c_IsLocked; }
    void setLock(bool isLocked) { c_IsLocked = isLocked; }

private:
    bool c_IsLocked;
};

class Stairs : public Obstacle
{
public:
    Stairs(uint32_t linkedMapId = 0, bool isRising = true, const sf::Vector2f & pos = sf::Vector2f(0, 0))
    : Obstacle(STAIRS, pos, true, false, false, false, false), c_LinkedMapId(linkedMapId), c_IsActivated(false)
    , c_IsRising(isRising)
    {
    }

    virtual void firstStepAction(Map * mape, Living * l)
    {
        if(l->getEntityTypeId() == PLAYER)
            c_IsActivated = true;
        else
            c_IsActivated = false;
    }

    bool isActivated() const { return c_IsActivated; }
    bool isRising() const { return c_IsRising; }
    uint32_t getLinkedMapId() const { return c_LinkedMapId; }

    void setActivated(bool a) { c_IsActivated = a; }
    void setLinkedMapId(uint32_t mapId) { c_LinkedMapId = mapId; }
    void setRising(bool isRising) { c_IsRising = isRising; }

private:
    uint32_t c_LinkedMapId;
    bool c_IsActivated;

    bool c_IsRising;
};

#endif // OBSTACLE_H

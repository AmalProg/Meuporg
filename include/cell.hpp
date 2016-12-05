#ifndef CELL_HPP
#define CELL_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include "living.hpp"
#include "Obstacle.hpp"
class Obstacle;
class LootBag;
class Map;

class Cell
{
public:
    Cell(uint16_t col, uint16_t lin);

    void setLiving(Living * p);
    void setCover(Obstacle * c);
    void setFiller(Obstacle * f);
    void setStairs(Obstacle * s);
    void removeObstacle(const Obstacle *o);
    void addLootBag(LootBag * lB);
    void removeLootBag(const LootBag * lB);

    Living * getLiving() const { return c_Living; }
    Obstacle * getCover() const { return c_Cover; }
    Obstacle * getFiller() const { return c_Filler; }
    Obstacle * getStairs() const { return c_Stairs; }
    const std::list< Obstacle * > & getObstacles() const { return c_Obstacles; }
    const std::list< LootBag * > & getLootBags() const { return c_LootBags; }
    uint16_t getC() const { return c; }
    uint16_t getL() const { return l; }
    uint16_t getNbrEntity() const { return c_Obstacles.size() + c_LootBags.size() + (c_Living!=NULL); }
    bool isEmpty() const { return c_IsEmpty; }
    bool isCovered() const { return c_IsCovered; }
    bool isFilled() const { return c_IsFilled; }
    bool gotStairs() const { return c_GotStairs; }
    bool isWalkable() const { return c_Walkable; }

    void stateTest(); // doit être appelé à chaque tour de boucle

private:
    Living * c_Living;
    Obstacle * c_Cover;
    Obstacle * c_Filler;
    Obstacle * c_Stairs;
    std::list< Obstacle * > c_Obstacles;
    std::list< LootBag * > c_LootBags;
    uint16_t c;
    uint16_t l;
    bool c_IsEmpty;
    bool c_IsCovered;
    bool c_IsFilled;
    bool c_GotStairs;
    bool c_Walkable;
};

#endif // CELL_HPP

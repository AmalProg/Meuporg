#ifndef LOOTTABLE_HPP
#define LOOTTABLE_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
class Item;
#include "bag.hpp"

typedef struct lootInfos
{
    const Item * item;
    uint16_t nbr;
    float prob;
} lootInfos;

class LootTable
{
    public:
        LootTable();

        void addItem(const Item * item, uint16_t nbr, float prob);

        const std::list< lootInfos > & getLootsInfos() const { return c_Loots; }
        std::vector< BagCell > getLoots() const;

    private:
        std::list< lootInfos > c_Loots;
};

#endif // LOOTTABLE_HPP

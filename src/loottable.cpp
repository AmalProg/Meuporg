#include "loottable.hpp"

LootTable::LootTable()
{
}

void LootTable::addItem(const Item * item, uint16_t nbr, float prob)
{
    lootInfos lInfos;
    lInfos.item = item;
    lInfos.nbr = nbr;
    if(prob < 0)
        prob = 0;
    else if(prob > 100)
        prob = 100;
    lInfos.prob = prob;

    c_Loots.push_back(lInfos);
}

std::vector< BagCell > LootTable::getLoots() const
{
    float random;
    uint16_t nbr;

    std::vector< BagCell > loots;
    for(std::list< lootInfos >::const_iterator it = c_Loots.begin(); it != c_Loots.end(); ++it)
    {
        nbr = 0;

        for(uint16_t i = 0; i < (*it).nbr; ++i)
        {
            random = rand() % 101;
            if(random <= (*it).prob)
                ++nbr;
        }

        if(nbr != 0)
        {
            BagCell bC;
            bC.item = (*it).item;
            bC.nbr = nbr;
            loots.push_back(bC);
        }
    }

    return loots;
}

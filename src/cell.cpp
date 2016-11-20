#include "cell.hpp"

Cell::Cell(uint16_t col, uint16_t lin)
 : c_Living(NULL), c_Cover(NULL), c_Filler(NULL), c_Stairs(NULL), c(col), l(lin),  c_IsEmpty(true)
 , c_IsCovered(false), c_IsFilled(false), c_GotStairs(false), c_Walkable(true)
{
}

void Cell::setLiving(Living * p)
{
    c_Living = p;
}

void Cell::setCover(Obstacle * c)
{
    if(c_Cover != NULL)
        removeObstacle(c_Cover);
    c_Cover = c;
    c_Obstacles.push_back(c_Cover);
}
void Cell::setFiller(Obstacle * f)
{
    if(c_Filler != NULL)
        removeObstacle(c_Filler);
    c_Filler = f;
    c_Obstacles.push_back(c_Filler);
}
void Cell::setStairs(Obstacle * s)
{
    if(c_Stairs != NULL)
        removeObstacle(c_Stairs);
    c_Stairs = s;
    c_Obstacles.push_back(c_Stairs);
}

void Cell::removeObstacle(const Obstacle * o)
{
    c_Obstacles.erase(std::find(c_Obstacles.begin(), c_Obstacles.end(), o));
}

void Cell::addLootBag(LootBag * lB)
{
    c_LootBags.push_back(lB);
}
void Cell::removeLootBag(const LootBag * lB)
{
    c_LootBags.erase(std::find(c_LootBags.begin(), c_LootBags.end(), lB));
}

void Cell::stateTest()
{
    if(c_Living != NULL)
    {
        c_Walkable = false;
        c_IsEmpty = false;
    }
    else
    {
        c_IsEmpty = true;
        c_Walkable = true;
        c_IsCovered = false;
        c_IsFilled = false;
        for(std::list< Obstacle * >::iterator it = c_Obstacles.begin(); it != c_Obstacles.end(); ++it)
        {
            c_IsEmpty = false;
            if(!(*it)->isWalkable())
                c_Walkable = false;
        }

        if(c_Filler != NULL)
            c_IsFilled = true;
        if(c_Cover != NULL)
            c_IsCovered = true;
        if(c_Stairs != NULL)
            c_GotStairs = true;

        if(c_LootBags.size() != 0)
            c_IsEmpty = false;
    }
}

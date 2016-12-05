#include "cell.hpp"

Cell::Cell(uint16_t col, uint16_t lin)
 : c_Living(NULL), c_Cover(NULL), c_Filler(NULL), c_Stairs(NULL), c(col), l(lin),  c_IsEmpty(true)
 , c_IsCovered(false), c_IsFilled(false), c_GotStairs(false), c_Walkable(true)
{
}

void Cell::setLiving(Living * p)
{
    c_Living = p;

    if(p != NULL)
    {
        c_Walkable = false;
        c_IsEmpty = false;
    }
    else
    {
        stateTest();
    }
}

void Cell::setCover(Obstacle * c)
{
    if(c_Cover != NULL)
        removeObstacle(c_Cover);
    if(c != NULL)
    {
        c_Cover = c;
        c_Obstacles.push_back(c_Cover);

        c_IsCovered = true;
        c_IsEmpty = false;
        if(!c->isWalkable())
            c_Walkable = false;
    }
    else
    {
        c_Cover = NULL;

        c_IsCovered = false;
    }
}
void Cell::setFiller(Obstacle * f)
{
    if(c_Filler != NULL)
        removeObstacle(c_Filler);
    if(f != NULL)
    {
        c_Filler = f;
        c_Obstacles.push_back(c_Filler);

        c_IsFilled = true;
        c_IsEmpty = false;
        if(!f->isWalkable())
            c_Walkable = false;
    }
    else
    {
        c_Filler = NULL;

        c_IsFilled = false;
    }
}
void Cell::setStairs(Obstacle * s)
{
    if(c_Stairs != NULL)
        removeObstacle(c_Stairs);
    if(s != NULL)
    {
        c_Stairs = s;
        c_Obstacles.push_back(c_Stairs);

        c_GotStairs = true;
        c_IsEmpty = false;
        if(!s->isWalkable())
            c_Walkable = false;
    }
    else
    {
        c_Stairs = NULL;

        c_GotStairs = false;
    }
}

void Cell::removeObstacle(const Obstacle * o)
{
    c_Obstacles.erase(std::find(c_Obstacles.begin(), c_Obstacles.end(), o));
    stateTest();
}

void Cell::addLootBag(LootBag * lB)
{
    c_LootBags.push_back(lB);
    stateTest();
}
void Cell::removeLootBag(const LootBag * lB)
{
    c_LootBags.erase(std::find(c_LootBags.begin(), c_LootBags.end(), lB));
    stateTest();
}

void Cell::stateTest()
{
    c_Walkable = true;
    c_IsEmpty = true;

    for(std::list< Obstacle * >::iterator it = c_Obstacles.begin(); it != c_Obstacles.end(); ++it)
    {
        c_IsEmpty = false;
        if(!(*it)->isWalkable())
        {
            c_Walkable = false;
            break;
        }
    }

    if(c_LootBags.size() != 0)
        c_IsEmpty = false;
}

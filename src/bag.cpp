#include "bag.hpp"

Bag::Bag(int nbrSlotsMax) : c_NbrSlotsMax(nbrSlotsMax)
{

}

bool Bag::addItem(const Item * item, uint16_t nbr)
{
    for(uint16_t i = 0; i < c_Items.size(); i++)
    {
        if(item == c_Items[i].item)
        {
            c_Items[i].nbr += nbr;
            return true;
        }
    }

    if(c_Items.size() < c_NbrSlotsMax)
    {
        BagCell bagCell;
        bagCell.item = item;
        bagCell.nbr = nbr;

        c_Items.push_back(bagCell);
        return true;
    }

    return false;
}

void Bag::removeItem(uint16_t i, uint16_t nbr)
{
    if(c_Items.size() > i)
    {
        if((c_Items[i].nbr - nbr) <= 0)
        { // si il ne reste plus d'items
            c_Items[i].item = NULL;
            c_Items[i].nbr = 0;
            c_Items.erase(c_Items.begin() + i);
        }
        else
        {
            c_Items[i].nbr -= nbr;
        }
    }
}

int16_t Bag::findItem(const Item * item) const
{
    for(uint16_t i = 0; i < c_Items.size(); ++i)
    {
        if(c_Items[i].item == item)
            return i;
    }
    return -1;
}

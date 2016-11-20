#ifndef BAG_HPP
#define BAG_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
class Item;

typedef struct BagCell
{
    const Item * item;
    uint16_t nbr;
} BagCell;

class Bag
{
    public:
        Bag(int nbrSlotsMax = 24);
        ~Bag() {};
        const Item * operator[](uint16_t i) const {return c_Items[i].item;}

        virtual bool addItem(const Item * item, uint16_t nbr);
        virtual void removeItem(uint16_t i, uint16_t nbr);

        uint32_t getBalance() const { return c_Balance; }
        uint16_t getNbrItems() const {return c_Items.size();}
        const Item * getItem(uint16_t i) const {return c_Items[i].item;}
        uint16_t getNbrOfItem(uint16_t i) const {return c_Items[i].nbr;}
        int16_t findItem(const Item * item) const;

    protected:
        uint16_t c_NbrSlotsMax; // nombre de slots
        std::vector< BagCell > c_Items;
        uint32_t c_Balance; // argent
};

#endif // BAG_HPP

#ifndef MENU_HPP
#define MENU_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Map.hpp"
class LootBag;

class Menu
{
    public:
        Menu(sf::RenderWindow & a);

        void draw(const Map * m, const Player * p);
        void manage(sf::Event & event, const Player * p);

        void setShowingInventory();
        void setShowingCellChoice(int16_t i);
        void setShowingLootBag(LootBag * lB);

        int16_t getItemToDestroy() { int16_t tmp = c_ItemToDestroy; c_ItemToDestroy = -1;  return tmp; }
        int16_t getItemToUse() { int16_t tmp = c_ItemToUse; c_ItemToUse = -1; return tmp; }
        const Cell * getCellToFocus() { const Cell * tmp = c_CellToFocus; c_CellToFocus = NULL; return tmp; }
        int16_t getItemToShortCut() { int16_t tmp = c_ItemToShortCut; c_ItemToShortCut = -1; return tmp; }
        sf::Keyboard::Key getKeyToShortCut() { sf::Keyboard::Key tmp = c_KeyToShortCut; c_KeyToShortCut = sf::Keyboard::Unknown; return tmp; }
        BagCell getItemToTake() { BagCell tmp = c_ItemToTake; c_ItemToTake.item = NULL; c_ItemToTake.nbr = 0; return tmp; }

        bool isShowingInventory() const { return c_ShowingInventory; }
        bool isShowingItemMenu() const { return c_ShowingItemMenu; }
        bool isShowingShortCutChoice() const { return c_ShowingShortCutChoice; }
        bool isShowingCellChoice() const { return c_ShowingCellChoice; }
        bool isShowingLootBag() const { return c_ShowingLootBag; }

    private:
        void inventoryMenu(const Player * p);
        void itemMenu(const Player * p);
        void cellChoiceMenu(const Map * m, const Player * p);
        void shortCutMenu(const Map * m);
        void lootbagMenu();

    private:
        sf::RenderWindow & app;

        sf::Font arial;

        bool c_ShowingInventory;
        sf::Texture c_InventoryTexture;
        sf::Sprite c_InventorySprite;
        std::vector< sf::Text > c_InventoryTexts;
        uint16_t c_InventorySelected;
        uint16_t c_InventoryFirst;
        uint16_t c_InventoryNbrShown;
        uint16_t c_InventoryNbrItems;

        bool c_ShowingItemMenu;
        sf::Texture c_ItemMenuTexture;
        sf::Sprite c_ItemMenuSprite;
        sf::Text c_ItemMenuTexts[3];
        uint16_t c_ItemMenuSelected;
        int16_t c_ItemToDestroy;
        int16_t c_ItemToUse;

        bool c_ShowingCellChoice;
        std::list< Cell * > c_AffectedCells;
        Cell * c_CellChoiceSelected;
        Cell * c_CellToFocus;

        bool c_ShowingShortCutChoice;
        sf::Texture c_ShortCutTexture;
        sf::Sprite c_ShortCutSprite[NBRSLOT];
        int16_t c_ShortCutSelected;
        int16_t c_ItemToShortCut;
        sf::Keyboard::Key c_KeyToShortCut;

        bool c_ShowingLootBag;
        sf::Texture c_LootBagTexture;
        sf::Sprite c_LootBagSprite;
        LootBag * c_LootBag;
        std::vector< sf::Text > c_LootBagTexts;
        int16_t c_LootBagSelected;
        uint16_t c_LootBagFirst;
        uint16_t c_LootBagNbrShown;
        uint16_t c_LootBagNbrItems;
        BagCell c_ItemToTake;
};

#endif // MENU_HPP

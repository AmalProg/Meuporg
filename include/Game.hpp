#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "menu.hpp"
#include "Map.hpp"
#include "Item.hpp"
class Cell;
#include "RawText.hpp"
#include "ChoiceText.hpp"

class Game
{
    public:
        Game(sf::RenderWindow & a);
        ~Game();

        void loop();

    private:
        void save(const std::string & fileName);
        void load(const std::string & fileName);

        bool movePlayer(Player * p, Direction d);
        bool useItem(Player * p, const Item * item, uint16_t c, uint16_t l);
        void useWeapon(Player * p, const Item * item, const std::list<Cell *> & cellsF);

    private:
        void update(const sf::Time & elapsed);
        void eventManage();
        void walkEventTest();
        void actionEventTest();
        void realTimeEventManage();
        void deathManage();

        void genNextMap(Map * map, Map * lastMap, uint16_t lvl);

        void drawShortCuts();

    private:
        sf::RenderWindow & app;
        uint16_t c_NbrCellsToDraw; // nbr de cells � affiher sur la fen�tre

        std::vector< Map * > c_Maps; // liste de toutes les maps
        Map * c_Map; // map actuelle
        Player * c_Player;

        uint16_t c_ActualLevel;

        std::vector< sf::Keyboard::Key > c_ItemShortCuts;

        Menu c_Menu;
        RawText c_RawText;
        ChoiceText c_ChoiceText;

        sf::Clock c_GameClock;
        uint64_t c_NbrLoops;
        sf::Font c_Font;
        sf::Text c_FpsTxt;
};

#endif // GAME_HPP

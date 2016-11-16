#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "Map.hpp"
#include "cell.hpp"
#include "Living.hpp"
#include "Player.hpp"
#include "character.hpp"
#include "monster.hpp"
#include "RawText.hpp"
#include "ChoiceText.hpp"
#include "Obstacle.hpp"
#include "Item.hpp"
#include "menu.hpp"
#include "lootbag.hpp"

#include <sstream>

class Game
{
    public:
        Game(sf::RenderWindow & a);
        ~Game();

        void loop();

        //void setPlayer(Player * p) {c_Player = p;}
        bool movePlayer(Player * p, Direction d);
        bool useItem(Player * p, const Item * item, uint16_t c, uint16_t l);

    private:
        void eventManage();
        int32_t choiceTextEventManage();
        void rawTextEventManage();
        void walkEventTest();
        void actionEventTest();
        void realTimeEventManage();
        void deathManage();

        void genNextMap(Map * map, uint16_t lvl);

        void drawShortCuts();

    private:
        sf::RenderWindow & app;

        std::vector< Map * > c_Maps; // liste de toutes les maps
        Map * c_Map; // map actuelle
        Player * c_Player;

        uint16_t c_ActualLevel;

        sf::Keyboard::Key c_ShortCutKeys[NBRSLOT];

        Menu c_Menu;
        RawText c_RawText;
        ChoiceText c_ChoiceText;
};

#endif // GAME_HPP

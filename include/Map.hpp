#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <list>
#include "cell.hpp"
#include "Living.hpp"
class Obstacle;
class Stairs;
#include "monster.hpp"
class Monster;
#include "lootbag.hpp"
#include "GenInfo.hpp"
#include "Tree.hpp"

std::string nbrToString(float nbr);
typedef struct PathCell
{ // utilisé dans la recherche de chemin
    const Cell * cell;
    uint16_t cellDist;
} PathCell;

class Map
{
    public:
        Map(sf::RenderWindow & a);
        Map(uint16_t c, uint16_t l, sf::RenderWindow & a);
        ~Map();

        void moveMap(); // déplace la map en fonction du focus sur la vue
        bool addObstacle(Obstacle * o,  uint16_t c, uint16_t l);
        void removeObstacle(const Obstacle * o, uint16_t c, uint16_t l);
        bool addStairs(Stairs * s,  uint16_t c, uint16_t l);
        bool addMonster(Monster * m, uint16_t c, uint16_t l);
        void removeMonster(const Monster * m);
        bool addCharacter(Character * cha, uint16_t c, uint16_t l);
        void removeCharacter(const Character * cha);
        bool addLootBag(LootBag * lB,  uint16_t c, uint16_t l);
        void removeLootBag(const LootBag * lB, uint16_t c, uint16_t l);
        void draw();

        void generateMap(const GenInfo & genInfos);
        void save(std::ofstream & file);
        void load(std::ofstream & file);

        void moveLiving(Living * p, uint16_t c, uint16_t l); // déplace le joueur sur la case donnée
        void setFocus(Living * p);

        bool getLivingOnCell(Living * p, uint16_t c, uint16_t l); // retourne le Living de cette case
        bool getObstaclesOnCell(std::list< Obstacle * > os, uint16_t c, uint16_t l); // retourne l'obstacle de cette case
        uint16_t getNbrColumn() const {return c_Map.size();}
        uint16_t getNbrLine() const {return c_Map[0].size();}
        uint16_t getNbrTotalCells() const { return c_Map.size() * c_Map[0].size();}
        uint16_t getCellSize() const { return c_CellSize; }
        const std::list< Obstacle * > & getObstacles() const { return c_Obstacles; }
        const std::list< Stairs * > & getStairss() const { return c_Stairss; }
        const std::list< Living * > & getLivings() const { return c_Livings; }
        const std::list< Monster * > & getMonsters() const { return c_Monsters; }
        const std::list< Character * > & getCharacters() const { return c_Characters; }
        const std::list< LootBag * > & getLootBags() const { return c_LootBags; }

        Cell * getCell(uint16_t c, uint16_t l) const { if(c >= 0 && l >= 0 && c < getNbrColumn() && l < getNbrLine()) return c_Map[c][l]; return NULL; }
        float getDist(Cell * c1, Cell * c2) const;
        uint16_t getCellDist(Cell * c1, Cell * c2) const;
        bool lineOfSight(Cell * c1, Cell * c2) const;
        std::vector< Cell * > getPath(const Cell * startC, const Cell * endC, bool skipLivings, bool fullMap, uint16_t maxMoreSteps) const;
        uint16_t getPathLenght(const Cell * startC, const Cell * endC, bool skipLivings) const;
        std::list< Cell * > getCellsDistFromCell(uint16_t c, uint16_t l, uint16_t dist) const; // cellules a moins d'un certaine distance
        std::list< Cell * > getCellsBetweenDistFromCell(uint16_t c, uint16_t l, uint16_t distMax, int distMin) const;
        Cell * getRCell(uint16_t c, uint16_t l) const { if(c+1 < c_Map.size()) return c_Map[c+1][l]; return NULL; }
        Cell * getLCell(uint16_t c, uint16_t l) const { if(c-1 >= 0) return c_Map[c-1][l]; return NULL; }
        Cell * getUCell(uint16_t c, uint16_t l) const { if(l-1 >= 0) return c_Map[c][l-1]; return NULL; }
        Cell * getDCell(uint16_t c, uint16_t l) const { if(l+1 < c_Map[c].size()) return c_Map[c][l+1]; return NULL; }
        Cell * getRCell(const Cell * c) const;
        Cell * getLCell(const Cell * c) const;
        Cell * getUCell(const Cell * c) const;
        Cell * getDCell(const Cell * c) const;

    private:
        void removeLiving(uint16_t c, uint16_t l);

        void recursiveNeighbourDist(std::list< Cell * > & cells, Cell * c, int distMax, Cell * baseCell) const;
        void pathMap(std::vector < std::vector < int16_t > > * distMap, const std::vector< const Cell * > & actualCs, uint16_t actualDist, const Cell * targetC, bool skipLivings, bool fullMap) const;
        void recursivePath(std::vector < Cell * > * bestPath, std::vector < std::vector < int16_t > > * distMap, const Cell * startC, const Cell * endC, uint16_t maxMoreSteps) const;

        std::vector < Cell *> getCellsOnLineOfSight(Cell * c1, Cell * c2) const;

        bool addObstacleOnCell(EntityTypeId id, Cell * cell);
        bool addLivingOnCell(EntityTypeId id, Cell * cell);
        void expandEntity(EntityTypeId id, Cell * cell, float expandValue);
        void standardize(EntityTypeId id, uint16_t switchFloor, sf::Vector2i targetColumns, sf::Vector2i targetLines);

    private:
        std::vector< std::vector < Cell *> > c_Map; // Type et numero de chaque contenu

        std::list< Obstacle * > c_Obstacles;
        std::list< Stairs * > c_Stairss;
        std::list< Living * > c_Livings;
        std::list< Monster * > c_Monsters;
        std::list< Character * > c_Characters;
        std::list< LootBag * > c_LootBags;

        Living * c_Focus;
        sf::RenderWindow & app;
        sf::View c_View;

        float c_CellSize;
};

#endif // MAP_HPP

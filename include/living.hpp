#ifndef LIVING_HPP
#define LIVING_HPP

#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <list>
#include "Entity.hpp"
class Map;
class Player;

enum Direction{UP = 0, RIGHT, DOWN, LEFT};

class Living : public Entity
{
    public:
        Living(EntityTypeId typeId, const std::string & name = "unknow", float maxLife = 100, Direction dir = DOWN,
               const sf::Vector2f & pos = sf::Vector2f(0, 0), float speed = 1.5);
        virtual ~Living();

        virtual void heal(float h) {if(h < 0) return; c_Life += h; if(c_Life > c_MaxLife) c_Life = c_MaxLife;}
        virtual void takeDamages(float d) {if(d < 0) return; c_Life -= d; if(c_Life <= 0) { c_Life = 0; c_IsDead = true; }}

        virtual void speakAction() {}; // activé si l'on essaye de "parler" a l'obstacle
        virtual void touchAction() {}; // activé lorsqu'on essaye de marcher sur l'obstacle
        virtual void realTimeAction(Map * m, Player * p) {}; // activé dans diverses cas en fonction de positions de certains objets etc ...

        virtual void draw(sf::RenderWindow & app, uint16_t cellSize);
        virtual void update(const sf::Time & elapsed);

        void setMaxLife(float l) {c_MaxLife = l; if(c_Life > c_MaxLife) c_Life = c_MaxLife; }
        void setDirection(Direction dir);
        void setKiller(Living * l) { c_Killer = l; }
        void setSpeed(float s) { c_Speed = s; }
        void setPosition(uint16_t i, uint16_t j, uint16_t cellSize);
        void setPosition(const sf::Vector2f & position, uint16_t cellSize);
        void moveTo(uint16_t i, uint16_t j, uint16_t cellSize);

        std::string getName() const {return c_Name;}
        float getMaxLife() const {return c_MaxLife;}
        float getLife() const {return c_Life;}
        bool isDead() const { return c_IsDead; }
        Living * getKiller() const { return c_Killer; }
        Direction getDirection() const {return c_Direction;}
        bool isMoveable() const { return c_IsMoveable; }
        float getSpeed() const { return c_Speed; }

    protected:
        std::string c_Name;
        float c_MaxLife; // vie maximale
        float c_Life; // vie actuelle
        bool c_IsDead;
        Living * c_Killer;

        Direction c_Direction; // direction du regard

        float c_Speed; // en cell / s
        bool c_IsMoveable;
        sf::Time c_SpeedTime;
        sf::Vector2f c_PosToMove; // position ou l'on veux aller avec moveTo
        sf::Time c_SmoothMoveTime;
        uint16_t c_CellSize; // taille des cellules de la map
};

#endif // LIVING_HPP

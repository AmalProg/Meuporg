#ifndef LIVING_HPP
#define LIVING_HPP

#include <iostream>
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

        virtual void speakAction() {}; // activ� si l'on essaye de "parler" a l'obstacle
        virtual void touchAction() {}; // activ� lorsqu'on essaye de marcher sur l'obstacle
        virtual void realTimeAction(Map * m, Player * p) {}; // activ� dans diverses cas en fonction de positions de certains objets etc ...

        void draw(sf::RenderWindow & app, uint16_t cellSize);

        void setMaxLife(float l) {c_MaxLife = l; if(c_Life > c_MaxLife) c_Life = c_MaxLife; }
        void setDirection(Direction dir)  { c_Direction = dir; }
        void setKiller(Living * l) { c_Killer = l; }
        void setSpeed(float s) { c_Speed = s; }

        std::string getName() const {return c_Name;}
        float getMaxLife() const {return c_MaxLife;}
        float getLife() const {return c_Life;}
        bool isDead() const { return c_IsDead; }
        Living * getKiller() const { return c_Killer; }
        Direction getDirection() const {return c_Direction;}
        bool isMoveable();
        float getSpeed() const { return c_Speed; }
        //uint64_t getID() const { return c_ID; }

    protected:
        std::string c_Name;
        float c_MaxLife; // vie maximale
        float c_Life; // vie actuelle
        bool c_IsDead;
        Living * c_Killer;

        Direction c_Direction; // direction du regard

        float c_Speed; // en cell / s
        bool c_IsMoveable;
        sf::Clock c_SpeedClock;

        sf::RectangleShape c_Shape;

        uint64_t c_ID;
    public:
        static uint64_t ID;
};

#endif // LIVING_HPP

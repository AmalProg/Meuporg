#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Entity.hpp"

enum EffectType{HEAL, DAMAGE, SPEED};

class Effect
{
    public:
        Effect(EffectType t, float v, uint16_t maxR, uint16_t minR, float duration = 0, float tickTime = 0);

        EffectType getType() const { return c_Type; }
        float getValue() const { return c_Value; }
        uint16_t getMaxEffectRange() const { return c_MaxEffectRange; }
        uint16_t getMinEffectRange() const { return c_MinEffectRange; }
        float getDuration() const { return c_Duration; }
        float getTickTime() const { return c_TickTime; }
        Entity * getLauncher() const { return c_Launcher; }

        uint32_t getId() const { return c_Id; }

    private:
        uint32_t c_Id;
        static uint32_t id;

        EffectType c_Type; // type de l'effet
        float c_Value; // valeur de l'effet
        uint16_t c_MaxEffectRange; // range d'application de l'effet max
        uint16_t c_MinEffectRange; // range d'application de l'effet min
        float c_Duration;
        float c_TickTime; // combien de temps entre chaque tick

        Entity * c_Launcher;
};

#endif // EFFECT_HPP

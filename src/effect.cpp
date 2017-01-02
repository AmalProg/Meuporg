#include "effect.hpp"

uint32_t Effect::id = 0;

Effect::Effect(EffectType t, float v, uint16_t maxR, uint16_t minR, float duration, float tickTime)
: c_Type(t), c_Value(v), c_MaxEffectRange(maxR), c_MinEffectRange(minR), c_Duration(duration), c_TickTime(tickTime)
{
    c_Id = id;
    id++;
}

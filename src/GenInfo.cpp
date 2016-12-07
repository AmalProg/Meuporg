#include "GenInfo.hpp"

GenInfo::GenInfo()
{

}

void GenInfo::addObstacleInfos(EntityTypeId eTypeId, const float expandValue, const float maxOccurences)
{
    EntityInfo infos(eTypeId);
    infos.addInfo("expand", expandValue);
    infos.addInfo("maxOccu", maxOccurences);

    c_ObstaclesInfos.push_back(infos);
}

void GenInfo::addLivingInfos(EntityTypeId eTypeId, const uint16_t nbrGrouped, const float maxOccurences)
{
    EntityInfo infos(eTypeId);
    infos.addInfo("nbrGroup", nbrGrouped);
    infos.addInfo("maxOccu", maxOccurences);

    c_LivingsInfos.push_back(infos);
}

void GenInfo::addStandardizeInfos(EntityTypeId eTypeId, const uint16_t switchFloorValue)
{
    EntityInfo infos(eTypeId);
    infos.addInfo("switchValue", switchFloorValue);

    c_StandardizeInfos.push_back(infos);
}

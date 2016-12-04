#ifndef GENINFO_HPP
#define GENINFO_HPP

#include "Entity.hpp"

class EntityInfo
{
    public:
        EntityInfo(EntityTypeId id) : c_Id(id)
        {};

        void addInfo(const std::string & name, float value)
        { c_Infos[name] = value; }

        EntityTypeId getEntityTypeId() const { return c_Id; }
        float getInfo(const std::string & name) { return c_Infos[name]; }

    private:
        EntityTypeId c_Id;
        std::map< std::string, float > c_Infos;
};

class GenInfo
{
    public:
        GenInfo();

        void addObstacleInfos(EntityTypeId eTypeId, const float expandValue, const float maxOccurences);
        void addLivingInfos(EntityTypeId eTypeId, const uint16_t nbrGrouped, const float maxOccurences);

        std::vector< EntityInfo > getObstaclesInfos() const { return c_ObstaclesInfos; }
        std::vector< EntityInfo > getLivingsInfos() const { return c_LivingsInfos; }

    private:
        std::vector< EntityInfo > c_ObstaclesInfos;
        std::vector< EntityInfo > c_LivingsInfos;
};

#endif // GENINFO_HPP

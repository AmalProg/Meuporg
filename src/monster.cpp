#include "monster.hpp"

Monster::Monster(const std::string & name, AggroState aggroState, uint16_t aggroDist, float maxLife,
                 Direction dir, const sf::Vector2f & pos, const sf::Color & color, float speed, float delayAtkTime)
: Living(name, maxLife, dir, pos, speed), c_AggroState(aggroState), c_AggroDist(aggroDist), c_IsInAggro(false)
, c_DelayAtkTime(delayAtkTime)
{
    c_Shape.setFillColor(color);
    setSpeed(c_Speed);
}

void Monster::setLootTable(const LootTable & lootTable)
{
    c_LootTable = lootTable;
    c_Loots = c_LootTable.getLoots();
}

void Monster::speakAction(){}
void Monster::touchAction()
{
}
void Monster::realTimeAction(Map * m, Player * p) // p est le joueur en train de jouer
{
    Cell * pCell = m->getCell(p->getPosition().x, p->getPosition().y);
    Cell * myCell = m->getCell(c_Position.x, c_Position.y);

    if(m->getCellDist(pCell, myCell) > 1 && m->getCellDist(pCell, myCell) <= c_AggroDist)
    {
        c_IsInAggro = true;
        c_LastAggroTime.restart();
    }
    else if(c_LastAggroTime.getElapsedTime().asSeconds() >= 2.0)
        c_IsInAggro = false; // perte d'aggro au fil du temps si trop éloigné

    if(isMoveable()) // gestion du déplacmeent
    {
        if(c_IsInAggro)
        {
            std::vector< Cell * > path = m->getPath(myCell, pCell, false, false, 1);

            if(path.size() != 0 && path[0]->isWalkable())
            {
                m->moveLiving(this, path[0]->getC(), path[0]->getL());
                c_LastAtkTime.restart(); // on attaque pas instantannément après un déplacement
            }
            else
            {
                std::vector< Cell * > path = m->getPath(myCell, pCell, true, false, 0);

                if(path.size() != 0 && path[0]->isWalkable())
                {
                    m->moveLiving(this, path[0]->getC(), path[0]->getL());
                    c_LastAtkTime.restart(); // on attaque pas instantannément après un déplcaement
                }
            }
        }
    }
    attack(m, p);
}

Sheep::Sheep(const std::string & name, AggroState aggroState, uint16_t aggroDist, float maxLife,
      Direction dir, const sf::Vector2f & pos, const sf::Color & color, float speed, float delayAtkTime)
: Monster(name, aggroState, aggroDist, maxLife, dir, pos, color, speed, delayAtkTime)
{
    c_LootTable.addItem(Item::crap, 2, 80);
    c_Loots = c_LootTable.getLoots();
}

void Sheep::attack(Map * m, Player * p)
{
    Cell * pCell = m->getCell(p->getPosition().x, p->getPosition().y);
    Cell * myCell = m->getCell(c_Position.x, c_Position.y);

    if(m->getCellDist(pCell, myCell) == 1 && c_LastAtkTime.getElapsedTime().asSeconds() >= c_DelayAtkTime) // gestion de l'attaque
    {
        p->takeDamages(5);
        std::cout << c_Name << " made 5 damages done on " << p->getName() << "\n";
        c_LastAtkTime.restart();
    }
}

Wolf::Wolf(const std::string & name, AggroState aggroState, uint16_t aggroDist, float maxLife,
      Direction dir, const sf::Vector2f & pos, const sf::Color & color, float speed, float delayAtkTime)
: Monster(name, aggroState, aggroDist, maxLife, dir, pos, color, speed, delayAtkTime)
{
    c_LootTable.addItem(Item::teeth, 1, 30);
    c_Loots = c_LootTable.getLoots();
}

void Wolf::attack(Map * m, Player * p)
{
    Cell * pCell = m->getCell(p->getPosition().x, p->getPosition().y);
    Cell * myCell = m->getCell(c_Position.x, c_Position.y);

    if(m->getCellDist(pCell, myCell) == 1 && c_LastAtkTime.getElapsedTime().asSeconds() >= c_DelayAtkTime) // gestion de l'attaque
    {
        p->takeDamages(10);
        std::cout << c_Name << " made 10 damages on " << p->getName() << "\n";
        c_LastAtkTime.restart();
    }
}

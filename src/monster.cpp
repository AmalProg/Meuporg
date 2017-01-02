#include "monster.hpp"

Monster::Monster(EntityTypeId typeId, const std::string & name, AggroState aggroState, uint16_t aggroDist, float lostAggroTime
                 , float maxLife, Direction dir, const sf::Vector2f & pos, const sf::Color & color, float speed, float delayAtkTime)
: Living(typeId, name, maxLife, dir, pos, speed), c_AggroState(aggroState), c_AggroDist(aggroDist), c_IsInAggro(false)
, c_LostAggroTime(lostAggroTime), c_DelayAtkTime(delayAtkTime)
{
    c_Sprite.setTexture(Entity::monstersTextures);
    setDirection(dir);
}

void Monster::setLootTable(const LootTable & lootTable)
{
    c_LootTable = lootTable;
    c_Loots = new LootBag(c_LootTable.getLoots());
}

void Monster::update(const sf::Time & elapsed)
{
    c_LastAggroTime += elapsed;
    if(elapsed.asSeconds() < c_LostAggroTime && c_LastAggroTime.asSeconds() > c_LostAggroTime)
        c_LastAggroTime = sf::seconds(c_LostAggroTime);

    c_LastAtkTime += elapsed;
    if(elapsed.asSeconds() < c_DelayAtkTime && c_LastAtkTime.asSeconds() > c_DelayAtkTime)
        c_LastAtkTime = sf::seconds(c_DelayAtkTime);

    Living::update(elapsed);
}

void Monster::speakAction(){}
void Monster::touchAction(){}
void Monster::realTimeAction(Map * m, Player * p) // p est le joueur en train de jouer
{
    Cell * pCell = m->getCell(p->getPosition().x, p->getPosition().y);
    Cell * myCell = m->getCell(c_Position.x, c_Position.y);

    if(c_AggroState == AGGRESIVE && m->getCellDist(pCell, myCell) <= c_AggroDist)
    {
        c_IsInAggro = true;
        c_LastAggroTime = sf::Time::Zero;
    }
    else if(c_LastAggroTime.asSeconds() >= c_LostAggroTime)
        c_IsInAggro = false; // perte d'aggro au fil du temps si trop éloigné

    if(isMoveable()) // gestion du déplacmeent
    {
        if(c_IsInAggro && m->getCellDist(pCell, myCell) > 1)
        {// si en en aggro et en mode aggresif
            std::vector< Cell * > path = m->getPath(myCell, pCell, false, false, 1);

            if(path.size() != 0 && path[0]->isWalkable())
            {
                if(path[0]->getC() - c_Position.x == 1)
                    setDirection(RIGHT);
                else if(path[0]->getC() - c_Position.x == -1)
                    setDirection(LEFT);
                else if(path[0]->getL() - c_Position.y == 1)
                    setDirection(DOWN);
                else if(path[0]->getL() - c_Position.y == -1)
                    setDirection(UP);

                m->moveLiving(this, path[0]->getC(), path[0]->getL());
                c_LastAtkTime = sf::Time::Zero; // on attaque pas instantannément après un déplacement

            }
            else
            {
                std::vector< Cell * > path = m->getPath(myCell, pCell, true, false, 0);

                if(path[0]->getC() - c_Position.x == 1)
                    setDirection(RIGHT);
                else if(path[0]->getC() - c_Position.x == -1)
                    setDirection(LEFT);
                else if(path[0]->getL() - c_Position.y == 1)
                    setDirection(DOWN);
                else if(path[0]->getL() - c_Position.y == -1)
                    setDirection(UP);

                if(path.size() != 0 && path[0]->isWalkable())
                {
                    m->moveLiving(this, path[0]->getC(), path[0]->getL());
                    c_LastAtkTime = sf::Time::Zero; // on attaque pas instantannément après un déplcaement
                }
            }
        }
        else if(!c_IsInAggro)
        {
            uint16_t moveProb = rand() % 101;
            if(moveProb < 1) // 1% de chance de se déplacer
            {
                Direction dir = (Direction)(rand() % 4); // direction vers laquelle il veut se diriger
                Cell * cell = NULL;// cell vers laquelle il veut se diriger
                switch(dir)
                {
                case UP:
                    if(c_Position.y > 0)
                    {
                        cell = m->getUCell(myCell);
                        setDirection(UP);
                    }
                    break;
                case DOWN:
                    if(c_Position.y < m->getNbrLine()-1)
                    {
                        cell = m->getDCell(myCell);
                        setDirection(DOWN);
                    }
                    break;
                case LEFT:
                    if(c_Position.x > 0)
                    {
                        cell = m->getLCell(myCell);
                        setDirection(LEFT);
                    }
                    break;
                case RIGHT:
                    if(c_Position.x < m->getNbrColumn()-1)
                    {
                        cell = m->getRCell(myCell);
                        setDirection(RIGHT);
                    }
                    break;
                }
                if(cell != NULL && cell->isWalkable() && !cell->gotStairs())
                    m->moveLiving(this, cell->getC(), cell->getL());
            }
        }
    }
    attack(m, p);

    Living::realTimeAction(m, p);
}

Sheep::Sheep(const std::string & name, AggroState aggroState, uint16_t aggroDist, float lostAggroTime, float maxLife,
      Direction dir, const sf::Vector2f & pos, const sf::Color & color, float speed, float delayAtkTime)
: Monster(SHEEP, name, aggroState, aggroDist, lostAggroTime, maxLife, dir, pos, color, speed, delayAtkTime)
{
    c_LootTable.addItem(Item::getItemFromId(BONE), 2, 5);
    c_Loots = new LootBag(c_LootTable.getLoots());
}

void Sheep::attack(Map * m, Player * p)
{
    Cell * pCell = m->getCell(p->getPosition().x, p->getPosition().y);
    Cell * myCell = m->getCell(c_Position.x, c_Position.y);

    if(c_AggroState == AGGRESIVE && m->getCellDist(pCell, myCell) == 1 && c_LastAtkTime.asSeconds() >= c_DelayAtkTime) // gestion de l'attaque
    {
        p->takeDamages(5);
        std::cout << c_Name << " made 5 damages done on " << p->getName() << "\n";
        c_LastAtkTime -= sf::seconds(c_DelayAtkTime);
    }
}

Wolf::Wolf(const std::string & name, AggroState aggroState, uint16_t aggroDist, float lostAggroTime, float maxLife,
      Direction dir, const sf::Vector2f & pos, const sf::Color & color, float speed, float delayAtkTime)
: Monster(WOLF, name, aggroState, aggroDist, lostAggroTime, maxLife, dir, pos, color, speed, delayAtkTime)
{
    c_LootTable.addItem(Item::getItemFromId(TEETH), 1, 30);
    c_LootTable.addItem(Item::getItemFromId(BONE), 1, 5);
    c_Loots = new LootBag(c_LootTable.getLoots());

    c_Sprite.setColor(sf::Color(120, 120, 120));
}

void Wolf::attack(Map * m, Player * p)
{
    Cell * pCell = m->getCell(p->getPosition().x, p->getPosition().y);
    Cell * myCell = m->getCell(c_Position.x, c_Position.y);

    if(c_AggroState == AGGRESIVE && m->getCellDist(pCell, myCell) == 1 && c_LastAtkTime.asSeconds() >= c_DelayAtkTime) // gestion de l'attaque
    {
        p->takeDamages(10);
        std::cout << c_Name << " made 10 damages on " << p->getName() << "\n";
        c_LastAtkTime -= sf::seconds(c_DelayAtkTime);
    }
}

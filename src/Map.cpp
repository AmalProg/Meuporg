#include "Map.hpp"

std::string nbrToString(float nbr)
{
    std::ostringstream o;
    o << nbr;
    return o.str();
}

int16_t CELLSIZE = 45;

Map::Map(uint16_t c, uint16_t l, sf::RenderWindow & a) : app(a), c_CellSize(CELLSIZE)
{
    for(uint16_t i = 0; i < c; i++) // cr�er une map de cases vide
    { // colonnes
        c_Map.push_back(std::vector < Cell *>());
        for(uint16_t j = 0; j < l; j++) // lignes
            c_Map[i].push_back(new Cell(i, j));
    }

    c_View.setCenter(sf::Vector2f(0, 0));
    c_View.setSize(app.getSize().x, app.getSize().y);
}
Map::~Map()
{
    for(uint16_t i = 0; i < c_Map.size(); i++)
        for(uint16_t j = 0; j < c_Map[i].size(); j++)
            delete  c_Map[i][j];

    for(std::list< Obstacle * >::iterator it = c_Obstacles.begin(); it != c_Obstacles.end(); ++it)
        delete (*it);
    for(std::list< Living * >::iterator it = c_Livings.begin(); it != c_Livings.end(); ++it)
        delete (*it);
    for(std::list< LootBag * >::iterator it = c_LootBags.begin(); it != c_LootBags.end(); ++it)
        delete (*it);
}

void Map::moveMap()
{
    if(c_Focus->getPosition().x >= c_Map.size() - c_View.getSize().x / CELLSIZE / 2)
        c_View.setCenter(c_Map.size() * CELLSIZE - c_View.getSize().x/2, c_View.getCenter().y); // place la map a droite
    else if(c_Focus->getPosition().x < c_View.getSize().x / CELLSIZE / 2)
            c_View.setCenter(c_View.getSize().x/2, c_View.getCenter().y); // place la map a gauche
    else if(c_Focus->getPosition().x >= c_View.getSize().x / CELLSIZE / 2)
        c_View.setCenter(c_Focus->getPosition().x * CELLSIZE, c_View.getCenter().y); // centre la map sur le joueur focus

    if(c_Focus->getPosition().y >= c_Map[0].size() - c_View.getSize().y / CELLSIZE / 2)
        c_View.setCenter(c_View.getCenter().x, c_Map[0].size() * CELLSIZE - c_View.getSize().y/2); // place la map en bas
    else if(c_Focus->getPosition().y < c_View.getSize().y / CELLSIZE / 2)
                c_View.setCenter(c_View.getCenter().x, c_View.getSize().y/2); // place la map en haut
    else if(c_Focus->getPosition().y >= c_View.getSize().y / CELLSIZE / 2)
        c_View.setCenter(c_View.getCenter().x, c_Focus->getPosition().y * CELLSIZE); // centre la map sur le joueur focus

    app.setView(c_View);
}

void Map::removeLiving(uint16_t c, uint16_t l)
{
    std::list< Living * >::iterator it = std::find(c_Livings.begin(), c_Livings.end(), c_Map[c][l]->getLiving());
    c_Map[c][l]->setLiving(NULL);
    c_Livings.erase(it);
}
bool Map::addObstacle(Obstacle * o,  uint16_t c, uint16_t l)
{
    if(o->isCover()) // si il recouvre du sol
    {
        o->setPosition(c, l);
        if(c_Map[c][l]->isCovered())
            removeObstacle(c_Map[c][l]->getCover(), c, l);
        c_Map[c][l]->setCover(o);
        c_Obstacles.push_back(o);

        return true;
    }
    if(o->isFiller() && c_Map[c][l]->getLiving() == NULL)
    {
        o->setPosition(c, l);
        if(c_Map[c][l]->isFilled())
            removeObstacle(c_Map[c][l]->getFiller(), c, l);
        c_Map[c][l]->setFiller(o);
        c_Obstacles.push_back(o);

        return true;
    }
    return false;
}
void Map::removeObstacle(const Obstacle * o, uint16_t c, uint16_t l)
{
    c_Map[c][l]->removeObstacle(o);
    std::list< Obstacle * >::iterator it = std::find(c_Obstacles.begin(), c_Obstacles.end(), o);
    c_Obstacles.erase(it);
}
bool Map::addStairs(Stairs * s,  uint16_t c, uint16_t l)
{
    if(c_Map[c][l]->isWalkable())
    {
        s->setPosition(c, l);
        c_Map[c][l]->setStairs(s); // on donne l'ID de l'obstacle
        c_Obstacles.push_back(s);
        c_Stairss.push_back(s);

        return true;
    }
    return false;
}
bool Map::addMonster(Monster * m, uint16_t c, uint16_t l)
{
    if(c_Map[c][l]->isWalkable() && c_Map[c][l]->getLiving() == NULL) // si la case est vide
    {
        c_Map[c][l]->setLiving(m);
        c_Monsters.push_back(m);
        c_Livings.push_back(m);
        m->setPosition(c, l);

        return true;
    }
    return false;
}
void Map::removeMonster(const Monster * m)
{
    std::list< Monster * >::iterator it = std::find(c_Monsters.begin(), c_Monsters.end(), m);
    removeLiving(m->getPosition().x, m->getPosition().y);
    c_Monsters.erase(it);
}
bool Map::addCharacter(Character * cha, uint16_t c, uint16_t l)
{
    if(c_Map[c][l]->isWalkable() && c_Map[c][l]->getLiving() == NULL) // si la case est vide
    {
        c_Map[c][l]->setLiving(cha); // on donne son Id de joueur
        c_Characters.push_back(cha);
        c_Livings.push_back(cha);
        cha->setPosition(c, l);

        return true;
    }
    return false;
}
void Map::removeCharacter(const Character * cha)
{
    std::list< Character * >::iterator it = std::find(c_Characters.begin(), c_Characters.end(), cha);
    removeLiving(cha->getPosition().x, cha->getPosition().y);
    c_Characters.erase(it);
}
bool Map::addLootBag(LootBag * lB,  uint16_t c, uint16_t l)
{
    std::cout << c << ", " << l << "\n";
    c_Map[c][l]->addLootBag(lB); // on donne l'ID de l'obstacle
    c_LootBags.push_back(lB);
    lB->setPosition(sf::Vector2f(c, l));

    return true;
}
void Map::removeLootBag(const LootBag * lB, uint16_t c, uint16_t l)
{
    c_Map[c][l]->removeLootBag(lB);
    std::list< LootBag * >::iterator it = std::find(c_LootBags.begin(), c_LootBags.end(), lB);
    c_LootBags.erase(it);
    delete lB;
}

void Map::draw()
{
    static sf::RectangleShape backG(sf::Vector2f(CELLSIZE, CELLSIZE));
    backG.setFillColor(sf::Color::White);
    static sf::RectangleShape lineR(sf::Vector2f(1, CELLSIZE));
    lineR.setFillColor(sf::Color::Black);
    static sf::RectangleShape lineB(sf::Vector2f(CELLSIZE, 1));
    lineB.setFillColor(sf::Color::Black);

    if(c_Map.begin() != c_Map.end()) // si la map est definit
    {
        for(uint16_t i = 0; i < c_Map.size(); i++)
        {
            for(uint16_t j = 0; j < c_Map[i].size(); j++)
            {
                backG.setPosition(CELLSIZE * i, CELLSIZE * j);
                lineR.setPosition(CELLSIZE * i + (CELLSIZE-1), CELLSIZE * j);
                lineB.setPosition(CELLSIZE * i, CELLSIZE * j + (CELLSIZE-1));
                app.draw(backG); // affiche le fond blanc d'une case
                app.draw(lineR);
                app.draw(lineB); // et les bords

                std::list< Obstacle * > os = c_Map[i][j]->getObstacles();
                for(std::list< Obstacle * >::iterator it = os.begin(); it != os.end(); ++it)
                {
                    (*it)->draw(app, CELLSIZE);
                }
                std::list< LootBag * > lBs = c_Map[i][j]->getLootBags();
                for(std::list< LootBag * >::iterator it = lBs.begin(); it != lBs.end(); ++it)
                {
                    (*it)->draw(app, CELLSIZE);
                }
                if(c_Map[i][j]->getLiving() != NULL)
                {
                    c_Map[i][j]->getLiving()->draw(app, CELLSIZE);
                }
            }
        }
    }
}

void Map::generateMap(const GenInfo & genInfos)
{
    std::vector< EntityInfo > ObsInfos = genInfos.getObstaclesInfos();
    for(std::vector< EntityInfo >::iterator it = ObsInfos.begin();
            it != ObsInfos.end(); it++)
    {
        EntityTypeId id = (*it).getEntityId();
        float maxOccu = (*it).getInfo("maxOccu");
        float expand = (*it).getInfo("expand");

        uint32_t occurencesNbr = 0;
        if(maxOccu > 0)
        {
            uint16_t nbrTryMax = maxOccu * 10/(log(maxOccu) + 1); // nombre d'essais max pour placer des occurences
            uint16_t nbrTry = 0;
            do
            {
                uint16_t c = rand() % getNbrColumn();
                uint16_t l = rand() % getNbrLine();

                Cell * cell = getCell(c, l);

                if(addObstacleOnCell(id, cell)) // si le nom�bre � chang� (normalement +1)
                {
                    expandEntity(id, cell, expand);
                    occurencesNbr++; //oon ajoute une occurence
                }

                nbrTry++;
            }while(nbrTry < nbrTryMax && occurencesNbr < maxOccu);
        }
        else if(maxOccu == -1)
        {
            for(uint16_t i = 0; i < getNbrColumn(); i++)
            {
                for(uint16_t j = 0; j < getNbrLine(); j++)
                {
                    addObstacleOnCell(id, getCell(i, j));
                }
            }
        }
    }
    standardize(ROCK, 5, sf::Vector2i(0, getNbrColumn()-1), sf::Vector2i(0, getNbrLine()-1));
    standardize(WATER, 5, sf::Vector2i(0, getNbrColumn()-1), sf::Vector2i(0, getNbrLine()-1));

    while(true)
    { // place un escalier prot�g� par un porte ferm�e a clef
        uint16_t x = rand() % getNbrColumn();
        uint16_t y = rand() % getNbrLine();
        getCell(x, y)->stateTest();
        if(getCell(x, y)->isWalkable())
        {
            addStairs(new Stairs(NULL, false), x, y);
            addObstacle(new LockedDoor, x, y);
            break;
        }
    }

    std::vector< EntityInfo > LivInfos = genInfos.getLivingsInfos();
    for(std::vector< EntityInfo >::iterator it = LivInfos.begin();
            it != LivInfos.end(); it++)
    {
        EntityTypeId id = (*it).getEntityId();
        float maxOccu = (*it).getInfo("maxOccu");
        float nbrGroup = (*it).getInfo("nbrGroup");

        uint32_t occurencesNbr = 0;
        if(maxOccu > 0)
        {
            uint16_t nbrTryMax = maxOccu * 10/(log(maxOccu) + 1); // nombre d'essais max pour placer des occurences
            uint16_t nbrTry = 0;
            do
            {
                uint16_t c = rand() % getNbrColumn();
                uint16_t l = rand() % getNbrLine();
                Cell * cell = getCell(c, l);

                if(addLivingOnCell(id, cell)) // si le nom�bre � chang� (normalement +1)
                    occurencesNbr++; //oon ajoute une occurence

                nbrTry++;
            }while(nbrTry < nbrTryMax && occurencesNbr < maxOccu);
        }
    }
}

void Map::expandEntity(EntityTypeId id, Cell * cell, float expandValue)
{
    if(expandValue <= 0)
        return;

    bool eTable[4];
    for(uint16_t i = 0; i < 4; i++)
        eTable[i] = true; // l'expansion sur la cellule i va bien �tre effectu�e

    if(expandValue < 1)
    {
        uint16_t f = 0;
        uint16_t s = 0;
        uint16_t t = 0;

        if(expandValue <= 1.25)
        {
            f = rand()%4;
            eTable[f] = false;
        }
        if(expandValue <= 1.5)
        {
            do{ s = rand()%4; }while(s == f);
            eTable[s] = false;
        }
        if(expandValue <= 1.75)
        {
            do{ t = rand()%4; }while(t == f || t == s);
            eTable[t] = false;
        }

        expandValue = 1;
    }

    Cell * r = getRCell(cell);
    Cell * l = getLCell(cell);
    Cell * d = getDCell(cell);
    Cell * u = getUCell(cell);

    if(eTable[0] && r != NULL && addObstacleOnCell(id, r))
        expandEntity(id, r, expandValue-1);
    if(eTable[1] && l != NULL && addObstacleOnCell(id, l))
        expandEntity(id, l, expandValue-1);
    if(eTable[2] && d != NULL && addObstacleOnCell(id, d))
        expandEntity(id, d, expandValue-1);
    if(eTable[3] && u != NULL && addObstacleOnCell(id, u))
        expandEntity(id, u, expandValue-1);
}
bool Map::addObstacleOnCell(EntityTypeId id, Cell * cell)
{
    uint16_t c = cell->getC();
    uint16_t l = cell->getL();

    cell->stateTest();

    switch(id)
    {
    case ROCK:
        if(!cell->isFilled())
        {
            addObstacle(new Rock(), c, l);
            return true;
        }
        break;
    case DOOR:
        if(cell->isWalkable())
        {
            addObstacle(new Door(), c, l);
            return true;
        }
        break;
    case LOCKEDDOOR:
        if(cell->isWalkable())
        {
            addObstacle(new LockedDoor(), c, l);
            return true;
        }
        break;
    case WATER:
        if(!cell->isCovered() || cell->getCover()->getEntityTypeId() == GRASS)
        {
            addObstacle(new Water(), c, l);
            return true;
        }
        break;
    case GRASS:
        if(!cell->isCovered())
        {
            addObstacle(new Grass(), c, l);
            return true;
        }
        break;

    default:
        break;
    }

    return false;
}
bool Map::addLivingOnCell(EntityTypeId id, Cell * cell)
{
    uint16_t c = cell->getC();
    uint16_t l = cell->getL();
    cell->stateTest();

    switch(id)
    {
    case SHEEP:
        if(cell->isWalkable() && cell->getLiving() == NULL)
            addMonster(new Sheep(), c, l);
        break;
    case WOLF:
        if(cell->isWalkable() && cell->getLiving() == NULL)
            addMonster(new Wolf(), c, l);
        break;
    case CHARACTER:
        if(cell->isWalkable() && cell->getLiving() == NULL)
            addCharacter(new Character(), c, l);
        break;

    default:
        break;
    }
}

void Map::standardize(EntityTypeId id, uint16_t switchFloor, sf::Vector2i targetColumns, sf::Vector2i targetLines)
{
    sf::Vector2i mapCalculCol; // dimensions pour les calculs de voisins
    sf::Vector2i mapCalculLine;

    // verification que les cells peuvent bien �tre dans la taille de la map
    if(targetColumns.x <= 0) { targetColumns.x = 0; mapCalculCol.x = targetColumns.x; }
    else mapCalculCol.x = targetColumns.x - 1;
    if(targetLines.x <= 0) { targetLines.x = 0; mapCalculLine.x = targetLines.x; }
    else mapCalculLine.x = targetLines.x - 1;
    if(targetColumns.y >= getNbrColumn()-1) { targetColumns.y = getNbrColumn()-1; mapCalculCol.y = targetColumns.y; }
    else mapCalculCol.y = targetColumns.y + 1;
    if(targetLines.y >= getNbrLine()-1) { targetLines.y = getNbrLine()-1; mapCalculLine.y = targetLines.y; }
    else mapCalculLine.y = targetLines.y + 1;

    std::vector< std::vector< bool > > idMap(getNbrColumn(), std::vector< bool >(getNbrLine())); // tableau bool�en donnant les cell poss�dant l'id
    std::vector< std::vector< uint16_t > > neighbourMap(getNbrColumn(), std::vector< uint16_t >(getNbrLine())); // tableau du nombre de voisins par case

    for(uint16_t i = mapCalculCol.x; i <= mapCalculCol.y; i++)
    { // on standardize sur les colonnes et lines donn�es en param�tre
        for(uint16_t j = mapCalculLine.x; j <= mapCalculLine.y; j++)
        {
            idMap[i][j] = false;
            neighbourMap[i][j] = 8 - 3 * (i == 0 || i == getNbrColumn()-1) - 3 * (j == 0 || j == getNbrLine()-1)
            + 1 * ((i == 0 && j == 0) || (i == getNbrColumn()-1 && j == getNbrLine()-1) || (i == 0 && j == getNbrLine()-1) || (i == getNbrColumn()-1 && j == 0));

            Cell * cell = getCell(i, j);
            std::list< Obstacle * > os = cell->getObstacles();
            for(std::list< Obstacle * >::iterator it = os.begin(); it != os.end(); ++it)
            {
                if((*it)->getEntityTypeId() == id)
                {
                    idMap[i][j] = true;
                    break;
                }
            }
        }
    }

    for(uint16_t i = targetColumns.x; i <= targetColumns.y; i++)
    {
        for(uint16_t j = targetLines.x; j <= targetLines.y; j++)
        {
            if(!idMap[i][j]) // si on est pas du type � harmoniser
            {
                uint16_t nbrNeighbourId = 0;
                for(int16_t x = -1; x <= 1; x++)
                    for(int16_t y = -1; y <= 1; y++)
                        if(i+x >= 0 && j+y >= 0 && i+x < getNbrColumn() && j+y < getNbrLine())
                            if(idMap[i+x][j+y])
                                nbrNeighbourId++; // on compte le nbr de voisin du type � harmoniser

                if(nbrNeighbourId >= round(switchFloor / 8.f * neighbourMap[i][j])) // si il y a plus de voisin du type � harmoniser que un ratio des cells voisines
                {
                     if(addObstacleOnCell(id, getCell(i, j)))
                     {
                        standardize(id, switchFloor, sf::Vector2i(i-1, i+1), sf::Vector2i(j-1, j+1));
                        // on standardize les 8 cellules autour (update)
                     }
                }
            }
        }
    }
}


void Map::moveLiving(Living * li, uint16_t c, uint16_t l)
{
    uint16_t aC = li->getPosition().x;
    uint16_t aL = li->getPosition().y;
    c_Map[aC][aL]->setLiving(NULL); // le joueur n'est plus sur cette case

    c_Map[c][l]->setLiving(li); // on donne son Id de joueur
    li->setPosition(c, l);
}

void Map::setFocus(Living * p)
{
    c_Focus = p;
    moveMap();
}
std::vector< Cell * > Map::getPath(const Cell * startC, const Cell * endC, bool skipLivings, bool fullMap, uint16_t maxMoreSteps) const
{
    std::vector < std::vector < int16_t > > distMap;
    for(uint16_t i = 0; i < getNbrColumn(); i++)
    { // init de la map
        distMap.push_back(std::vector < int16_t >(0));
        for(uint16_t j = 0; j < getNbrLine(); j++)
            distMap[i].push_back(-1);
    }
    distMap[endC->getC()][endC->getL()] = 0; // la cellule ayant comme valeur 0

    std::vector < const Cell * > endCell; endCell.push_back(endC);

    pathMap(&distMap, endCell, 0, startC, skipLivings, fullMap);

    std::vector< Cell * > bestPath;
    recursivePath(&bestPath, &distMap, startC, endC, maxMoreSteps);

    return bestPath;
}
uint16_t Map::getPathLenght(const Cell * startC, const Cell * endC, bool skipLivings) const
{
    return getPath(startC, endC, skipLivings, false, 0).size();
}
void Map::recursivePath(std::vector < Cell * > * bestPath, std::vector < std::vector < int16_t > > * distMap, const Cell * startC, const Cell * endC, uint16_t maxMoreSteps) const
{
    if(endC == startC)
        return;

    uint16_t col = startC->getC();
    uint16_t line = startC->getL();

    const Cell * r = getRCell(startC);
    const Cell * l = getLCell(startC);
    const Cell * d = getDCell(startC);
    const Cell * u = getUCell(startC);

    std::vector< const Cell * > possibleCells;

    if(r != NULL && (*distMap)[col+1][line] >= 0 && (*distMap)[col+1][line] <= (*distMap)[col][line] + maxMoreSteps/2.f)
        possibleCells.push_back(r);
    if(l != NULL && (*distMap)[col-1][line] >= 0 && (*distMap)[col-1][line] <= (*distMap)[col][line] + maxMoreSteps/2.f)
        possibleCells.push_back(l);
    if(d != NULL && (*distMap)[col][line+1] >= 0 && (*distMap)[col][line+1] <= (*distMap)[col][line] + maxMoreSteps/2.f)
        possibleCells.push_back(d);
    if(u != NULL && (*distMap)[col][line-1] >= 0 && (*distMap)[col][line-1] <= (*distMap)[col][line] + maxMoreSteps/2.f)
        possibleCells.push_back(u);

    uint16_t choice;
    if(possibleCells.size() > 0)
        choice = rand() % possibleCells.size();
    else
        return;

    uint16_t newC = possibleCells[choice]->getC();
    uint16_t newL = possibleCells[choice]->getL();
    Cell * nextCell = getCell(newC, newL);
    bestPath->push_back(nextCell);

    recursivePath(bestPath, distMap, nextCell, endC, maxMoreSteps - ((*distMap)[newC][newL] - (*distMap)[col][line] + 1));
}
void Map::pathMap(std::vector < std::vector < int16_t > > * distMap, const std::vector< const Cell * > & actualCs, uint16_t actualDist, const Cell * targetC, bool skipLivings, bool fullMap) const
{
    actualDist++;
    std::vector< const Cell * > nextCs;

    for(uint16_t i = 0; i < actualCs.size(); i++)
    {
        std::vector< const Cell * > neigbours;
        neigbours.push_back(getRCell(actualCs[i])); // les quatres cells adjacentes de chaques cellules ajout�es pr�c�dement
        neigbours.push_back(getLCell(actualCs[i]));
        neigbours.push_back(getDCell(actualCs[i]));
        neigbours.push_back(getUCell(actualCs[i]));

        for(uint16_t j = 0; j < 4; j++)
        {
            if(neigbours[j] == targetC)
            {
                (*distMap)[neigbours[j]->getC()][neigbours[j]->getL()] = actualDist;

                if(!fullMap)
                    return;
            }

            if(neigbours[j] != NULL && (neigbours[j]->isWalkable() || (skipLivings && neigbours[j]->getLiving() != NULL)))
            {
                if((*distMap)[neigbours[j]->getC()][neigbours[j]->getL()] < 0)
                {
                    (*distMap)[neigbours[j]->getC()][neigbours[j]->getL()] = actualDist;
                    nextCs.push_back(neigbours[j]);
                }
            }
        }
    }

    if(nextCs.size() > 0)
        pathMap(distMap, nextCs, actualDist, targetC, skipLivings, fullMap);
}

std::list< Cell * > Map::getCellsDistFromCell(uint16_t c, uint16_t l, uint16_t dist) const
{
    std::list< Cell * > cells;

    recursiveNeighbourDist(cells, c_Map[c][l], dist, c_Map[c][l]);

    /*sf::RectangleShape p(sf::Vector2f(10, 10));
    p.setFillColor(sf::Color::Red);
    for(uint16_t i = 0; i < cells.size(); i++)
    {
        p.setPosition(cells[i]->getC() * CELLSIZE, cells[i]->getL() * CELLSIZE);
        app.draw(p);
    }*/

    return cells;
}
std::list< Cell * > Map::getCellsBetweenDistFromCell(uint16_t c, uint16_t l, uint16_t distMax, int distMin) const
{
    std::list< Cell * > cellsMax;

    recursiveNeighbourDist(cellsMax, c_Map[c][l], distMax, c_Map[c][l]);

    for(std::list< Cell * >::iterator it = cellsMax.begin(); it != cellsMax.end(); ++it)
    {
        if(getCellDist(c_Map[c][l], *it) < distMin)
        {
            it = cellsMax.erase(it);
            it--;
        }
    }

    /*sf::RectangleShape p(sf::Vector2f(10, 10));
    p.setFillColor(sf::Color::Red);
    for(std::list< Cell * >::iterator it = cellsMax.begin(); it != cellsMax.end(); it++)
    {
        p.setPosition((*it)->getC() * CELLSIZE, (*it)->getL() * CELLSIZE);
        app.draw(p);
    }*/

    return cellsMax;
}
void Map::recursiveNeighbourDist(std::list< Cell * > & cells, Cell * c, int distMax, Cell * baseCell) const
{
    if(c == NULL || getCellDist(c, baseCell) > distMax || std::find(cells.begin(), cells.end(), c) != cells.end())
        return;

    cells.push_back(c);

    recursiveNeighbourDist(cells, getRCell(c), distMax, baseCell);
    recursiveNeighbourDist(cells, getLCell(c), distMax, baseCell);
    recursiveNeighbourDist(cells, getUCell(c), distMax, baseCell);
    recursiveNeighbourDist(cells, getDCell(c), distMax, baseCell);
}

bool Map::getLivingOnCell(Living * p, uint16_t c, uint16_t l)
{
    const Cell * cell = c_Map[c][l];
    if(c_Map[c][l]->getLiving() != NULL)
    { // si il y a un Living sur le cellule
        p = cell->getLiving();
        return true;
    }
    return false;
}
bool Map::getObstaclesOnCell(std::list< Obstacle * > os, uint16_t c, uint16_t l)
{
    const Cell * cell = c_Map[c][l];
    if(cell->getObstacles().size() != 0)
    { // si il y a un obstacle sur le cellule
        os = cell->getObstacles();
        return true;
    }
    return false;
}

float Map::getDist(Cell * c1, Cell * c2) const
{
    if(c1 == NULL || c2 == NULL)
        return -1;
    return sqrt((c1->getC() - c2->getC()) * (c1->getC() - c2->getC()) + (c1->getL() - c2->getL()) * (c1->getL() - c2->getL()));
}
uint16_t Map::getCellDist(Cell * c1, Cell * c2) const
{
    if(c1 == NULL || c2 == NULL)
        return -1;
    return abs(c1->getC() - c2->getC()) + abs(c1->getL() - c2->getL());
}
bool Map::lineOfSight(Cell * c1, Cell * c2) const
{
    std::vector < Cell *> cells = getCellsOnLineOfSight(c1, c2);

    for(std::vector< Cell *>::const_iterator it = cells.begin(); it != cells.end(); ++it)
    {
        const std::list< Obstacle * > obstacles = (*it)->getObstacles();
        for(std::list< Obstacle * >::const_iterator it2 = obstacles.begin(); it2 != obstacles.end(); ++it2)
        {
            if((*it)->getC() == c2->getC() && (*it)->getL() == c2->getL())
            {
                if((*it2)->isAttackBlocking())
                    return false;
            }
            else if((*it2)->isVisionBlocking())
                return false;
        }
    }
    return true;
}
std::vector < Cell *> Map::getCellsOnLineOfSight(Cell * c1, Cell * c2) const
{
    sf::Vector2f origin(c1->getC() * c_CellSize + c_CellSize/2.f, c1->getL() * c_CellSize + c_CellSize/2.f); // point d'origine de la ligne de vue
    sf::Vector2f direction((c2->getC() - c1->getC()) * c_CellSize, (c2->getL() - c1->getL()) * c_CellSize); // direction de la ligne de vue

    std::vector < Cell *> cells; // cellules sur la trajectoire de la ligne de vue
    cells.push_back(c2); // on ajoute la cell focus de base

    for(uint16_t i = 0; i < c_Map.size(); ++i)
    {
        for(uint16_t j = 0; j < c_Map[i].size(); ++j)
        {
            if(getCellDist(c1, getCell(i, j)) <= getCellDist(c1, c2)) // pas de tests si la cellule est trop loin
            {
                sf::Vector2f pLT(i * c_CellSize, j * c_CellSize); // point left top
                sf::Vector2f pRT(i * c_CellSize + c_CellSize, j * c_CellSize); // point right top
                sf::Vector2f pRB(i * c_CellSize + c_CellSize, j * c_CellSize + c_CellSize); // point right bottom
                sf::Vector2f pLB(i * c_CellSize, j * c_CellSize + c_CellSize); // point left bottom

                float kLT = (pLT.y - origin.y) * c_CellSize / (direction.y * c_CellSize);
                float kRT = (origin.x - pRT.x) * c_CellSize / (-direction.x * c_CellSize);
                float kRB = (pRB.y - origin.y) * -c_CellSize / (direction.y * -c_CellSize);
                float kLB = (origin.x - pLB.x) * -c_CellSize / (direction.x * c_CellSize);
                float iLT = ((origin.y - pLT.y) * direction.x + (pLT.x - origin.x) * direction.y) / (-direction.y * c_CellSize);
                float iRT = ((origin.y - pRT.y) * direction.x + (pRT.x - origin.x) * direction.y) / (direction.x * c_CellSize);
                float iRB = ((origin.y - pRB.y) * direction.x + (pRB.x - origin.x) * direction.y) / (direction.y * c_CellSize);
                float iLB = ((origin.y - pLB.y) * direction.x + (pLB.x - origin.x) * direction.y) / (-direction.x * c_CellSize);

                if(((kLT <= 0 || kLT >= 1) || (iLT <= 0 || iLT >= 1)) && ((kRT <= 0 || kRT >= 1) || (iRT <= 0 || iRT >= 1))
                   && ((kRB <= 0 || kRB >= 1) || (iRB <= 0 || iRB >= 1)) && ((kLB <= 0 || kLB >= 1) || (iLB <= 0 || iLB >= 1)))
                    continue; // si pas de collision entre la ligne de vue et la cellule on passe � la suivante
                cells.push_back(getCell(i, j)); // sinon ajout au cellules sur la ligne de vue
            }
        }
    }
    return cells;
}

Cell * Map::getRCell(const Cell * c) const { if(c->getC()+1 < c_Map.size()) return c_Map[c->getC()+1][c->getL()]; return NULL; }
Cell * Map::getLCell(const Cell * c) const { if(c->getC()-1 >= 0) return c_Map[c->getC()-1][c->getL()]; return NULL; }
Cell * Map::getUCell(const Cell * c) const { if(c->getL()-1 >= 0) return c_Map[c->getC()][c->getL()-1]; return NULL; }
Cell * Map::getDCell(const Cell * c) const { if(c->getL()+1 < c_Map[c->getC()].size()) return c_Map[c->getC()][c->getL()+1]; return NULL; }

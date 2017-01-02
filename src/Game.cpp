#include "Game.hpp"

Game::Game(sf::RenderWindow & a) : app(a), c_NbrCellsToDraw(20), c_ActualLevel(0), c_Menu(app), c_RawText(RawText()), c_ChoiceText(ChoiceText()),
 c_NbrLoops(0)
{
    Item::initItems();
    Entity::initTextures();

    c_Font.loadFromFile("arial.ttf");
    c_FpsTxt.setString("0");
    c_FpsTxt.setFont(c_Font);
    c_FpsTxt.setColor(sf::Color::Red);

    c_Map = new Map(app, app.getSize().x / c_NbrCellsToDraw);
    c_Maps.push_back(c_Map);
    c_Player = new Player("Amal", 100, UP, 7.5);
    c_Player->takeItem(Item::getItemFromId(KEY), 5);
    c_Player->takeItem(Item::getItemFromId(WOODENSWORD), 1);
    c_Player->takeItem(Item::getItemFromId(LONGSWORD), 1);
    c_Player->takeItem(Item::getItemFromId(DAMAGEPOTION1), 50);
    c_Player->takeItem(Item::getItemFromId(HEALPOTION1), 50);

    GenInfo genInfo;
    genInfo.addObstacleInfos(GRASS, 0, -1);
    genInfo.addObstacleInfos(WATER, 7.5, 1);
    genInfo.addObstacleInfos(SOIL, 7.5, 1);
    genInfo.addObstacleInfos(SAND, 3.25, 2);
    genInfo.addObstacleInfos(QUICKSAND, 3.25, 2);
    genInfo.addObstacleInfos(ROCK, 2.5, 2);
    genInfo.addObstacleInfos(ROCK, 3.25, 4);
    genInfo.addObstacleInfos(ROCK, 7.5, 2);
    genInfo.addObstacleInfos(ROCK, 4.75, 1);
    genInfo.addObstacleInfos(ROCK, 0.5, 2);
    genInfo.addObstacleInfos(FIRE, 1.5, 1);
    genInfo.addStandardizeInfos(WATER, 4);
    genInfo.addStandardizeInfos(SAND, 5);
    genInfo.addStandardizeInfos(SOIL, 5);
    genInfo.addStandardizeInfos(ROCK, 5);
    genInfo.addLivingInfos(SHEEP, 1, 10);
    genInfo.addLivingInfos(WOLF, 1, 4);
    c_Map->generateMap(genInfo, 37, 29);

    uint16_t x = 0, y = 0;
    do
    { // place le player aleatoirement sur la map
        x = rand() % c_Map->getNbrColumn();
        y = rand() % c_Map->getNbrLine();
    }while(!c_Map->addCharacter(c_Player, x, y));
    c_Map->setFocus(c_Player);

    LootBag * basicBag = new LootBag;
    basicBag->addItem(Item::getItemFromId(KEY), 3);
    basicBag->addItem(Item::getItemFromId(WOODENSWORD), 1);
    basicBag->addItem(Item::getItemFromId(HEALPOTION1), 2);
    while(true)
    { // place le lootBag de base aleatoirement sur la map
        x = rand() % c_Map->getNbrColumn();
        y = rand() % c_Map->getNbrLine();
        if(c_Map->getCell(x, y)->isWalkable())
        {
            c_Map->addLootBag(basicBag, x, y);
            break;
        }
    }

    c_ItemShortCuts.push_back(sf::Keyboard::Num1);
    c_ItemShortCuts.push_back(sf::Keyboard::Num2);
    c_ItemShortCuts.push_back(sf::Keyboard::Num3);
    c_ItemShortCuts.push_back(sf::Keyboard::Num4);
}
Game::~Game()
{
    delete c_Map;
}

void Game::loop()
{
    c_GameClock.restart();
    while (app.isOpen())
    {
        {
            const Bag * bag = c_Player->getBag();
            int16_t item;
            if((item = c_Menu.getItemToDestroy()) != -1) // gestion de destruction d'item
            {
                c_Player->removeItem(bag->getItem(item), bag->getNbrOfItem(item));
            }
            if((item = c_Menu.getItemToEquip()) != -1) // gestion de d'équippement d'item
            {
                c_Player->equip(bag->getItem(item));
            }

            const Cell * focusedCell = NULL;
            if((focusedCell = c_Menu.getCellToFocus()) != NULL)// gestion d'utilisation d'item
            {
                useItem(c_Player, c_Player->getBag()->getItem(c_Menu.getItemToUse()), focusedCell->getC(), focusedCell->getL());
            }

            int16_t keyIndex;
            if((keyIndex = c_Menu.getKeyIndexToShortCut()) != -1) // gestion des raccourcis
            {
                c_Player->setEquippedItem(c_Player->getItem(c_Menu.getItemToShortCut()), keyIndex);
            }

            BagCell itemToTake; itemToTake.item = NULL; itemToTake.nbr = 0;
            itemToTake = c_Menu.getItemToTake();
            if(itemToTake.item != NULL) // gestion de récupératin d'item
            {
                if(!c_Player->takeItem(itemToTake.item, itemToTake.nbr))
                    std::cout << "hein??, l'item est introuvable";
            }

            std::list< LootBag * > lootBagsToDelete;
            for(std::list< LootBag * >::const_iterator it = c_Map->getLootBags().begin(); it != c_Map->getLootBags().end(); ++it)
            { // gestion des LootBags
                if((*it)->getNbrItems() == 0)
                    lootBagsToDelete.push_back((*it));
            }
            for(std::list< LootBag * >::iterator it = lootBagsToDelete.begin(); it != lootBagsToDelete.end(); it++)
            {
                c_Map->removeLootBag((*it), (*it)->getPosition().x, (*it)->getPosition().y);
                delete (*it);
            }

            for(std::list< Stairs * >::const_iterator it = c_Map->getStairss().begin(); it != c_Map->getStairss().end(); ++it)
            { // Gestion des escaliers
                if((*it)->isActivated())
                {
                    (*it)->setActivated(false); // on desactive l'escalier

                    Map * lastMap = c_Map;
                    lastMap->removeCharacter(c_Player); // on enlève player de la map actuelle
                    lastMap->setFocus(NULL);

                    c_Map = Map::getMapFromId((*it)->getLinkedMapId()); // on pointe la nouvelle map

                    if(c_Map == NULL)
                    {
                        c_Map = new Map(app, lastMap->getCellSize());
                        c_ActualLevel++;
                        genNextMap(c_Map, lastMap, c_ActualLevel);
                        (*it)->setLinkedMapId(c_Map->getMapId());
                        c_Maps.push_back(c_Map);
                    }
                    else if((*it)->isRising()) // on remonte d'un étage
                        c_ActualLevel--;
                    else
                        c_ActualLevel++; // on descend

                    for(std::list< Stairs * >::const_iterator it2 = c_Map->getStairss().begin(); it2 != c_Map->getStairss().end(); ++it2)
                    {
                        if((*it2)->getLinkedMapId() == lastMap->getMapId())
                        { // on arrive sur l'escalier qui va a l'étage ou l'on était
                            c_Map->addCharacter(c_Player, (*it2)->getPosition().x, (*it2)->getPosition().y);
                            c_Map->setFocus(c_Player);
                            break;
                        }
                    }
                    break;
                }
            }
        }

        static float fpsValue = 1;
        { // event managing
            if(c_NbrLoops % 50 == 2)
                fpsValue = c_GameClock.getElapsedTime().asSeconds();

            if(c_GameClock.getElapsedTime().asSeconds() < 1.f/5)
                update(c_GameClock.restart());
            else
                c_GameClock.restart();

            eventManage();
            deathManage();
            realTimeEventManage();
            walkEventTest();
        }


        c_FpsTxt.setString(nbrToString((int)(1/fpsValue)));
        c_FpsTxt.setPosition(app.getSize().x - c_FpsTxt.getLocalBounds().width, 0);

        { //draw managing
            app.clear();

            c_Map->moveMap(); // déplace la map par rapport au focus
            c_Map->draw(); // draw the map

            sf::View lastView = app.getView();

            c_Menu.draw(c_Map, c_Player); // draw actives menus

            app.setView(sf::View(sf::FloatRect(0, 0, app.getSize().x, app.getSize().y)));
            app.draw(c_FpsTxt);

            app.display();
        }

        if(c_NbrLoops == 1)
        {
            sf::Texture texture;
            texture.create(app.getSize().x, app.getSize().y);
            texture.update(app);
            c_RawText.newText("Recherchez votre sac. Il n'a pas du tomber très loin de vous !", texture, app);
        }
        c_NbrLoops++;
    }
}

void Game::save(const std::string & fileName)
{
    std::ofstream file(std::string("saves\\" + fileName).c_str(), std::ios_base::out | std::ios_base::trunc);
    if(!file)
    {
        std::cerr << "ouverture du fichier impossible" << "\n";
        return;
    }

    file << "Maps" << "\n";
    file << c_Maps.size() << "\n";
    file << c_ActualLevel << "\n";
    for(uint16_t i = 0; i < c_Maps.size(); i++)
    {
        file << c_Maps[i]->getMapId() << "\n";
        c_Maps[i]->save(file);
    }

    file << "Player" << "\n";
    file << c_Player->getPosition().x << " " << c_Player->getPosition().y << " ";
    file << c_Player->getName() << " " << c_Player->getLife() << " " << c_Player->getMaxLife() << " " << c_Player->getDirection() << " ";
    file << c_Player->getSpeed() << "\n";
    file << c_Player->getBag()->getNbrItems() << "\n";
    for(uint16_t i = 0; i < c_Player->getBag()->getNbrItems(); i++)
    {
        file << c_Player->getBag()->getItem(i)->getItemId() << " " << c_Player->getBag()->getNbrOfItem(i) << "\n";
    }

    file << "ShortCuts" << "\n";
    file << NBRSLOT << "\n";
    for(uint16_t i = 0; i < NBRSLOT; i++)
    {
        file << c_ItemShortCuts[i] << " " << c_Player->getEquippedItem(i) << "\n";
    }

    file << "end";
    file.close();
}
void Game::load(const std::string & fileName)
{
    std::ifstream file(std::string("saves\\" + fileName).c_str(), std::ios_base::in);
    if(!file)
    {
        std::cout << "chargement de la map enregistrée impossible" << "\n";
        return;
    }

    std::string type;
    do
    {
        file >> type;

        if(type == "Maps")
        {
            uint16_t nbrMaps, actualLevel;
            file >> nbrMaps >> actualLevel;
            uint32_t mapId;
            for(uint16_t i = 0; i < nbrMaps; i++)
            {
                file >> mapId;

                Map * m = new Map(app, app.getSize().x / c_NbrCellsToDraw, mapId);
                m->load(file);
                c_Maps.push_back(m);

                if(i == actualLevel)
                    c_Map = m;
            }
        }

        if(type == "Player")
        {
            uint16_t c, l, life, totalLife, direction;
            float speed;
            std::string name;

            file >> c >> l >> name >> life >> totalLife >> direction >> speed;
            c_Player = new Player(name, totalLife, (Direction)direction, speed, sf::Vector2f(c, l));
            c_Player->takeDamages(totalLife - life);

            uint16_t nbrItems;
            file >> nbrItems;
            uint16_t itemId, nbrOfItem;
            for(uint16_t i = 0; i < nbrItems; i++)
            {
                file >> itemId >> nbrOfItem;
                c_Player->takeItem(Item::getItemFromId((ItemId)itemId), nbrOfItem);
            }
            c_Map->addCharacter(c_Player, c, l);
            c_Map->setFocus(c_Player);
        }

        if(type == "ShortCuts")
        {
            uint16_t nbrShortCuts;
            file >> nbrShortCuts;
            for(uint16_t i = 0; i < nbrShortCuts; i++)
            {
                uint16_t key, itemId;
                file >> key >> itemId;
                c_ItemShortCuts[i] = (sf::Keyboard::Key)key;
                c_Player->setEquippedItem(Item::getItemFromId((ItemId)itemId), i);
            }
        }
    }while(type != "end");
}

void Game::genNextMap(Map * map, Map * lastMap, uint16_t lvl)
{
    GenInfo genInfo;
    genInfo.addObstacleInfos(GRASS, 0, -1);
    genInfo.addObstacleInfos(WATER, 5.5, 1);
    genInfo.addObstacleInfos(SOIL, 7.5, 1);
    genInfo.addObstacleInfos(SAND, 3.25, 2);
    genInfo.addObstacleInfos(ROCK, 2.5, 2);
    genInfo.addObstacleInfos(ROCK, 4.75, 1);
    genInfo.addObstacleInfos(ROCK, 0.5, 2);
    genInfo.addObstacleInfos(FIRE, 1.5, 1);
    genInfo.addStandardizeInfos(WATER, 4);
    genInfo.addStandardizeInfos(SAND, 5);
    genInfo.addStandardizeInfos(SOIL, 5);
    genInfo.addStandardizeInfos(ROCK, 5);
    genInfo.addLivingInfos(SHEEP, 1, 10);
    genInfo.addLivingInfos(WOLF, 1, 4);
    map->generateMap(genInfo, 37, 29);

    for(std::list< Stairs * >::const_iterator it = map->getStairss().begin(); it != map->getStairss().end(); it++)
    {
        if((*it)->isRising())
        {
            (*it)->setLinkedMapId(lastMap->getMapId());
            break;
        }
    }
}

void Game::useItem(Player * p, const Item * item, uint16_t c, uint16_t l)
{
    Living * lAffected = NULL;

    uint16_t dist = abs(p->getPosition().x - c) + abs(p->getPosition().y - l);
    if(dist >= item->getMinUseRange() && dist <= item->getMaxUseRange())
    { // si on peut utiliser l'item a cette distance
        for(uint16_t i = 0; i < item->getNbrEffect(); i++)
        { // on gère chaque effet de l'item
            const Effect * effect = &item->getEffect(i);
            std::list< Cell * > cellsAffected = c_Map->getCellsBetweenDistFromCell(c, l, effect->getMaxEffectRange(), effect->getMinEffectRange());
            for(std::list< Cell * >::iterator it = cellsAffected.begin(); it != cellsAffected.end(); it++)
            {
                if((lAffected = (*it)->getLiving()) != NULL)
                    lAffected->addEffect(effect);
            }
        }

        // gestion pour le type d'objet
        if(item->getItemType() == CONSUMABLE)
        {
            p->removeItem(item, 1);
        }
        p->itemUsed(item);
    }
}
void Game::useWeapon(Player * p, const Item * item, const std::list<Cell *> & cellsF)
{
    Living * lAffected = NULL;

    for(std::list< Cell * >::const_iterator it = cellsF.begin(); it != cellsF.end(); it++)
    {
        for(uint16_t i = 0; i < item->getNbrEffect(); i++)
        { // on gère chaque effet de l'item
            const Effect * effect = &item->getEffect(i);
            std::list< Cell * > cellsAffected = c_Map->getCellsBetweenDistFromCell((*it)->getC(), (*it)->getL(), effect->getMaxEffectRange(), effect->getMinEffectRange());
            for(std::list< Cell * >::iterator it = cellsAffected.begin(); it != cellsAffected.end(); it++)
            {
                if((lAffected = (*it)->getLiving()) != NULL)
                    lAffected->addEffect(effect);
            }
        }
    }
    p->weaponUsed();
}

bool Game::movePlayer(Player * p, Direction d)
{
    uint16_t c = p->getPosition().x;
    uint16_t l = p->getPosition().y;
    int16_t newL = l + (d == DOWN) - (d == UP); // nouvelle colonne
    int16_t newC = c + (d == RIGHT) - (d == LEFT); // nouvelle ligne

    if(newL >= 0 && newL < c_Map->getNbrLine() && newC >= 0 && newC < c_Map->getNbrColumn())
    { // si on ne sort pas de la Map
        Cell * newCell = c_Map->getCell(newC, newL);

        if(newCell->isWalkable() && c_Player->isMoveable())
        {// si l'on peut marcher sur la case
            if(newCell->gotStairs() && ((Stairs*)newCell->getStairs())->isRising() && c_ActualLevel == 0)
            {
                sf::Texture texture;
                texture.create(app.getSize().x, app.getSize().y);
                texture.update(app); // récupère l'affichage actuel pour l'afficher en fond

                std::vector< std::string > answers;
                answers.push_back("Oui");
                answers.push_back("Non");
                c_ChoiceText.newText("Voulez vous vraiment retourner à la surface ?"
                                        , answers, texture, app, true);

                if(c_ChoiceText.getChoice() == 0)
                    c_RawText.newText("Je suis désolé mais ce n'est pas possible.Retourné à votre tâche !", texture, app);
                else
                    c_RawText.newText("C'est bien ! Ne faites pas l'enfant.", texture, app);

                return false;
            }
            else
            {
                c_Map->moveLiving(p, newC, newL);
                return true;
            }
        }
        else
        {
            for(std::list< Obstacle * >::const_iterator it = newCell->getObstacles().begin(); it != newCell->getObstacles().end(); ++it)
            {
                (*it)->touchAction(c_Map, c_Player);
            }
            if(newCell->getLiving() != NULL)
                newCell->getLiving()->touchAction();
        }
    }
    return false;
}

void Game::update(const sf::Time & elapsed)
{
    QuickSand::updateTick(elapsed);
    Fire::updateTick(elapsed);
    c_Map->update(elapsed);
}

void Game::deathManage()
{
    std::list< Monster* > monstersToDelete;
    for(std::list< Monster * >::const_iterator it = c_Map->getMonsters().begin(); it != c_Map->getMonsters().end(); ++it)
    {
        if((*it)->isDead())
        {
            LootBag * lB = (*it)->getLoots();
            lB->setPosition((*it)->getPosition());
            if(lB->getNbrItems() != 0)
            {
                c_Map->addLootBag(lB, (*it)->getPosition().x, (*it)->getPosition().y);
            }
            else
                delete lB;

            if((*it)->getKiller() != NULL)
                std::cout << (*it)->getName() << " est mort par " << (*it)->getKiller()->getEntityId() << "\n";

            monstersToDelete.push_back((*it));
        }
    }
    for(std::list< Monster * >::iterator it = monstersToDelete.begin(); it != monstersToDelete.end(); it++)
    {
        c_Map->removeMonster((*it));
        delete (*it);
    }
}

void Game::eventManage()
{
    sf::Event event;
    while (app.pollEvent(event))
    {
        c_Menu.manage(event, c_Map, c_Player);

        switch(event.type)
        {
        case sf::Event::Closed:
            app.close();
            break;

        case sf::Event::Resized:
                app.setSize(sf::Vector2u(800, 600));
            break;

        case sf::Event::KeyPressed:
            switch(event.key.code)
            {
            case sf::Keyboard::F:
                if(!c_Menu.isShowingInventory() && !c_Menu.isShowingLootBag() && !c_Menu.isShowingCellChoice())
                    actionEventTest();
                break;

            case sf::Keyboard::J:
                std::cout << "save" << "\n";
                save("TestSave.txt");
                break;

            case sf::Keyboard::E:
                c_Menu.setShowingInventory();
            break;

            default:
                break;
            }
            break;

        default:
            break;
        }
    }

    sf::Keyboard keyboard;
    sf::Mouse mouse;

    if(!c_Menu.isShowingInventory() && !c_Menu.isShowingLootBag())
    {
        if(keyboard.isKeyPressed(sf::Keyboard::Z))
        {
            if(mouse.isButtonPressed(sf::Mouse::Right))
            {
                if(c_Player->getDirection() != DOWN)
                    movePlayer(c_Player, UP);
            }
            else if(c_Player->isMoveable())
            {
                if(c_Player->getDirection() != UP)
                    c_Player->setDirection(UP);
                else
                   movePlayer(c_Player, UP);
            }
        }
        else if(keyboard.isKeyPressed(sf::Keyboard::S))
        {
            if(mouse.isButtonPressed(sf::Mouse::Right))
            {
                if(c_Player->getDirection() != UP)
                    movePlayer(c_Player, DOWN);
            }
            else if(c_Player->isMoveable())
            {
                if(c_Player->getDirection() != DOWN)
                    c_Player->setDirection(DOWN);
                else
                    movePlayer(c_Player, DOWN);
            }
        }
        else if(keyboard.isKeyPressed(sf::Keyboard::Q))
        {
            if(mouse.isButtonPressed(sf::Mouse::Right))
            {
                if(c_Player->getDirection() != RIGHT)
                    movePlayer(c_Player, LEFT);
            }
            else if(c_Player->isMoveable())
            {
                if(c_Player->getDirection() != LEFT)
                c_Player->setDirection(LEFT);
                else
                    movePlayer(c_Player, LEFT);
            }
        }
        else if(keyboard.isKeyPressed(sf::Keyboard::D))
        {
            if(mouse.isButtonPressed(sf::Mouse::Right))
            {
                if(c_Player->getDirection() != LEFT)
                    movePlayer(c_Player, RIGHT);
            }
            else if(c_Player->isMoveable())
            {
                if(c_Player->getDirection() != RIGHT)
                    c_Player->setDirection(RIGHT);
                else
                    movePlayer(c_Player, RIGHT);
            }
        }

        for(uint16_t i = 0; i < NBRSLOT; ++i)
        {
            if(keyboard.isKeyPressed(c_ItemShortCuts[i]))
            {
                if(c_Player->getBagIndexOfEquippedItem(i) != -1 && c_Player->canUseItem(c_Player->getEquippedItem(i)))
                    c_Menu.setShowingCellChoice(c_Player->getBagIndexOfEquippedItem(i));
            }
        }
        if(keyboard.isKeyPressed(sf::Keyboard::Space))
        {
            if(c_Player->getWeaponEquipped() != NULL && c_Player->canUseWeapon())
                useWeapon(c_Player, c_Player->getWeaponEquipped(),
                            c_Player->getWeaponEquipped()->getTargetableCells(c_Map, c_Player, c_Player->getPosition().x, c_Player->getPosition().y));
        }
    }
}

void Game::realTimeEventManage()
{
    for(std::list< Living * >::const_iterator it = c_Map->getLivings().begin(); it != c_Map->getLivings().end(); ++it)
    {
        (*it)->realTimeAction(c_Map, c_Player);
    }
    for(std::list< Obstacle * >::const_iterator it = c_Map->getObstacles().begin(); it != c_Map->getObstacles().end(); ++it)
    {
        (*it)->realTimeAction(c_Map, c_Player);
    }
}

void Game::walkEventTest()
{
    for(std::list< Living * >::const_iterator it = c_Map->getLivings().begin(); it != c_Map->getLivings().end(); ++it)
    {
        Cell * cell = c_Map->getCell((*it)->getPosition()); // cell sur laquelle est le Living
        for(std::list< Obstacle * >::const_iterator it2 = cell->getObstacles().begin(); it2 != cell->getObstacles().end(); ++it2)
        {
            (*it2)->walkAction(c_Map, (*it));
        }
    }
}

void Game::actionEventTest()
{
    Cell * focusedCell = NULL;
    switch(c_Player->getDirection())
    {
    case RIGHT:
        focusedCell = c_Map->getRCell(c_Player->getPosition().x, c_Player->getPosition().y);
        break;

    case LEFT:
        focusedCell = c_Map->getLCell(c_Player->getPosition().x, c_Player->getPosition().y);
        break;

    case UP:
        focusedCell = c_Map->getUCell(c_Player->getPosition().x, c_Player->getPosition().y);
        break;

    case DOWN:
        focusedCell = c_Map->getDCell(c_Player->getPosition().x, c_Player->getPosition().y);
        break;
    }

    if(focusedCell == NULL)
        return;

    LootBag * lB = focusedCell->getLootBag();
    const std::list< Obstacle * > & os = focusedCell->getObstacles();
    if(focusedCell->getLiving() != NULL) // priorité au entité vivantes
        focusedCell->getLiving()->speakAction();
    else if(lB != NULL) // puis aux lootBag
        c_Menu.setShowingLootBag(lB);
    else if(os.size() != 0) // et enfin aux obstacles
    {
        for(std::list< Obstacle * >::const_iterator it = os.begin(); it != os.end(); ++it)
        {
            (*it)->speakAction(c_Map, c_Player);
        }
    }
}




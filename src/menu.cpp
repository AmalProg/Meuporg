#include "menu.hpp"

Menu::Menu(sf::RenderWindow & a) : app(a), c_CharacterSize(30),
c_ShowingInventory(false), c_InventorySelected(-1), c_InventoryFirst(0), c_InventoryNbrShown(100), c_InventoryNbrItems(0),
c_ShowingItemMenu(false), c_ItemMenuSelected(-1), c_ItemToDestroy(-1), c_ItemToUse(-1), c_ItemToEquip(-1),
c_ShowingCellChoice(false), c_CellToFocus(NULL),
c_ShowingShortCutChoice(false), c_ShortCutSelected(0), c_ItemToShortCut(-1), c_KeyToShortCut(sf::Keyboard::Unknown),
c_ShowingLootBag(false), c_LootBag(NULL), c_LootBagSelected(-1), c_LootBagFirst(0), c_LootBagNbrShown(100), c_LootBagNbrItems(0)
{
    c_ItemToTake.item = NULL;
    c_ItemToTake.nbr = 0;

    arial.loadFromFile("arial.ttf");
    itemsFont.loadFromFile("font\\FantaisieArtistique.ttf");
    menuFont.loadFromFile("font\\mops.ttf");

    if(c_InventoryTexture.loadFromFile("image\\menu\\basicMenu.png"))
        c_InventorySprite.setTexture(c_InventoryTexture);

    if(c_LootBagTexture.loadFromFile("image\\menu\\basicMenu.png"))
        c_LootBagSprite.setTexture(c_LootBagTexture);
    if(c_ItemInfosTexture.loadFromFile("image\\menu\\itemMenu.png"))
        c_ItemInfosSprite.setTexture(c_ItemInfosTexture);

    if(c_ItemMenuTexture.loadFromFile("image\\menu\\itemMenu.png"))
    {
        c_ItemMenuSprite.setTexture(c_ItemMenuTexture);

        c_ItemMenuTexts.push_back(sf::Text(sf::String("Utilisez"), menuFont, 30));
        c_ItemMenuTexts.push_back(sf::Text(sf::String("Raccourci"), menuFont, 30));
        c_ItemMenuTexts.push_back(sf::Text(sf::String("Détruire"), menuFont, 30));
    }

    if(c_ShortCutTexture.loadFromFile("image\\shortCut.jpg"))
    {
        for(uint16_t i = 0; i < NBRSLOT; ++i)
        {
            c_ShortCutSprite[i].setTexture(c_ShortCutTexture);
        }
    }
}

void Menu::draw(const Map * m, const Player * p)
{
    sf::View lastView = app.getView();
    app.setView(sf::View(sf::FloatRect(0, 0, 800, 600)));

    if(c_ShowingInventory)
        inventoryMenu(p);
    if(c_ShowingItemMenu)
        itemMenu(p);
    if(c_ShowingShortCutChoice)
        shortCutMenu(m);
    if(c_ShowingLootBag)
        lootbagMenu();

    app.setView(m->getView());

    if(c_ShowingCellChoice)
        cellChoiceMenu(m, p);

    app.setView(lastView);
}

void Menu::manage(sf::Event & event, const Map * m, const Player * p)
{
    sf::Mouse mouse;

    float viewMultX = m->getView().getSize().x / (float)app.getSize().x;
    float viewMultY = m->getView().getSize().y / (float)app.getSize().y;
    float cellSizeDrawnX = (float)m->getCellSize() / viewMultX;
    float cellSizeDrawnY = (float)m->getCellSize() / viewMultY;
    float mapSizeDrawnX = m->getView().getSize().x / 2.f ;
    float mapSizeDrawnY = m->getView().getSize().y / 2.f;
    int32_t x = ((float)mouse.getPosition(app).x + (m->getView().getCenter().x - mapSizeDrawnX) / viewMultX) / cellSizeDrawnX;
    int32_t y = ((float)mouse.getPosition(app).y + (m->getView().getCenter().y - mapSizeDrawnY) / viewMultY) / cellSizeDrawnY;
    // coordonnées en terme de cell de la souris

    switch(event.type)
    {
    case sf::Event::MouseWheelMoved:
        if(c_ShowingLootBag)
        {
            if(c_LootBagFirst + event.mouseWheel.delta < 0)
                c_LootBagFirst = 0;
            else if(c_LootBagFirst + event.mouseWheel.delta > c_LootBagNbrItems - c_LootBagNbrShown)
                c_LootBagFirst = c_LootBagNbrItems - c_LootBagNbrShown;
            else
                c_LootBagFirst += event.mouseWheel.delta;
        }
        else if(c_ShowingInventory && !c_ShowingItemMenu)
        {
            if(c_InventoryFirst + event.mouseWheel.delta < 0)
                c_InventoryFirst = 0;
            else if(c_InventoryFirst + event.mouseWheel.delta > c_InventoryNbrItems - c_InventoryNbrShown)
                c_InventoryFirst = c_InventoryNbrItems - c_InventoryNbrShown;
            else
                c_InventoryFirst += event.mouseWheel.delta;
        }
        break;

    case sf::Event::KeyPressed:
        switch(event.key.code)
        {
            case sf::Keyboard::Escape:
                if(c_ShowingLootBag)
                    c_ShowingLootBag = false;
                else if(c_ShowingShortCutChoice)
                    c_ShowingShortCutChoice = false;
                else if(c_ShowingCellChoice)
                    c_ShowingCellChoice = false;
                else if(c_ShowingItemMenu)
                    c_ShowingItemMenu = false;
                else if(c_ShowingInventory)
                    c_ShowingInventory = false;

                break;

            default:
                break;
        }
        break;

    case sf::Event::MouseButtonReleased:
    {
        switch(event.mouseButton.button)
        {
            case sf::Mouse::Left:
                if(c_ShowingLootBag)
                {
                    if(c_LootBagSelected != -1 && c_LootBagRects[c_LootBagSelected].contains(mouse.getPosition(app).x, mouse.getPosition(app).y))
                    {
                        c_ItemToTake.item = c_LootBag->getItem(c_LootBagSelected);
                        c_ItemToTake.nbr = c_LootBag->getNbrOfItem(c_LootBagSelected);

                        // on enlève l'item du sac
                        c_LootBag->removeItem(c_LootBagSelected, c_LootBag->getNbrOfItem(c_LootBagSelected));
                        if(c_LootBag->getNbrItems() == 0) // si plus d'objets on ne regard eplus le sac
                        {
                            c_ShowingLootBag = false;
                            c_LootBagNbrShown = 100;
                        }
                    }
                }
                else if(c_ShowingShortCutChoice)
                {
                    if(c_ShortCutSprite[c_ShortCutSelected].getGlobalBounds().contains(mouse.getPosition(app).x, mouse.getPosition(app).y))
                    {
                        c_ItemToShortCut = c_InventorySelected;
                        if(c_ShortCutSelected == 0)
                            c_KeyToShortCut = sf::Keyboard::Num1;
                        else if(c_ShortCutSelected == 1)
                            c_KeyToShortCut = sf::Keyboard::Num2;
                        else if(c_ShortCutSelected == 2)
                            c_KeyToShortCut = sf::Keyboard::Num3;
                        else if(c_ShortCutSelected == 3)
                            c_KeyToShortCut = sf::Keyboard::Num4;


                        c_ShowingInventory = false;
                        c_ShowingItemMenu = false;
                        c_ShowingShortCutChoice = false;
                    }
                }
                else if(c_ShowingCellChoice and c_CellChoiceSelected != NULL)
                { // choix d'une cellule de la map
                    if(c_CellChoiceSelected->getC() == x && c_CellChoiceSelected->getL() == y)
                    {
                        c_ItemToUse = c_InventorySelected;
                        c_CellToFocus = c_CellChoiceSelected;

                        c_ShowingCellChoice = false;
                    }
                }
                else if(c_ShowingItemMenu)
                { // choix de ce que l'on veut faire de l'item
                    if(c_ItemMenuSelected != -1 && c_ItemMenuRects[c_ItemMenuSelected].contains(mouse.getPosition(app).x, mouse.getPosition(app).y))
                    {
                        if(c_ItemMenuSelected == 0)
                        {
                            if(p->getItem(c_InventorySelected)->getItemType() != EQUIPMENT
                               && p->canUseItem(p->getItem(c_InventorySelected)))
                            {
                                c_ShowingInventory = false;
                                c_ShowingItemMenu = false;
                                c_ShowingCellChoice = true;
                            }
                            else
                                std::cout << "Cet item est en CD !" << "\n";

                            if(p->getItem(c_InventorySelected)->getItemType() == EQUIPMENT)
                            {
                                c_ShowingInventory = false;
                                c_ShowingItemMenu = false;
                                c_ItemToEquip = c_InventorySelected;
                            }
                            else
                                std::cout << "Cet item est en CD !" << "\n";

                        }
                        if(c_ItemMenuSelected == 1)
                        {
                            c_ShowingShortCutChoice = true;
                        }
                        if(c_ItemMenuSelected == 2)
                        {
                            c_ItemToDestroy = c_InventorySelected;
                            c_ShowingItemMenu = false;
                        }
                    }
                }
                else if(c_ShowingInventory) // choix d'un item dans l'inventaire
                    if(c_InventoryNbrItems > 0 && c_InventorySelected != -1 && c_InventoryRects[c_InventorySelected].contains(mouse.getPosition(app).x, mouse.getPosition(app).y))
                        c_ShowingItemMenu = true;

                break;
            case sf::Mouse::Right:
                if(c_ShowingLootBag)
                    c_ShowingLootBag = false;
                else if(c_ShowingShortCutChoice)
                    c_ShowingShortCutChoice = false;
                else if(c_ShowingCellChoice)
                    c_ShowingCellChoice = false;
                else if(c_ShowingItemMenu)
                    c_ShowingItemMenu = false;
                else if(c_ShowingInventory)
                    c_ShowingInventory = false;

                break;

            default:
                break;
        }
        break;
    }break;

    default:
        break;
    }

    if(c_ShowingLootBag)
    {
        c_LootBagSelected = -1;
        for(uint16_t i = 0; i < c_LootBagRects.size(); ++i)
        {
            if(c_LootBagRects[i].contains(mouse.getPosition(app).x, mouse.getPosition(app).y))
            {
                c_LootBagSelected = i + c_LootBagFirst;
            }
        }
    }
    else if(c_ShowingShortCutChoice)
    {
        for(uint16_t i = 0; i < NBRSLOT; ++i)
        {
            if(c_ShortCutSprite[i].getGlobalBounds().contains(mouse.getPosition(app).x, mouse.getPosition(app).y))
                c_ShortCutSelected = i;
        }
    }
    else if(c_ShowingCellChoice)
    {
        bool focused = false;
        for(std::list< Cell * >::iterator it = c_AffectedCells.begin(); it != c_AffectedCells.end(); it++)
        {
            if(x == (*it)->getC() && y == (*it)->getL())
            {
                c_CellChoiceSelected = *it;
                focused = true;
            }
        }

        if(!focused) // si aucune cellule visée
            c_CellChoiceSelected = NULL;

    }
    else if(c_ShowingItemMenu)
    {
        c_ItemMenuSelected = -1;
        for(uint16_t i = 0; i < c_ItemMenuRects.size(); ++i)
        {
            if(c_ItemMenuRects[i].contains(mouse.getPosition(app).x, mouse.getPosition(app).y))
                c_ItemMenuSelected = i;
        }
    }
    else if(c_ShowingInventory)
    {
        c_InventorySelected = -1;
        for(uint16_t i = 0; i < c_InventoryRects.size(); ++i)
        {
            if(c_InventoryRects[i].contains(mouse.getPosition(app).x, mouse.getPosition(app).y))
            {
                c_InventorySelected = i + c_InventoryFirst;
            }
        }
    }
}

void Menu::shortCutMenu(const Map * m)
{
    for(uint16_t i = 0; i < NBRSLOT; ++i)
    {
        if(i == c_ShortCutSelected)
            c_ShortCutSprite[i].setColor(sf::Color::Red);
        else
            c_ShortCutSprite[i].setColor(sf::Color(150, 150, 150));
        c_ShortCutSprite[i].setPosition(m->getCellSize()/2 + i * m->getCellSize(), m->getCellSize()/2);

        app.draw(c_ShortCutSprite[i]);
    }
}

void Menu::lootbagMenu()
{
    sf::View lastView = app.getView();
    app.setView(sf::View(sf::FloatRect(0, 0, app.getSize().x, app.getSize().y)));
    c_CharacterSize = app.getSize().x / 40;

    c_LootBagSprite.setScale(app.getSize().x * 3/10 / c_LootBagSprite.getLocalBounds().width,
                               app.getSize().y * 6/10 / c_LootBagSprite.getLocalBounds().height);
    c_LootBagSprite.setPosition(app.getSize().x / 2 - c_LootBagSprite.getLocalBounds().width / 2,
                                    app.getSize().y / 2 - c_LootBagSprite.getLocalBounds().height / 2);
    app.draw(c_LootBagSprite);

    uint16_t posX = c_LootBagSprite.getPosition().x;
    uint16_t posY = c_LootBagSprite.getPosition().y;
    uint16_t width = c_LootBagSprite.getGlobalBounds().width - c_LootBagSprite.getGlobalBounds().width / 31; // width sans ombre
    uint16_t height = c_LootBagSprite.getGlobalBounds().height - c_LootBagSprite.getGlobalBounds().height / 50; // height sans ombre

    uint16_t lastTextHeightSize = 0; // hauteur du dernier texte affiché
    uint16_t lastTextHeightPosition = posY + height / 27; // position du dernier texte affiché
    c_LootBagNbrItems = c_LootBag->getNbrItems();
    c_LootBagRects.clear();

    uint16_t gapHeight = height * 1/27;

    for(uint16_t i = c_LootBagFirst; i < c_LootBagNbrItems - c_LootBagFirst; ++i)
    { // affichage de tous les items du bag
        sf::Text text(sf::String(c_LootBag->getItem(i)->getName()), itemsFont, c_CharacterSize);
        text.setOrigin(text.getLocalBounds().left, text.getLocalBounds().top + text.getLocalBounds().height/2);
        text.setPosition(posX + width / 18, height * 1/27 + lastTextHeightPosition + lastTextHeightSize + ((gapHeight - lastTextHeightSize) * ((i - c_LootBagFirst) != 0)));

        if(i == c_LootBagSelected)
            text.setColor(sf::Color::Red);
        else
            text.setColor(sf::Color::Black);

        sf::Text nbrT(nbrToString(c_LootBag->getNbrOfItem(i)), itemsFont, c_CharacterSize);
        nbrT.setOrigin(nbrT.getLocalBounds().left, nbrT.getLocalBounds().top + nbrT.getLocalBounds().height/2);
        nbrT.setPosition(posX + width * 15/18 - width / 18 / 2 - nbrT.getGlobalBounds().width / 2
                         , height * 1/27 + lastTextHeightPosition + lastTextHeightSize + ((gapHeight - lastTextHeightSize) * ((i - c_LootBagFirst) != 0)));
        nbrT.setColor(sf::Color::Black);

        sf::IntRect rect(text.getPosition().x, text.getPosition().y - gapHeight/2
                            , width * 11/18, gapHeight);

        lastTextHeightPosition = text.getPosition().y;
        lastTextHeightSize = text.getGlobalBounds().height;
        if(lastTextHeightPosition + lastTextHeightSize >= posY + height * 26/27)
        {
            c_LootBagNbrShown = i-1; // on s'arrête avant d'écrire n'importe ou
            break;
        }

        c_LootBagRects.push_back(rect); // rectangle de collision

        app.draw(text); // nom de l'item
        app.draw(nbrT); // nombre d'items
    }

    app.setView(lastView);
}

void Menu::cellChoiceMenu(const Map * m, const Player * p)
{
    static sf::RectangleShape outline(sf::Vector2f(m->getCellSize() * 7/10, m->getCellSize() * 7/10));
    outline.setOutlineColor(sf::Color::Red);
    outline.setOutlineThickness(3);

    const Item * item = p->getBag()->getItem(c_InventorySelected);
    const sf::Vector2f  pPos = p->getPosition();
    Cell * playerCell = m->getCell(pPos.x, pPos.y);

    c_AffectedCells.clear();
    std::list< Cell * > canBeFocusedCells = item->getTargetableCells(m, p, pPos.x, pPos.y);
    for(std::list< Cell * >::iterator it = canBeFocusedCells.begin(); it != canBeFocusedCells.end(); it++)
    {
        if(m->lineOfSight(playerCell, (*it)))
        {
            outline.setPosition(m->getCellSize() * (*it)->getC() + m->getCellSize() * 15/100, m->getCellSize() * (*it)->getL() + m->getCellSize() * 15/100);
            if((*it) == c_CellChoiceSelected)
                outline.setFillColor(sf::Color::Red);
            else
                outline.setFillColor(sf::Color(0, 0, 0, 0));

            app.draw(outline);

            c_AffectedCells.push_back((*it));
        }
    }
}

void Menu::itemMenu(const Player * p)
{
    sf::View lastView = app.getView();
    app.setView(sf::View(sf::FloatRect(0, 0, app.getSize().x, app.getSize().y)));
    c_CharacterSize = app.getSize().x / 40;

    c_ItemMenuSprite.setScale(app.getSize().x * 2/10 / c_ItemMenuSprite.getLocalBounds().width,
                               app.getSize().y * 2/10 / c_ItemMenuSprite.getLocalBounds().height);
    c_ItemMenuSprite.setPosition(c_InventoryRects[c_InventorySelected].left - c_ItemMenuSprite.getGlobalBounds().width * 7/8
                                 , c_InventoryRects[c_InventorySelected].top + c_InventoryRects[c_InventorySelected].height * 2/3);// place le menu au niveau de l'affichage de l'item selectionné
    app.draw(c_ItemMenuSprite);

    uint16_t posX = c_ItemMenuSprite.getPosition().x;
    uint16_t posY = c_ItemMenuSprite.getPosition().y;
    uint16_t width = c_ItemMenuSprite.getGlobalBounds().width - c_ItemMenuSprite.getGlobalBounds().width / 27; // width sans ombre
    uint16_t height = c_ItemMenuSprite.getGlobalBounds().height - c_ItemMenuSprite.getGlobalBounds().height / 19; // height sans ombre

    c_ItemMenuRects.clear();
    uint16_t nbrTextToDraw = 3;
    if(!p->getItem(c_InventorySelected)->isUsable())
    {
        c_ItemMenuRects.push_back(sf::IntRect(0,0,0,0));
        nbrTextToDraw = 2;
    }
    uint16_t heighToDraw = height / (nbrTextToDraw + 1);

    uint16_t k =0;
    for(uint16_t i = c_ItemMenuTexts.size() - nbrTextToDraw; i < c_ItemMenuTexts.size(); ++i)
    {
        if(i == 0)
        {
            if(!p->getItem(c_InventorySelected)->isUsable())
                continue; // si l'item n'st pas utilisable on affiche que le choix de destruction d el'item
            else if(p->getItem(c_InventorySelected)->getItemType() == EQUIPMENT )
                c_ItemMenuTexts[i].setString(sf::String("Equipper")); // si un equipement on n'affiche pas le choix utiliser
            else
                c_ItemMenuTexts[i].setString(sf::String("Utilisez"));
        }

        c_ItemMenuTexts[i].setPosition(posX + width / 17, posY + heighToDraw * (k+1) - c_ItemMenuTexts[i].getLocalBounds().height/2 - c_ItemMenuTexts[i].getLocalBounds().top);
        c_ItemMenuTexts[i].setCharacterSize(c_CharacterSize);

        if(i == c_ItemMenuSelected)
            c_ItemMenuTexts[i].setColor(sf::Color::Red);
        else
            c_ItemMenuTexts[i].setColor(sf::Color::Black);

        c_ItemMenuRects.push_back(sf::IntRect(c_ItemMenuTexts[i].getPosition().x, c_ItemMenuTexts[i].getPosition().y
                            , width * 16/17, height * 2.5/10)); // rectangle de collision

        app.draw(c_ItemMenuTexts[i]);

        k++;
    }

    app.setView(lastView);
}

void Menu::inventoryMenu(const Player * p)
{
    sf::View lastView = app.getView();
    app.setView(sf::View(sf::FloatRect(0, 0, app.getSize().x, app.getSize().y)));
    c_CharacterSize = app.getSize().x / 40;

    c_InventorySprite.setScale(app.getSize().x * 3/10 / c_InventorySprite.getLocalBounds().width,
                               app.getSize().y * 6/10 / c_InventorySprite.getLocalBounds().height);
    c_InventorySprite.setPosition(app.getSize().x * 6/10, app.getSize().y * 2/10);
    app.draw(c_InventorySprite);
    uint16_t width = c_InventorySprite.getGlobalBounds().width - c_InventorySprite.getGlobalBounds().width / 31; // width sans ombre
    uint16_t height = c_InventorySprite.getGlobalBounds().height - c_InventorySprite.getGlobalBounds().height / 50; // height sans ombre
    uint16_t posX = c_InventorySprite.getPosition().x + width / 18;
    uint16_t posY = c_InventorySprite.getPosition().y + height / 27;

    const Bag * bag = p->getBag();
    uint16_t lastTextHeightSize = 0; // hauteur du dernier texte affiché
    uint16_t lastTextHeightPosition = posY; // position du dernier texte affiché
    c_InventoryNbrItems = bag->getNbrItems();
    c_InventoryRects.clear();

    uint16_t gapHeight = height * 1/27;

    for(uint16_t i = c_InventoryFirst; i < c_InventoryNbrItems - c_InventoryFirst; ++i)
    { // affichage de tous les items du bag
        const Item * item = bag->getItem(i);
        sf::Text text(sf::String(item->getName()), itemsFont, c_CharacterSize);
        text.setOrigin(text.getLocalBounds().left, text.getLocalBounds().top + text.getLocalBounds().height/2);
        text.setPosition(posX, height * 1/27 + lastTextHeightPosition + lastTextHeightSize + ((gapHeight - lastTextHeightSize) * ((i - c_InventoryFirst) != 0)));

        if(i == c_InventorySelected)
        {
            c_ItemInfosSprite.setScale(app.getSize().x * 2/10 / c_ItemInfosSprite.getLocalBounds().width,
                               app.getSize().y * 2/10 / c_ItemInfosSprite.getLocalBounds().height);
            c_ItemInfosSprite.setPosition(c_InventoryRects[c_InventorySelected].left - c_ItemInfosSprite.getGlobalBounds().width * 7/8
                                 , c_InventoryRects[c_InventorySelected].top - c_ItemInfosSprite.getGlobalBounds().height);// place le menu au niveau de l'affichage de l'item selectionné
            app.draw(c_ItemInfosSprite);

            uint16_t widthB = c_ItemInfosSprite.getGlobalBounds().width - c_ItemInfosSprite.getGlobalBounds().width / 27; // width sans ombre
            uint16_t heightB = c_ItemInfosSprite.getGlobalBounds().height - c_ItemInfosSprite.getGlobalBounds().height / 19; // height sans ombre
            uint16_t posXB = c_ItemInfosSprite.getPosition().x + widthB / 17;
            uint16_t posYB = c_ItemInfosSprite.getPosition().y + heightB / 10;

            sf::Color color(0, 0, 200); // couleur des infos

            sf::Text typeT("Type :", menuFont, c_CharacterSize * 2/3);
            sf::Text type("", menuFont, c_CharacterSize * 2/3);
            sf::Text dirT("Directional :", menuFont, c_CharacterSize * 2/3);
            sf::Text dir("", menuFont, c_CharacterSize * 2/3);
            sf::Text zoneTypeT("Zone :", menuFont, c_CharacterSize * 2/3);
            sf::Text zoneType("", menuFont, c_CharacterSize * 2/3);
            switch(item->getItemType())
            {
                case EQUIPMENT: type.setString("Equipment"); break;
                case CONSUMABLE: type.setString("Consumable"); break;
                case TOOL: type.setString("Tool"); break;
            }
            switch(item->isDirectional())
            {
                case true: dir.setString("Yes"); break;
                case false: dir.setString("No"); break;
            }
            switch(item->getZoneType())
            {
                case AOE: zoneType.setString("AOE"); break;
                case LINE: zoneType.setString("Line"); break;
            }
            typeT.setOrigin(typeT.getLocalBounds().left, typeT.getLocalBounds().top + typeT.getLocalBounds().height/2);
            type.setOrigin(type.getLocalBounds().left, type.getLocalBounds().top + type.getLocalBounds().height/2);
            dirT.setOrigin(dirT.getLocalBounds().left, dirT.getLocalBounds().top + dirT.getLocalBounds().height/2);
            dir.setOrigin(dir.getLocalBounds().left, dir.getLocalBounds().top + dir.getLocalBounds().height/2);
            zoneTypeT.setOrigin(zoneTypeT.getLocalBounds().left, zoneTypeT.getLocalBounds().top + zoneTypeT.getLocalBounds().height/2);
            zoneType.setOrigin(zoneType.getLocalBounds().left, zoneType.getLocalBounds().top + zoneType.getLocalBounds().height/2);
            typeT.setPosition(posXB, posYB + heightB * 1/10);
            type.setPosition(posXB + widthB * 16/18 - type.getGlobalBounds().width, posYB + heightB * 1/10);
            dirT.setPosition(posXB, posYB + heightB * 2/10);
            dir.setPosition(posXB + widthB * 16/18 - dir.getGlobalBounds().width, posYB + heightB * 2/10);
            zoneTypeT.setPosition(posXB, posYB + heightB * 3/10);
            zoneType.setPosition(posXB + widthB * 16/18 - zoneType.getGlobalBounds().width, posYB + heightB * 3/10);
            typeT.setColor(sf::Color::Black);
            type.setColor(color);
            dirT.setColor(sf::Color::Black);
            dir.setColor(color);
            zoneTypeT.setColor(sf::Color::Black);
            zoneType.setColor(color);

            sf::Text rangeT("Range :", menuFont, c_CharacterSize * 2/3);
            sf::Text range(nbrToString(item->getMinUseRange()) + " - " + nbrToString(item->getMaxUseRange()), menuFont, c_CharacterSize * 2/3);
            rangeT.setOrigin(rangeT.getLocalBounds().left, rangeT.getLocalBounds().top + rangeT.getLocalBounds().height/2);
            range.setOrigin(range.getLocalBounds().left, range.getLocalBounds().top + range.getLocalBounds().height/2);
            rangeT.setPosition(posXB, posYB + heightB * 4/10);
            range.setPosition(posXB + widthB * 16/18 - range.getGlobalBounds().width, posYB + heightB * 4/10);
            rangeT.setColor(sf::Color::Black);
            range.setColor(color);

            for(uint16_t i = 0; i < item->getNbrEffect(); i++)
            {
                sf::Text eType("", menuFont, c_CharacterSize * 2/3);
                sf::Text value(nbrToString(item->getEffect(i).value), menuFont, c_CharacterSize * 2/3);
                sf::Text eRange(nbrToString(item->getEffect(i).minEffectRange) + " - " + nbrToString(item->getEffect(i).maxEffectRange), menuFont, c_CharacterSize * 2/3);

                switch(item->getEffect(i).type)
                {
                    case HEAL: eType.setString("Heal : ");
                        value.setColor(sf::Color(0, 100, 0));
                        break;
                    case DAMAGE: eType.setString("Damage : ");
                        value.setColor(sf::Color(200, 0, 0));
                        break;
                }
                eType.setOrigin(eType.getLocalBounds().left, eType.getLocalBounds().top + eType.getLocalBounds().height/2);
                value.setOrigin(value.getLocalBounds().left, value.getLocalBounds().top + value.getLocalBounds().height/2);
                eRange.setOrigin(eRange.getLocalBounds().left, eRange.getLocalBounds().top + eRange.getLocalBounds().height/2);
                eType.setPosition(posXB, range.getPosition().y + heightB * (2+i)/10);
                value.setPosition(posXB + eType.getGlobalBounds().width + widthB * 1/18, range.getPosition().y + heightB * (2+i)/10);
                eRange.setPosition(posXB + widthB * 16/18 - eRange.getGlobalBounds().width, range.getPosition().y + heightB * (2+i)/10);
                eType.setColor(sf::Color::Black);
                eRange.setColor(color);

                app.draw(eType);
                app.draw(value);
                app.draw(eRange);
            }

            app.draw(typeT);
            app.draw(type);
            app.draw(dirT);
            app.draw(dir);
            app.draw(zoneTypeT);
            app.draw(zoneType);
            app.draw(rangeT);
            app.draw(range);

            text.setColor(sf::Color::Red);
        }
        else
            text.setColor(sf::Color::Black);

        sf::Text nbrT(nbrToString(bag->getNbrOfItem(i)), itemsFont, c_CharacterSize);
        nbrT.setOrigin(nbrT.getLocalBounds().left, nbrT.getLocalBounds().top + nbrT.getLocalBounds().height/2);
        nbrT.setPosition(posX + width * 15/18 - width / 18 / 2 - nbrT.getGlobalBounds().width / 2
                         , height * 1/27 + lastTextHeightPosition + lastTextHeightSize + (gapHeight - lastTextHeightSize) * ((i - c_InventoryFirst) != 0));
        nbrT.setColor(sf::Color::Black);

        sf::IntRect rect(text.getPosition().x, text.getPosition().y - gapHeight/2
                            , width * 11/18, gapHeight);

        lastTextHeightPosition = text.getPosition().y;
        lastTextHeightSize = text.getGlobalBounds().height;
        if(lastTextHeightPosition + lastTextHeightSize >= posY + height * 25/27)
        {
            c_InventoryNbrShown = i-1; // on s'arrête avant d'écrire n'importe ou
            break;
        }

        c_InventoryRects.push_back(rect); // rectangle de collision

        app.draw(text); // nom de l'item
        app.draw(nbrT); // nombre d'items
    }

    app.setView(lastView);
}

void Menu::setShowingLootBag(LootBag * lB)
{
    if(!c_ShowingCellChoice && !c_ShowingItemMenu && !c_ShowingItemMenu && !c_ShowingInventory)
    {
        c_LootBag = lB;
        c_ShowingLootBag = true;
    }
}
void Menu::setShowingCellChoice(int16_t i)
{
    if(!c_ShowingInventory && !c_ShowingItemMenu && !c_ShowingItemMenu && !c_ShowingLootBag)
    {
        c_ShowingCellChoice = true;
        c_InventorySelected = i;
    }
}
void Menu::setShowingInventory()
{
    if(!c_ShowingCellChoice && !c_ShowingItemMenu && !c_ShowingItemMenu && !c_ShowingLootBag)
    {
        c_ShowingInventory = true;
    }
}

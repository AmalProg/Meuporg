#include "menu.hpp"

Menu::Menu(sf::RenderWindow & a) : app(a),
c_ShowingInventory(false), c_InventorySelected(0), c_InventoryFirst(0), c_InventoryNbrShown(100), c_InventoryNbrItems(0),
c_ShowingItemMenu(false), c_ItemMenuSelected(0), c_ItemToDestroy(-1), c_ItemToUse(-1), c_ItemToEquip(-1),
c_ShowingCellChoice(false), c_CellToFocus(NULL),
c_ShowingShortCutChoice(false), c_ShortCutSelected(0), c_ItemToShortCut(-1), c_KeyToShortCut(sf::Keyboard::Unknown),
c_ShowingLootBag(false), c_LootBag(NULL), c_LootBagSelected(0), c_LootBagFirst(0), c_LootBagNbrShown(100), c_LootBagNbrItems(0)
{
    c_ItemToTake.item = NULL;
    c_ItemToTake.nbr = 0;


    if(c_InventoryTexture.loadFromFile("image\\inventoryMenu.jpg"))
        c_InventorySprite.setTexture(c_InventoryTexture);

    if(c_LootBagTexture.loadFromFile("image\\LootBagMenu.jpg"))
    {
        c_LootBagSprite.setTexture(c_LootBagTexture);
    }

    if(c_ItemMenuTexture.loadFromFile("image\\itemMenu.jpg"))
    {
        c_ItemMenuSprite.setTexture(c_ItemMenuTexture);

        arial.loadFromFile("arial.ttf");

        c_ItemMenuTexts[0] = sf::Text(sf::String("Utilisez"), arial, 30);
        c_ItemMenuTexts[1] = sf::Text(sf::String("Equipper"), arial, 30);
        c_ItemMenuTexts[2] = sf::Text(sf::String("Raccourci"), arial, 30);
        c_ItemMenuTexts[3] = sf::Text(sf::String("Détruire"), arial, 30);
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
                    if(c_LootBagTexts[c_LootBagSelected].getGlobalBounds().contains(mouse.getPosition(app).x, mouse.getPosition(app).y))
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
                    if(c_ItemMenuTexts[c_ItemMenuSelected].getGlobalBounds().contains(mouse.getPosition(app).x, mouse.getPosition(app).y))
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
                        }
                        if(c_ItemMenuSelected == 1)
                        {
                            c_ShowingInventory = false;
                            c_ShowingItemMenu = false;
                            c_ItemToEquip = c_InventorySelected;
                        }
                        if(c_ItemMenuSelected == 2)
                        {
                            c_ShowingShortCutChoice = true;
                        }
                        if(c_ItemMenuSelected == 3)
                        {
                            c_ItemToDestroy = c_InventorySelected;
                            c_ShowingItemMenu = false;
                        }
                    }
                }
                else if(c_ShowingInventory) // choix d'un item dans l'inventaire
                    if(c_InventoryNbrItems > 0 && c_InventoryTexts[c_InventorySelected].getGlobalBounds().contains(mouse.getPosition(app).x, mouse.getPosition(app).y))
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
        for(uint16_t i = 0; i < c_LootBagTexts.size(); ++i)
        {
            if(c_LootBagTexts[i].getGlobalBounds().contains(mouse.getPosition(app).x, mouse.getPosition(app).y))
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
        for(uint16_t i = 0; i < 4; ++i)
        {
            if(c_ItemMenuTexts[i].getGlobalBounds().contains(mouse.getPosition(app).x, mouse.getPosition(app).y))
                c_ItemMenuSelected = i;
        }
    }
    else if(c_ShowingInventory)
    {
        for(uint16_t i = 0; i < c_InventoryTexts.size(); ++i)
        {
            if(c_InventoryTexts[i].getGlobalBounds().contains(mouse.getPosition(app).x, mouse.getPosition(app).y))
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

    c_LootBagSprite.setPosition(app.getSize().x / 2 - c_LootBagSprite.getLocalBounds().width / 2,
                                    app.getSize().y / 2 - c_LootBagSprite.getLocalBounds().height / 2);
    app.draw(c_LootBagSprite);

    uint16_t lastTextHeightSize = 0;
    uint16_t lastTextHeightPosition = 0;
    c_LootBagNbrItems = c_LootBag->getNbrItems();

    c_LootBagTexts.clear();
    for(uint16_t i = c_LootBagFirst; i < c_LootBagFirst + c_LootBagNbrShown; ++i)
    {
        if(lastTextHeightPosition + lastTextHeightSize + 10 > 400 || i >= c_LootBag->getNbrItems())
        {
            c_LootBagNbrShown = i; // on s'arrête avnt d'écrire n'importe ou
            break;
        }

        c_LootBagTexts.push_back(sf::Text(sf::String(c_LootBag->getItem(i)->getName()), arial, 30));
        c_LootBagTexts[i].setPosition(c_LootBagSprite.getPosition().x + 10,
                                    c_LootBagSprite.getPosition().y + lastTextHeightPosition + lastTextHeightSize + (10 * ((i - c_LootBagFirst) != 0)) );
        if(i == c_LootBagSelected)
            c_LootBagTexts[i].setColor(sf::Color::Red);
        else
            c_LootBagTexts[i].setColor(sf::Color::Black);

        sf::Text nbrT(nbrToString(c_LootBag->getNbrOfItem(i)), arial, 30);
        nbrT.setPosition(c_LootBagSprite.getPosition().x + c_LootBagSprite.getLocalBounds().width - 35,
                         c_LootBagSprite.getPosition().y + lastTextHeightPosition + lastTextHeightSize + (10 * ((i - c_LootBagFirst) != 0)) );
        nbrT.setColor(sf::Color::Black);

        app.draw(c_LootBagTexts.back());
        app.draw(nbrT);

        lastTextHeightPosition = c_LootBagTexts[i].getPosition().y - c_LootBagSprite.getPosition().y;
        lastTextHeightSize = c_LootBagTexts[i].getGlobalBounds().height;
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

    uint16_t lastTextHeightSize = 0;
    uint16_t lastTextHeightPosition = 0;

    int16_t basePos = c_InventoryTexts[c_InventorySelected].getPosition().y + 25;
    // place le menu au niveau de l'affichage de l'item selectionné

    c_ItemMenuSprite.setPosition(442, basePos);
    app.draw(c_ItemMenuSprite);

    for(uint16_t i = 0; i < 4; ++i)
    {
        if(!p->getItem(c_InventorySelected)->isUsable() && (i == 0 || i == 1))
            continue; // si l'item n'st pas utilisable on affiche que le choix de destruction d el'item
        if(p->getItem(c_InventorySelected)->getItemType() == EQUIPMENT && (i == 0))
            continue; // si un equipement on n'affiche pas le choix utiliser
        if(!p->getItem(c_InventorySelected)->getItemType() == EQUIPMENT && (i == 1))
            continue; // si pas un equipement on n'affiche pas le choix equipper

        c_ItemMenuTexts[i].setPosition(450, basePos + lastTextHeightPosition + lastTextHeightSize + (10 * (i != 0)) );
        if(i == c_ItemMenuSelected)
            c_ItemMenuTexts[i].setColor(sf::Color::Red);
        else
            c_ItemMenuTexts[i].setColor(sf::Color::Black);

        app.draw(c_ItemMenuTexts[i]);

        lastTextHeightPosition = c_ItemMenuTexts[i].getPosition().y - basePos;
        lastTextHeightSize = c_ItemMenuTexts[i].getGlobalBounds().height;
        // permet de positionner le porchain affichage d'item dans le menu
    }

    app.setView(lastView);
}

void Menu::inventoryMenu(const Player * p)
{
    sf::View lastView = app.getView();
    app.setView(sf::View(sf::FloatRect(0, 0, app.getSize().x, app.getSize().y)));

    c_InventorySprite.setScale(app.getSize().x / c_InventorySprite.getLocalBounds().width,
                               app.getSize().y / c_InventorySprite.getLocalBounds().height);
    app.draw(c_InventorySprite);

    const Bag * bag = p->getBag();
    uint16_t lastTextHeightSize = 0;
    uint16_t lastTextHeightPosition = 0;
    c_InventoryNbrItems = bag->getNbrItems();
    c_InventoryNbrShown = c_InventoryNbrItems - c_InventoryFirst;
    c_InventoryTexts.clear();

    for(uint16_t i = c_InventoryFirst; i < c_InventoryFirst + c_InventoryNbrItems; ++i)
    { // affichage de tous les items du bag
        if(lastTextHeightPosition + lastTextHeightSize + 10 > 560)
        {
            c_InventoryNbrShown = i; // on s'arrête avant d'écrire n'importe ou
            break;
        }

        c_InventoryTexts.push_back(sf::Text(sf::String(bag->getItem(i)->getName()), arial, 30));
        c_InventoryTexts.back().setPosition(560, lastTextHeightPosition + lastTextHeightSize + (10 * ((i - c_InventoryFirst) != 0)) );
        if(i == c_InventorySelected)
            c_InventoryTexts.back().setColor(sf::Color::Red);
        else
            c_InventoryTexts.back().setColor(sf::Color::Black);

        sf::Text nbrT(nbrToString(bag->getNbrOfItem(i)), arial, 30);
        nbrT.setPosition(770, lastTextHeightPosition + lastTextHeightSize + (10 * ((i - c_InventoryFirst) != 0)) );
        nbrT.setColor(sf::Color::Black);

        app.draw(c_InventoryTexts.back());
        app.draw(nbrT);

        lastTextHeightPosition = c_InventoryTexts.back().getPosition().y;
        lastTextHeightSize = c_InventoryTexts.back().getGlobalBounds().height;
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

#include "ChoiceText.hpp"

ChoiceText::ChoiceText() : Text()
{
    c_ChoiceBackGround.setOutlineThickness(5);
    c_ChoiceBackGround.setFillColor(sf::Color(200, 200, 200));
    c_ChoiceBackGround.setOutlineColor(sf::Color::Black);
    c_CharSize = 25;

    c_Font.loadFromFile("arial.ttf");

    c_SelectedChoice = 0;
    c_FirstChoicesShown = 0;
    c_ShowChoices = false;
    c_IsCancelable = false;
}

void ChoiceText::newText(const std::string & question, std::vector< std::string > & choices, const sf::Texture & texture
                         , sf::RenderWindow & app, bool isCancelable)
{
    c_ActualDisplay.setTexture(texture);
    c_ActualDisplay.setPosition(0, 0);

    c_CutText = cutText(question, c_Font, c_CharSize, app.getSize().x);
    c_Choices = choices;
    c_CurrentLine = 0;
    c_NbrLine = c_CutText.size();
    c_IsCancelable = isCancelable;
    c_SelectedChoice = 0;
    c_NumberChoicesShown = 4;
    c_FirstChoicesShown = 0;
    c_ShowChoices = false;

    c_Drawing = true;
    while(c_Drawing)
    {
        eventManage(app);
        draw(app);
    }
}

void ChoiceText::eventManage(sf::RenderWindow & app)
{
    sf::Event event;
    while (app.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::KeyPressed:
            {
                switch(event.key.code)
                {
                case sf::Event::Closed:
                    c_Drawing = false;
                    app.close();
                break;

                case sf::Keyboard::Escape:
                    if(c_IsCancelable)
                    {
                        c_Drawing = false;
                        c_SelectedChoice = -1;
                    }
                    break;
                case sf::Keyboard::Return:
                    if(!c_ShowChoices)
                        nextLine();
                    else
                        c_Drawing = false;
                    break;


                case sf::Keyboard::Z:
                    c_SelectedChoice--;
                    if(c_SelectedChoice < 0)
                    {
                        c_SelectedChoice = c_Choices.size()-1;
                        c_FirstChoicesShown = c_SelectedChoice-c_NumberChoicesShown+1;
                        if(c_FirstChoicesShown < 0)
                            c_FirstChoicesShown = 0;
                    }
                    else if(c_SelectedChoice < c_FirstChoicesShown)
                    {
                        c_FirstChoicesShown--;
                    }
                    break;
                case sf::Keyboard::S:
                    c_SelectedChoice++;
                    if(c_SelectedChoice == c_Choices.size())
                    {
                        c_SelectedChoice = 0;
                        c_FirstChoicesShown = 0;
                    }
                    else if(c_SelectedChoice >= c_FirstChoicesShown + c_NumberChoicesShown)
                    {
                        c_FirstChoicesShown++;
                    }
                    break;

                default:
                    break;
                }
            }break;

            case sf::Event::MouseButtonReleased:
            {
                switch(event.mouseButton.button)
                {
                    case sf::Mouse::Left:
                        if(!c_ShowChoices)
                            nextLine();
                        else
                            c_Drawing = false;
                        break;

                    default:
                        break;
                }
                break;
            }break;

            default:
                break;
        }
    }

    sf::Mouse mouse;

    for(uint16_t i = 0; i < c_ChoicesTexts.size(); ++i)
    {
        if(c_ChoicesTexts[i].getGlobalBounds().contains(mouse.getPosition(app).x, mouse.getPosition(app).y))
        {
            c_SelectedChoice = i;
        }
    }
}

void ChoiceText::draw(sf::RenderWindow & app)
{
    if(c_Drawing)
    {
        c_NbrCaraLine = app.getSize().x / 14;
        c_NbrLineDraw = app.getSize().y / 150;

        c_BackGround.setSize(sf::Vector2f(app.getSize().x - c_BackGround.getOutlineThickness()*2,
                                        app.getSize().y/4 - c_BackGround.getOutlineThickness()*2));
        c_BackGround.setPosition(sf::Vector2f(0 + c_BackGround.getOutlineThickness(),
                                            app.getSize().y * 3/4 + c_BackGround.getOutlineThickness()));


        sf::Text t(sf::String("azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN"), c_Font, c_CharSize);

        uint16_t charHeight = t.getGlobalBounds().height; // taille max d'un caract
        uint16_t height = c_Choices.size() * (charHeight + PXLBTWTEXT) - c_ChoiceBackGround.getOutlineThickness()*2;
        if(height > (app.getSize().y * 3/4 - c_ChoiceBackGround.getOutlineThickness()*3))
            height = app.getSize().y * 3/4 - c_ChoiceBackGround.getOutlineThickness()*3;

        c_NumberChoicesShown = (height + c_ChoiceBackGround.getOutlineThickness()*2) / (charHeight + PXLBTWTEXT);

        c_ChoiceBackGround.setSize(sf::Vector2f(app.getSize().x/4 + c_ChoiceBackGround.getOutlineThickness()*2,
                                                height + c_ChoiceBackGround.getOutlineThickness()*2));
        c_ChoiceBackGround.setPosition(sf::Vector2f(app.getSize().x - c_ChoiceBackGround.getSize().x,
                                            app.getSize().y * 3/4 - c_ChoiceBackGround.getSize().y));

        if(c_NextLine) // on passe a la ligne suivante
        {
            c_CurrentLine++;
            c_NextLine = false;
        }

        if(c_CurrentLine > c_NbrLine - c_NbrLineDraw)
        {
            c_ShowChoices = true; // si toutes les lignes ont été affichées, on affiche les réponses
        }

        app.clear();

        sf::View lastView = app.getView();
        // changement de vue pour garder le texte au même endroit
        // même avec changement de position de la vue principale
        app.setView(sf::View(sf::FloatRect(0, 0, app.getSize().x, app.getSize().y)));

        app.draw(c_ActualDisplay);

        app.draw(c_BackGround);
        app.draw(c_ChoiceBackGround);

        for(int16_t i = c_CurrentLine; i < c_CurrentLine + c_NbrLineDraw; i++)
        {
            if(i < c_CutText.size())
            {// lignes restantes vident
                sf::Text text(sf::String(c_CutText[i]), c_Font, c_CharSize);
                text.setPosition(c_BackGround.getPosition().x + c_BackGround.getOutlineThickness()
                                  , c_BackGround.getPosition().y + (i-c_CurrentLine) * c_CharSize);
                text.setColor(sf::Color(100, 100, 100));

                app.draw(text);
            }
        }

        c_ChoicesTexts.clear();
        for(int16_t i = c_FirstChoicesShown; i < c_FirstChoicesShown + c_NumberChoicesShown; i++)
        {
            if(i < c_Choices.size())
            {// lignes restantes vident
                sf::Text text(sf::String(c_Choices[i]), c_Font, c_CharSize);
                text.setPosition(c_ChoiceBackGround.getPosition().x + c_ChoiceBackGround.getOutlineThickness() / 2
                        , c_ChoiceBackGround.getPosition().y + (i-c_FirstChoicesShown) * (charHeight + (i != c_FirstChoicesShown) * PXLBTWTEXT) - text.getLocalBounds().height/4 + (i == c_FirstChoicesShown) * PXLBTWTEXT/2);

                c_ChoicesTexts.push_back(text);

                if(c_SelectedChoice == i)
                    text.setColor(sf::Color(200, 0, 0));
                else
                    text.setColor(sf::Color(100, 100, 100));

                app.draw(text);
            }
        }

        app.setView(lastView);
        app.display();
    }
}


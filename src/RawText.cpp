#include "RawText.hpp"

RawText::RawText() : Text()
{
    c_Font.loadFromFile("arial.ttf");
    c_CharSize = 25;
}

void RawText::newText(const std::string & text, const sf::Texture & texture, sf::RenderWindow & app)
{
    c_ActualDisplay.setTexture(texture);
    c_ActualDisplay.setPosition(0, 0);

    c_CutText = cutText(text, c_Font, c_CharSize, app.getSize().x);
    c_CurrentLine = 0;
    c_NbrLine = c_CutText.size();

    c_Drawing = true;
    while(c_Drawing)
    {
        eventManage(app);
        draw(app);
    }
}
void RawText::eventManage(sf::RenderWindow & app)
{
    sf::Event event;
    while (app.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                c_Drawing = false;
                app.close();
            break;

            case sf::Event::KeyPressed:
            {
                switch(event.key.code)
                {
                    case sf::Keyboard::Return:
                        nextLine();
                        break;

                    default:
                        break;
                }
            }break;

            case sf::Event::MouseButtonPressed:
            {
                switch(event.mouseButton.button)
                {
                    case sf::Mouse::Left:
                        nextLine();
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
}

void RawText::draw(sf::RenderWindow & app)
{
    if(c_Drawing)
    {
        c_NbrCaraLine = app.getSize().x / 14;
        c_NbrLineDraw = app.getSize().y / 150;

        c_BackGround.setSize(sf::Vector2f(app.getSize().x - c_BackGround.getOutlineThickness()*2,
                                        app.getSize().y/4 - c_BackGround.getOutlineThickness()*2));
        c_BackGround.setPosition(sf::Vector2f(0 + c_BackGround.getOutlineThickness(),
                                        app.getSize().y * 3/4 + c_BackGround.getOutlineThickness()));

        if(c_NextLine) // on passe a la ligne suivante
        {
            c_CurrentLine++;
            c_NextLine = false;

            if(c_CurrentLine > c_NbrLine - c_NbrLineDraw)
            {
                c_Drawing = false; // si toutes les lignes ont été affichées, on arrête d'afficher
                return;
            }
        }

        app.clear();

        sf::View lastView = app.getView();
        // changement de vue pour garder le texte au même endroit
        // même avec changement de position de la vue principale
        app.setView(sf::View(sf::FloatRect(0, 0, app.getSize().x, app.getSize().y)));

        app.draw(c_ActualDisplay);

        app.draw(c_BackGround);

        for(int16_t i = c_CurrentLine; i < c_CurrentLine + c_NbrLineDraw; i++)
        {
            if(i < c_CutText.size())
            {// lignes restantes vident
                sf::Text text(sf::String(c_CutText[i]), c_Font, c_CharSize);
                text.setPosition(c_BackGround.getPosition().x + c_BackGround.getOutlineThickness()
                                  , c_BackGround.getPosition().y + (i-c_CurrentLine)*35);
                text.setColor(sf::Color(100, 100, 100));
                app.draw(text);
            }
        }

        app.setView(lastView);
        app.display();
    }
}

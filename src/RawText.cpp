#include "RawText.hpp"

RawText::RawText() : Text()
{

}
RawText::RawText(const std::string & text) : Text()
{
    if(!c_Drawing) // initialis&ation lors d'un nouveau texte
    {
        c_CutText = cutText(text);
        c_CurrentLine = 0;
        c_NbrLine = c_CutText.size();

        c_Drawing = true;
    }
}

void RawText::newText(const std::string & text)
{
    if(!c_Drawing) // initialis&ation lors d'un nouveau texte
    {
        c_CutText = cutText(text);
        c_CurrentLine = 0;
        c_NbrLine = c_CutText.size();

        c_Drawing = true;
    }
}
void RawText::eventManage(sf::Event event)
{
    switch(event.type)
    {
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

        // changement de vue pour garder le texte au même endroit
        // même avec changement de position de la vue principale
        sf::View lastView = app.getView();
        app.setView(sf::View(sf::FloatRect(0, 0, app.getSize().x, app.getSize().y)));
        app.draw(c_BackGround);

        sf::Font font;
        font.loadFromFile("arial.ttf");

        for(int16_t i = c_CurrentLine; i < c_CurrentLine + c_NbrLineDraw; i++)
        {
            if(i < c_CutText.size())
            {// lignes restantes vident
                sf::Text text(sf::String(c_CutText[i]), font, 30);
                text.setPosition(c_BackGround.getPosition().x + c_BackGround.getOutlineThickness()
                                  , c_BackGround.getPosition().y + (i-c_CurrentLine)*35);
                text.setColor(sf::Color(100, 100, 100));
                app.draw(text);
            }
        }

        app.setView(lastView);
    }
}

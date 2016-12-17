#include "Text.hpp"

Text::Text()
{
    c_BackGround.setOutlineThickness(5);
    c_BackGround.setFillColor(sf::Color(200, 200, 200));
    c_BackGround.setOutlineColor(sf::Color::Black);

    c_NextLine = false;
    c_Drawing = false;
    c_CurrentLine = 0;
    c_NbrLine = 0;
}

std::vector< std::string > Text::cutText(const std::string & text, sf::Font & font, uint16_t charSize, uint16_t width)
{
    std::vector< std::string > cutText;

    sf::Text visualText("", font, charSize);
    for(uint16_t i = 0; i < text.length(); i++)
    {
        visualText.setString(visualText.getString() + text[i]);
        if(visualText.getLocalBounds().width > width)
        {
            std::string lineText = visualText.getString();
            if(i != text.length()-1 && text[i+1] != ' ')
                while(lineText.at(lineText.length()-1) != ' ')
                {
                    lineText.erase(lineText.end()-1);
                    i--;
                }
            cutText.push_back(lineText);
            visualText.setString("");
        }
    }

    if(!visualText.getString().isEmpty())
        cutText.push_back(visualText.getString());

    return cutText;
}

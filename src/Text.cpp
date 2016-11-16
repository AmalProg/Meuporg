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

std::vector< std::string > Text::cutText(const std::string & text)
{
    std::string tex = text; // texte à découper
    std::vector< std::string > cutText;
    while(tex.length() != 0) // jusqu'au parcours de tout le texte
    {
        cutText.push_back(tex.substr(0, c_NbrCaraLine)); // ajoute la ligne

        int8_t i = cutText.size()-1; // taille actuelle de cutText
        if(tex.length() > c_NbrCaraLine)
            if(cutText[i][cutText[i].length()-1] != ' ' && tex[c_NbrCaraLine] != ' ')
                cutText[i].append("-"); // ajoute '-' si le mot n'est pas finit
        if(cutText[i][0] == ' ')
            cutText[i].erase(0, 1); // la ligne ne commence pas par ' '


        tex.erase(0, c_NbrCaraLine); // supprime la ligne lu
    }

    return cutText;
}

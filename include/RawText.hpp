#ifndef RAWTEXT_HPP
#define RAWTEXT_HPP

#include "Text.hpp"

class RawText : public Text
{
    public:
        RawText();

        void newText(const std::string & text, const sf::Texture & texture, sf::RenderWindow & app);

    private:
        void draw(sf::RenderWindow & app);
        void eventManage(sf::RenderWindow & app);

    private:
        sf::Sprite c_ActualDisplay; //affichage présent avant d'afficher le texte
};

#endif // RAWTEXT_HPP

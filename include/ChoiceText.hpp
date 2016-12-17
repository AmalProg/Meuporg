#ifndef CHOICETEXT_HPP
#define CHOICETEXT_HPP

#define PXLBTWTEXT 4

#include "Text.hpp"

class ChoiceText : public Text
{
    public:
        ChoiceText();

        void newText(const std::string & question, std::vector< std::string > & choices, const sf::Texture & texture
                     , sf::RenderWindow & app, bool isCancelable = false);
        void draw(sf::RenderWindow & a);
        void eventManage(sf::RenderWindow & app);

        uint32_t getChoice() { return c_SelectedChoice;}

    private:
        std::vector< std::string > c_Choices;
        std::vector< sf::Text > c_ChoicesTexts;
        int32_t c_SelectedChoice;
        uint32_t c_NumberChoicesShown;
        int32_t c_FirstChoicesShown;
        bool c_ShowChoices;
        bool c_IsCancelable;

        sf::RectangleShape c_ChoiceBackGround;

        sf::Sprite c_ActualDisplay; //affichage présent avant d'afficher le texte
};

#endif // CHOICETEXT_HPP

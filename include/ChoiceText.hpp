#ifndef CHOICETEXT_HPP
#define CHOICETEXT_HPP

#define PXLBTWTEXT 4

#include "Text.hpp"

class ChoiceText : public Text
{
    public:
        ChoiceText();
        ChoiceText(const std::string & question, std::vector< std::string > & choices);

        void newText(const std::string & question, std::vector< std::string > & choices);
        void draw(sf::RenderWindow & a);
        void eventManage(sf::Event event, sf::RenderWindow & a);

        void setCancelable(bool c) { c_IsCancelable = c; }
        bool isCancelable() const { return c_IsCancelable; }

        bool isChoiced() const { return c_Choiced; }
        uint32_t getChoice() { return c_SelectedChoice; c_Choiced = false;}

    private:
        std::vector< std::string > c_Choices;
        std::vector< sf::Text > c_ChoicesTexts;
        int32_t c_SelectedChoice;
        uint32_t c_NumberChoicesShown;
        int32_t c_FirstChoicesShown;
        bool c_ShowChoices;
        bool c_Choiced;
        bool c_IsCancelable;

        uint16_t c_CharSize;
        sf::RectangleShape c_ChoiceBackGround;
};

#endif // CHOICETEXT_HPP

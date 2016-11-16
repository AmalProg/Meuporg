#ifndef TEXT_HPP
#define TEXT_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

class Text
{
    public:
        Text();

        virtual void draw(sf::RenderWindow & a) = 0;
        //virtual void eventManage(sf::Event event) = 0;

        void nextLine() { c_NextLine = true; }
        bool isDrawing() const { return c_Drawing; }

    protected:

        std::vector< std::string > cutText(const std::string & text);

    protected:
        uint16_t c_NbrLineDraw; // nombre de line possible d'afficher en même temps
        uint16_t c_NbrCaraLine;
        bool c_Drawing; // si on affiche du texte en ce moment
        bool c_NextLine; // demande de passage a une nouvelle ligne

        std::vector< std::string > c_CutText; // texte découpé en lignes
        uint16_t c_CurrentLine; // 1ere ligne actuelle affichée
        uint16_t c_NbrLine; // nombre de ligne du texte

        sf::RectangleShape c_BackGround; // boîte ou le texte sera écrit
};

#endif // TEXT_HPP

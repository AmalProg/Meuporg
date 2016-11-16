#ifndef RAWTEXT_HPP
#define RAWTEXT_HPP

#include "Text.hpp"


class RawText : public Text
{
    public:
        RawText();
        RawText(const std::string & text);

        void newText(const std::string & text);
        void draw(sf::RenderWindow & a);
        void eventManage(sf::Event event);

    private:
};

#endif // RAWTEXT_HPP

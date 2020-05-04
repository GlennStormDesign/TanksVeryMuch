#pragma once

#include "SFML/Graphics.hpp"

// Font Declarations

extern void FontInit();
extern bool FontsAvailable();
extern sf::Font& TitleFont();
extern sf::Font& HeadingFont();
extern sf::Font& TextFont();
extern sf::Font& DetailFont();

class FontManager {
public:
private:
    sf::Font m_titleFont, m_headingFont, m_textFont, m_detailFont;
    bool m_fontsAvailable = false;
public:
    FontManager() { FontInit(); }
    ~FontManager() { }

    void FontInit();
    bool FontsAvailable();
    sf::Font& GetTitleFont();
    sf::Font& GetHeadingFont();
    sf::Font& GetTextFont();
    sf::Font& GetDetailFont();
private:
};

enum HeadingAlignment {
    Center,
    Left,
    Right
};

class FontHeading {
public:
private:
    sf::Font m_font;
    unsigned short int m_fontSize;
    sf::Color m_fontColor;
    HeadingAlignment m_fontAlign;
public:
    FontHeading();
    ~FontHeading();
protected:
    virtual void InitHeadingText( sf::Text& t ); // initialize text object from this heading
    virtual HeadingAlignment GetAlignment();
};

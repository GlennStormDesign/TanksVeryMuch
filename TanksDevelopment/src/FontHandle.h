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
protected:
    sf::Font m_font;
    int m_fontSize;
    sf::Color m_fontColor;
    HeadingAlignment m_fontAlign;
    int m_lineSpace;
public:
    FontHeading() { HeadingInit(); };
    ~FontHeading() { };

    void HeadingInit()
    {
        // general init
        SubInit();
        // specific init
    }
protected:
    virtual void SubInit();
    void InitHeadingText( sf::Text& t ); // initialize text object from this heading
    HeadingAlignment GetAlignment();
    int GetLineSpace();
};


class DebugHeading : public FontHeading {
    DebugHeading() { HeadingInit(); }
    void SubInit() override
    {
        m_font = DetailFont();
        m_fontColor = sf::Color::Yellow;
        m_fontSize = 12;
    }
};

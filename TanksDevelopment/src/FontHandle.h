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
        //m_lineSpace = ?;
    }
};

class ClearSmallHeading : public FontHeading {
    ClearSmallHeading() { HeadingInit(); }
    void SubInit() override
    {
        m_font = DetailFont();
        m_fontColor = sf::Color::Black;
        m_fontSize = 24;
        //m_lineSpace = ?;
    }
};

class ClearLargeHeading : public FontHeading {
    ClearLargeHeading() { HeadingInit(); }
    void SubInit() override
    {
        m_font = DetailFont();
        m_fontColor = sf::Color::Black;
        m_fontSize = 36;
        //m_lineSpace = ?;
    }
};

class HUDLabelSemi : public FontHeading {
    HUDLabelSemi() { HeadingInit(); }
    void SubInit() override
    {
        m_font = TextFont();
        m_fontColor = sf::Color(255,255,255,128);
        m_fontSize = 32;
        //m_lineSpace = ?;
    }
};

class HUDLabelWhite : public FontHeading {
    HUDLabelWhite() { HeadingInit(); }
    void SubInit() override
    {
        m_font = TextFont();
        m_fontColor = sf::Color::White;
        m_fontSize = 32;
        //m_lineSpace = ?;
    }
};

class PanelLabel : public FontHeading {
    PanelLabel() { HeadingInit(); }
    void SubInit() override
    {
        m_font = TextFont();
        m_fontColor = sf::Color::Black;
        m_fontSize = 24;
        //m_lineSpace = ?;
    }
};

class PanelDescription : public FontHeading {
    PanelDescription() { HeadingInit(); }
    void SubInit() override
    {
        m_font = TextFont();
        m_fontColor = sf::Color::Black;
        m_fontSize = 18;
        //m_lineSpace = ?;
    }
};

class SubTitle : FontHeading {
    SubTitle() { HeadingInit(); }
    void SubInit() override
    {
        m_font = HeadingFont();
        m_fontColor = sf::Color::Black;
        m_fontSize = 48;
        //m_lineSpace = ?;
    }
};

class PanelTitle : FontHeading {
    PanelTitle() { HeadingInit(); }
    void SubInit() override
    {
        m_font = HeadingFont();
        m_fontColor = sf::Color::Black;
        m_fontSize = 36;
        //m_lineSpace = ?;
    }
};

class PanelHeading : FontHeading {
    PanelHeading() { HeadingInit(); }
    void SubInit() override
    {
        m_font = HeadingFont();
        m_fontColor = sf::Color::Black;
        m_fontSize = 24;
        //m_lineSpace = ?;
    }
};

class MainTitle : FontHeading {
    MainTitle() { HeadingInit(); }
    void SubInit() override
    {
        m_font = TitleFont();
        m_fontColor = sf::Color::Black;
        m_fontSize = 49;
        //m_lineSpace = ?;
    }
};

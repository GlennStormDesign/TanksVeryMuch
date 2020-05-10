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
    sf::Color m_shadowColor; // if drop shadow used
    HeadingAlignment m_fontAlign;
    float m_charSpace; // pct of char size (horizontal centering adjust)
    float m_lineSpace; // pct of char size (vertical centering adjust)
public:
    FontHeading() { HeadingInit(); }
    ~FontHeading() { }

    void HeadingInit()
    {
        // general init
        SubInit();
        // specific init
    }

    void InitHeadingText( sf::Text& t ); // initialize text object from this heading
    HeadingAlignment GetAlignment();
    sf::Vector2f GetFontSpace(); // horizontal and vertical offset to center heading
    sf::Color& GetShadowColor();
protected:
    virtual void SubInit() { }
};


class ToolipHeading : public FontHeading {
public:
    ToolipHeading() { HeadingInit(); }
    void SubInit() override
    {
        m_font = DetailFont();
        m_fontAlign = Left;
        m_fontColor = sf::Color::Yellow;
        m_shadowColor = sf::Color::Black;
        m_fontSize = 12;
        m_charSpace = 0.12f;
        m_lineSpace = 0.318f;
    }
};

class ClearSmallHeading : public FontHeading {
public:
    ClearSmallHeading() { HeadingInit(); }
    void SubInit() override
    {
        m_font = DetailFont();
        m_fontAlign = Left;
        m_fontColor = sf::Color::Black;
        m_shadowColor = sf::Color(128,128,128,255);
        m_fontSize = 20;
        m_charSpace = 0.12f;
        m_lineSpace = 0.318f;
    }
};

class ClearLargeHeading : public FontHeading {
public:
    ClearLargeHeading() { HeadingInit(); }
    void SubInit() override
    {
        m_font = DetailFont();
        m_fontAlign = Center;
        m_fontColor = sf::Color::Black;
        m_shadowColor = sf::Color(128,128,128,255);
        m_fontSize = 36;
        m_charSpace = 0.12f;
        m_lineSpace = 0.318f;
    }
};

class HUDLabelSemi : public FontHeading {
public:
    HUDLabelSemi() { HeadingInit(); }
    void SubInit() override
    {
        m_font = TextFont();
        m_fontAlign = Center;
        m_fontColor = sf::Color(255,255,255,128);
        m_shadowColor = sf::Color(0,0,0,128);
        m_fontSize = 32;
        m_charSpace = 0.04f;
        m_lineSpace = 0.115f;
    }
};

class HUDLabelWhite : public FontHeading {
public:
    HUDLabelWhite() { HeadingInit(); }
    void SubInit() override
    {
        m_font = TextFont();
        m_fontAlign = Center;
        m_fontColor = sf::Color::White;
        m_shadowColor = sf::Color::Black;
        m_fontSize = 32;
        m_charSpace = 0.04f;
        m_lineSpace = 0.115f;
    }
};

class PanelLabel : public FontHeading {
public:
    PanelLabel() { HeadingInit(); }
    void SubInit() override
    {
        m_font = TextFont();
        m_fontAlign = Center;
        m_fontColor = sf::Color::Black;
        m_shadowColor = sf::Color(128,128,128,255);
        m_fontSize = 28;
        m_charSpace = 0.04f;
        m_lineSpace = 0.115f;
    }
};

class PanelDescription : public FontHeading {
public:
    PanelDescription() { HeadingInit(); }
    void SubInit() override
    {
        m_font = TextFont();
        m_fontAlign = Left;
        m_fontColor = sf::Color::Black;
        m_shadowColor = sf::Color(128,128,128,255);
        m_fontSize = 18;
        m_charSpace = 0.04f;
        m_lineSpace = 0.115f;
    }
};

class SubTitle : public FontHeading {
public:
    SubTitle() { HeadingInit(); }
    void SubInit() override
    {
        m_font = HeadingFont();
        m_fontAlign = Center;
        m_fontColor = sf::Color::Black;
        m_shadowColor = sf::Color(128,128,128,255);
        m_fontSize = 48;
        m_charSpace = 0.05f;
        m_lineSpace = 0.19f;
    }
};

class PanelTitle : public FontHeading {
public:
    PanelTitle() { HeadingInit(); }
    void SubInit() override
    {
        m_font = HeadingFont();
        m_fontAlign = Center;
        m_fontColor = sf::Color::Black;
        m_shadowColor = sf::Color(128,128,128,255);
        m_fontSize = 36;
        m_charSpace = 0.05f;
        m_lineSpace = 0.19f;
    }
};

class PanelHeading : public FontHeading {
public:
    PanelHeading() { HeadingInit(); }
    void SubInit() override
    {
        m_font = HeadingFont();
        m_fontAlign = Left;
        m_fontColor = sf::Color::Black;
        m_shadowColor = sf::Color(128,128,128,255);
        m_fontSize = 24;
        m_charSpace = 0.05f;
        m_lineSpace = 0.19f;
    }
};

class MainTitle : public FontHeading {
public:
    MainTitle() { HeadingInit(); }
    void SubInit() override
    {
        m_font = TitleFont();
        m_fontAlign = Center;
        m_fontColor = sf::Color::Black;
        m_shadowColor = sf::Color(128,128,128,255);
        m_fontSize = 80;
        m_charSpace = 0.f;
        m_lineSpace = 0.15f;
    }
};

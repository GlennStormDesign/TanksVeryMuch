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
    float m_charSpace; // pct of char size (horizontal centering adjust)
    float m_lineSpace; // pct of char size (vertical centering adjust)
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


class ToolipHeading : public FontHeading {
    ToolipHeading() { HeadingInit(); }
    void SubInit() override
    {
        m_font = DetailFont();
        m_fontColor = sf::Color::Yellow;
        m_fontSize = 12;
        m_charSpace = 0.06f;
        m_lineSpace = 0.15f;
    }
};

class ClearSmallHeading : public FontHeading {
    ClearSmallHeading() { HeadingInit(); }
    void SubInit() override
    {
        m_font = DetailFont();
        m_fontColor = sf::Color::Black;
        m_fontSize = 24;
        m_charSpace = 0.06f;
        m_lineSpace = 0.15f;
    }
};

class ClearLargeHeading : public FontHeading {
    ClearLargeHeading() { HeadingInit(); }
    void SubInit() override
    {
        m_font = DetailFont();
        m_fontColor = sf::Color::Black;
        m_fontSize = 36;
        m_charSpace = 0.06f;
        m_lineSpace = 0.15f;
    }
};

class HUDLabelSemi : public FontHeading {
    HUDLabelSemi() { HeadingInit(); }
    void SubInit() override
    {
        m_font = TextFont();
        m_fontColor = sf::Color(255,255,255,128);
        m_fontSize = 32;
        m_charSpace = 0.04f;
        m_lineSpace = 0.115f;
    }
};

class HUDLabelWhite : public FontHeading {
    HUDLabelWhite() { HeadingInit(); }
    void SubInit() override
    {
        m_font = TextFont();
        m_fontColor = sf::Color::White;
        m_fontSize = 32;
        m_charSpace = 0.04f;
        m_lineSpace = 0.115f;
    }
};

class PanelLabel : public FontHeading {
    PanelLabel() { HeadingInit(); }
    void SubInit() override
    {
        m_font = TextFont();
        m_fontColor = sf::Color::Black;
        m_fontSize = 24;
        m_charSpace = 0.04f;
        m_lineSpace = 0.115f;
    }
};

class PanelDescription : public FontHeading {
    PanelDescription() { HeadingInit(); }
    void SubInit() override
    {
        m_font = TextFont();
        m_fontColor = sf::Color::Black;
        m_fontSize = 18;
        m_charSpace = 0.04f;
        m_lineSpace = 0.115f;
    }
};

class SubTitle : FontHeading {
    SubTitle() { HeadingInit(); }
    void SubInit() override
    {
        m_font = HeadingFont();
        m_fontColor = sf::Color::Black;
        m_fontSize = 48;
        m_charSpace = 0.05f;
        m_lineSpace = 0.19f;
    }
};

class PanelTitle : FontHeading {
    PanelTitle() { HeadingInit(); }
    void SubInit() override
    {
        m_font = HeadingFont();
        m_fontColor = sf::Color::Black;
        m_fontSize = 36;
        m_charSpace = 0.05f;
        m_lineSpace = 0.19f;
    }
};

class PanelHeading : FontHeading {
    PanelHeading() { HeadingInit(); }
    void SubInit() override
    {
        m_font = HeadingFont();
        m_fontColor = sf::Color::Black;
        m_fontSize = 24;
        m_charSpace = 0.05f;
        m_lineSpace = 0.19f;
    }
};

class MainTitle : FontHeading {
    MainTitle() { HeadingInit(); }
    void SubInit() override
    {
        m_font = TitleFont();
        m_fontColor = sf::Color::Black;
        m_fontSize = 49;
        m_charSpace = 0.f;
        m_lineSpace = 0.15f;
    }
};

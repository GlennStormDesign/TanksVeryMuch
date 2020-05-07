#include "FontHandle.h"

// Font Definitions

static FontManager fontMgr;

// FontManager interface

extern void FontInit()
{
    return fontMgr.FontInit();
}
extern bool FontsAvailable()
{
    return fontMgr.FontsAvailable();
}
extern sf::Font& TitleFont()
{
    return fontMgr.GetTitleFont();
}
extern sf::Font& HeadingFont()
{
    return fontMgr.GetHeadingFont();
}
extern sf::Font& TextFont()
{
    return fontMgr.GetTextFont();
}
extern sf::Font& DetailFont()
{
    return fontMgr.GetDetailFont();
}

// FontManager implementation

void FontManager::FontInit() {
    m_fontsAvailable = true;
    if ( !m_titleFont.loadFromFile("image/fontTitle.ttf") ||
            !m_headingFont.loadFromFile("image/fontHeading.ttf") ||
            !m_textFont.loadFromFile("image/fontText.ttf") ||
            !m_detailFont.loadFromFile("image/fontDetail.ttf") )
    {
        // error
        m_fontsAvailable = false;
    }
}
bool FontManager::FontsAvailable()
{
    return m_fontsAvailable;
}
sf::Font& FontManager::GetTitleFont()
{
    return m_titleFont;
}
sf::Font& FontManager::GetHeadingFont()
{
    return m_headingFont;
}
sf::Font& FontManager::GetTextFont()
{
    return m_textFont;
}
sf::Font& FontManager::GetDetailFont()
{
    return m_detailFont;
}

void FontHeading::InitHeadingText( sf::Text& textObj )
{
    textObj.setFont( m_font );
    textObj.setFillColor( m_fontColor );
    textObj.setCharacterSize( m_fontSize );
}

HeadingAlignment FontHeading::GetAlignment()
{
    return m_fontAlign;
}

sf::Vector2f FontHeading::GetFontSpace()
{
    return sf::Vector2f( (m_charSpace*m_fontSize), (m_lineSpace*m_fontSize) );
}

sf::Color& FontHeading::GetShadowColor()
{
    return m_shadowColor;
}

// Font Heading subclasses defined in header

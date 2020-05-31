#include <sstream>

#include "TankCore.h"

// Core Definitions

static DebugLogger debugMgr;

// DebugLogger interface

extern void DebugInit( const sf::Font& font )
{
    debugMgr.DebugInit(font);
}
extern const std::string& GetDebugText()
{
    return debugMgr.GetDebugText();
}
extern void SetDebugText( const std::string& text )
{
    debugMgr.SetDebugText( text );
}
extern void AddDebugText( const std::string& text )
{
    debugMgr.AddDebugText( text );
}
extern std::string FormatDebugHeader()
{
    return debugMgr.FormatDebugHeader();
}
extern std::string FormatInt( const int& i )
{
    return debugMgr.FormatInt(i);
}
extern std::string FormatFloat( const float& f )
{
    return debugMgr.FormatFloat(f);
}
extern std::string FormatVector2i( const sf::Vector2i& v )
{
    return debugMgr.FormatVector2i(v);
}
extern std::string FormatVector2f( const sf::Vector2f& v )
{
    return debugMgr.FormatVector2f(v);
}
extern void DrawDebug( sf::RenderWindow& window )
{
    debugMgr.DrawDebug(window);
}

// DebugLogger implementation

void DebugLogger::DebugInit( const sf::Font& font )
{
    m_debugLine.setFont(font);
    m_debugLine.setCharacterSize(12);
    m_debugLine.setFillColor(sf::Color::Yellow);
}
const std::string& DebugLogger::GetDebugText()
{
    return debugMgr.m_text;
}
void DebugLogger::SetDebugText( const std::string& text )
{
    debugMgr.m_text = text;
}
void DebugLogger::AddDebugText( const std::string& text )
{
    debugMgr.m_text += text;
}
std::string DebugLogger::FormatDebugHeader()
{
    std::string retString = "[DEBUG] ";
    retString += TANKSVERYMUCH_VERSION;
    retString += "\n\n";
    return retString;
}
std::string DebugLogger::FormatInt( const int& i )
{
    std::ostringstream ss;
    ss << i;
    return std::string( ss.str() );
}
std::string DebugLogger::FormatFloat( const float& f )
{
    std::ostringstream ss;
    ss << f;
    return std::string( ss.str() );
}
std::string DebugLogger::FormatVector2i( const sf::Vector2i& v )
{
    std::string retString;
    retString = "( ";
    retString += DebugLogger::FormatInt( v.x );
    retString += " , ";
    retString += DebugLogger::FormatInt( v.y );
    retString += " )";
    return retString;
}
std::string DebugLogger::FormatVector2f( const sf::Vector2f& v )
{
    std::string retString;
    retString = "( ";
    retString += DebugLogger::FormatFloat( v.x );
    retString += " , ";
    retString += DebugLogger::FormatFloat( v.y );
    retString += " )";
    return retString;
}
void DebugLogger::DrawDebug( sf::RenderWindow& window )
{
    m_debugLine.setString( GetDebugText() );
    m_debugLine.setPosition( window.getView().getCenter() + m_debugLineOffset );
    if ( m_debugLine.getString() != "" )
        window.draw( m_debugLine );
}

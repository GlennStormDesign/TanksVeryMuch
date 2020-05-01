#pragma once

#include <string>

#include <SFML/Graphics.hpp>

// Core Declarations

static const std::string TANKSVERYMUCH_VERSION = "v.0.18.05.01.20";
static const float DEG2RAD = 0.0174532f;

static float globalTime = 1.f;
static float globalScale = 2.f;

extern std::string GetDebugText();
extern void SetDebugText( const std::string& text );
extern void AddDebugText( const std::string& text );
extern std::string FormatDebugHeader();
extern std::string FormatInt( const int& i );
extern std::string FormatFloat( const float& f );
extern std::string FormatVector2i( const sf::Vector2i& v );
extern std::string FormatVector2f( const sf::Vector2f& v );

class DebugLogger {
public:
private:
    std::string m_text;
public:
    DebugLogger() { };
    ~DebugLogger() { };
    std::string GetDebugText();
    void SetDebugText( const std::string& text );
    void AddDebugText( const std::string& text );
    std::string FormatDebugHeader();
    std::string FormatInt( const int& i );
    std::string FormatFloat( const float& f );
    std::string FormatVector2i( const sf::Vector2i& v );
    std::string FormatVector2f( const sf::Vector2f& v );
private:
};

#pragma once

#include <string>

// Core Declarations

static const std::string TANKSVERYMUCH_VERSION = "v.0.17.04.30.20";
static const float DEG2RAD = 0.0174532f;

static float globalTime = 1.f;
static float globalScale = 2.f;

extern std::string GetDebugText();
extern void SetDebugText( const std::string& text );
extern void AddDebugText( const std::string& text );

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
private:
};

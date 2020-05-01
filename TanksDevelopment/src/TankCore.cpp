#include "TankCore.h"

// Core Definitions

static DebugLogger debugMgr;

// DebugLogger interface

extern std::string GetDebugText()
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

// DebugLogger implementation

std::string DebugLogger::GetDebugText()
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

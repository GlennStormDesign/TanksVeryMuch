#pragma once

#include <bitset>

#include <SFML/Graphics.hpp>

#include "TankCore.h"
#include "AudioHandle.h"
#include "VFXHandle.h"

// Tank Declarations

const sf::Vector2f TANK_ORIGIN = sf::Vector2f(16.f, 16.f);
const sf::Vector2f SHOTVFX_ORIGIN = sf::Vector2f(16.f, 41.f);
const sf::Color DEF_TANK_COLOR = sf::Color(32.f, 128.f, 80.f);
const sf::Color DEF_SHOTVFX_COLOR = sf::Color(255.f, 240.f, 220.f);
const unsigned int MAX_SHOTVFX_FRAMES = 7;

enum TankControllerType { LocalPlayer, RemotePlayer, Scripted, Drone };
enum LocalInputType { NoLocalInput, Keyboard, GameController, TouchScreen };

const std::bitset<8> BIT_WAIT     = (1<<7);
const std::bitset<8> BIT_FWD      = (1<<6);
const std::bitset<8> BIT_BACK     = (1<<5);
const std::bitset<8> BIT_LEFT     = (1<<4);
const std::bitset<8> BIT_RIGHT    = (1<<3);
const std::bitset<8> BIT_TUR_L    = (1<<2);
const std::bitset<8> BIT_TUR_R    = (1<<1);
const std::bitset<8> BIT_FIRE     = (1<<0);

static sf::FloatRect GetHitBox( const sf::Sprite& sprite, const float& boxSize )
{
    sf::FloatRect retBox = sprite.getGlobalBounds();
    retBox.left += retBox.width * (1.f-boxSize) * 0.5f;
    retBox.top += retBox.height * (1.f-boxSize) * 0.5f;
    retBox.width *= boxSize;
    retBox.height *= boxSize;
    return retBox;
}

class TankController
{
public:
    unsigned int controllerScore = 0;
private:
    bool m_active = true;
    std::bitset<8> controlBits = 0; // wait, forward, back, left, right, turL, turR, fire
    TankControllerType m_type;
    LocalInputType m_input;
    unsigned int m_localJoy = 0;
public:
    TankController() { }
    TankController( const TankControllerType& type ) : m_type(type), m_input(Keyboard) {};
    ~TankController() { }

    bool GetActiveState();
    void SetActiveState( const bool& active );
    bool GetControl( const std::bitset<8>& mask );
    void SetControl( const std::bitset<8>& mask, const bool& val );
    TankControllerType GetControllerType();
    void SetControllerType( const TankControllerType& type );
    LocalInputType GetInputType();
    void SetInputType( const LocalInputType& input );
    void Reset();
    void Update();
    void UpdateLocalPlayerInput();
};

class TankShot {
public:
    bool active = true;
    sf::RectangleShape shot;
private:
    unsigned int m_launchTank;
    float m_posX, m_posY, m_rot;
    float m_speed, m_power, m_life;
    VFXShotImpact m_impactVFX = VFXShotImpact( 0.f, 0.f, 0.f );
    VFXShotSparks m_sparksVFX = VFXShotSparks( 0.f, 0.f, 0.f );
public:
    TankShot()
    {
        ShotInit();
        m_posX = 0.f;
        m_posY = 0.f;
        m_rot = 0.f;
    }
    TankShot( const float& x, const float& y, const float& r )
        : m_posX(x), m_posY(y), m_rot(r)
    {
        ShotInit();
    }
    // REVIEW: need copy constructor?
    ~TankShot() { }

    void ShotInit();
    void Shoot( const float& x, const float& y, const float& r );
    float& GetSpeed();
    void SetSpeed( const float& speed );
    float& GetPower();
    void SetPower( const float& power );
    float& GetLife();
    void SetLife( const float& lifetime );
    bool isShotVFXActive();

    void UpdateShot( float timeDelta );
    void Detonate();
    void DrawShot( sf::RenderWindow& window );
    void DrawShotVFX( sf::RenderWindow& window );
private:
};

class Tank {
public:
    unsigned int tankID = 0;
    TankController controller;
    float fwdMove = 0.f;
    float baseRot = 0.f;
    float turretRot = 0.f;
    TankShot* shots = new TankShot[4]; // REVIEW: plug mem leak with delete[] in destructor, but why does that crash?
private:
    bool m_active = true;
    float m_posX, m_posY, m_baseR, m_turretR;
    bool m_tankMoving, m_turretMoving;
    // sfx loop slot for audioMgr reference, limit to loops available
    int m_idleLoop = -1;
    int m_turretLoop = -1;
    float m_idleVol = IDLE_MIN_VOLUME;
    float m_idlePitch = IDLE_MIN_PITCH;
    float m_localScale = 1.f;
    float m_moveSpeed = 20.f;
    float m_rotSpeed = 45.f;
    float m_animRate = 3.81f;
    float m_shotTimer = 0.f;
    int m_shotFrame = 0;
    sf::Color m_color = sf::Color::White;
    sf::Sprite m_base, m_turret, m_shotVFX;
    float m_armor = 100.f;
    VFXTankDust m_dust = VFXTankDust(0.f,0.f,0.f);
    sf::Vector2f m_dustOffset = sf::Vector2f(5.f,-6.f);
    VFXTankExhaust m_exhaust = VFXTankExhaust(0.f,0.f,0.f);
    sf::Vector2f m_exhaustOffset = sf::Vector2f(0.f,-8.f);
    VFXKillStain m_killStain = VFXKillStain(0.f,0.f,0.f);
    VFXKillRing m_killRing = VFXKillRing(0.f,0.f,0.f);
    VFXKillSparks m_killSparks = VFXKillSparks(0.f,0.f,0.f);
    VFXKillFire m_killFire = VFXKillFire(0.f,0.f,0.f);
    VFXKillDebris m_killDebris = VFXKillDebris(0.f,0.f,0.f,DEF_TANK_COLOR);
    VFXKillFlash m_killFlash = VFXKillFlash(0.f,0.f,0.f);
public:
    Tank() { TankInit(); }
    Tank( TankController control, const float& xPos, const float& yPos, const float& rot, const float& scale )
    {
        TankInit();
        SetController(control);
        SetPosition(xPos, yPos);
        SetBaseRotation(rot);
        SetTankScale(scale);
        m_dust.LaunchVFX(xPos,yPos,rot);
        m_dustOffset *= scale; // TEST: test scale vfx
        m_exhaust.LaunchVFX(xPos,yPos,rot);
        m_exhaustOffset *= scale;
    }
    Tank( const Tank &t )
    {
        tankID = t.tankID;
        controller = t.controller;
        fwdMove = t.fwdMove;
        baseRot = t.baseRot;
        turretRot = t.turretRot;
        shots = new TankShot[4];
        *shots = *t.shots;
        m_active = t.m_active; // wow, copy constructor ignores private modifier? (automatic friend?)
        m_posX = t.m_posX;
        m_posY = t.m_posY;
        m_baseR = t.m_baseR;
        m_turretR = t.m_turretR;
        m_tankMoving = t.m_tankMoving;
        m_turretMoving = t.m_turretMoving;
        m_idleLoop = t.m_idleLoop;
        m_turretLoop = t.m_turretLoop;
        m_idleVol = t.m_idleVol;
        m_idlePitch = t.m_idlePitch;
        m_localScale = t.m_localScale;
        m_moveSpeed = t.m_moveSpeed;
        m_rotSpeed = t.m_rotSpeed;
        m_animRate = t.m_animRate;
        m_shotTimer = t.m_shotTimer;
        m_shotFrame = t.m_shotFrame;
        m_color = t.m_color;
        m_base = t.m_base;
        m_turret = t.m_turret;
        m_shotVFX = t.m_shotVFX;
        m_armor = t.m_armor;
        m_dust = t.m_dust;
        m_dustOffset = t.m_dustOffset;
        m_exhaust = t.m_exhaust;
        m_exhaustOffset = t.m_exhaustOffset;
        m_killStain = t.m_killStain;
        m_killRing = t.m_killRing;
        m_killSparks = t.m_killSparks;
        m_killFire = t.m_killFire;
        m_killDebris = t.m_killDebris;
        m_killFlash = t.m_killFlash;
    }
    ~Tank() { /* delete[] shots; */ }

    void TankInit();
    void TankReset();
    bool& IsTankMoving();
    bool& IsTurretMoving();
    bool& GetActiveState();
    void SetActiveState( const bool& active );
    unsigned int GetTankID();
    void SetTankID( unsigned int id );
    void SetController( const TankController& c );
    void SetPosition( const float& x, const float& y );
    void SetBaseRotation( const float& r );
    void SetTurretRotation( const float& r );
    void SetTankScale( const float& scale );
    void SetMoveSpeed( const float& speed );
    void SetRotSpeed( const float& speed );
    float& GetShotTimer();
    void SetShotTimer( const float& time );
    void SetTankColor( const sf::Color& color );
    void SetTankDustColor( const sf::Color& color );
    sf::Sprite& GetBaseSprite();
    sf::Sprite& GetTurretSprite();
    sf::Sprite& GetShotVFXSprite();
    void SetSprites( const sf::Texture& baseSprite, const sf::Texture& turretSprite, const sf::Texture& shotVFXSprite );
    void SetSpriteScale( const float& globalScale );
    void TransformTank();
    void TransformShotVFX();
    void UpdateTank( const float& timeDelta );
    float& GetArmor();
    void SetArmor( const float& armor );
    bool DamageTank( const float& damage );
    void DestroyTank();
    void ScoreAward( Tank target );
    void DrawTankDustVFX( sf::RenderWindow& window );
    void DrawTankExhaustVFX( sf::RenderWindow& window );
    void DrawKillUnderVFX( sf::RenderWindow& window );
    void DrawKillOverVFX( sf::RenderWindow& window );
};

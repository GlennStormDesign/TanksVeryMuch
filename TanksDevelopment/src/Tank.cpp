#include <cmath>
#include <bitset>

#include "AudioHandle.h"
#include "Tank.h"
#include "TankScene.h"

// Tank Definitions

// TankController implementation

bool TankController::GetActiveState()
{
    return m_active;
}
void TankController::SetActiveState( const bool& active )
{
    m_active = active;
    if ( !active )
    {
        Reset();
        SetControl( BIT_WAIT, true );
    }
}
bool TankController::GetControl( const std::bitset<8>& mask )
{
    return ( ( controlBits & mask ) == mask );
}
void TankController::SetControl( const std::bitset<8>& mask, const bool& val )
{
    controlBits = (val) ? (controlBits | mask) : (controlBits & (~mask));
}
TankControllerType TankController::GetControllerType()
{
    return m_type;
}
void TankController::SetControllerType( const TankControllerType& type )
{
    m_type = type;
}
LocalInputType TankController::GetInputType()
{
    return m_input;
}
void TankController::SetInputType( const LocalInputType& input )
{
    m_input = input;
}
void TankController::Reset() { controlBits = 0; }
void TankController::Update()
{
    // reset, unless drone
    if ( m_type != Drone )
        Reset();
    if ( !m_active )
    {
        SetControl( BIT_WAIT, true );
        return;
    }
    switch (m_type)
    {
        case LocalPlayer:
            // temp hot swap joystick 0
            if ( !sf::Joystick::isConnected(m_localJoy) )
            {
                for ( int i=0; i<8; i++ ) { if (sf::Joystick::isConnected(i)) { m_localJoy = i; break; } }
            }
            if ( m_input == Keyboard && sf::Joystick::isConnected(m_localJoy) )
                SetInputType(GameController);
            else if ( m_input == GameController && !sf::Joystick::isConnected(m_localJoy) )
                SetInputType(Keyboard);
            UpdateLocalPlayerInput();
            break;
        case RemotePlayer:
            break;
        case Scripted:
            break;
        case Drone:
            // temp drone control
            if ( GetControl( BIT_FIRE ) )
            {
                SetControl( BIT_FIRE, false );
            }
            if ( rand() % 300 == 0 )
            {
                if ( GetControl( BIT_BACK ) )
                    Reset();
                SetControl( BIT_FWD, true );
            }
            if ( rand() % 300 == 0 )
            {
                if ( GetControl( BIT_FWD ) )
                    Reset();
                SetControl( BIT_BACK, true );
            }
            else if ( rand() % 100 == 0 )
            {
                if ( GetControl( BIT_RIGHT ) || rand() % 3 == 0 )
                    Reset();
                SetControl( BIT_LEFT, true );
            }
            else if ( rand() % 100 == 0 )
            {
                if ( GetControl( BIT_LEFT ) || rand() % 3 == 0 )
                    Reset();
                SetControl( BIT_RIGHT, true );
            }
            else if ( rand() % 500 == 0 )
            {
                if ( GetControl( BIT_TUR_R ) || rand() % 3 == 0 )
                    Reset();
                SetControl( BIT_TUR_L, true );
            }
            else if ( rand() % 500 == 0 )
            {
                if ( GetControl( BIT_TUR_L ) || rand() % 3 == 0 )
                    Reset();
                SetControl( BIT_TUR_R, true );
            }
            else if ( rand() % 750 == 0 )
            {
                if ( rand() % 4 == 0 )
                    Reset();
                SetControl( BIT_FIRE, true );
            }
            if ( controlBits == 0 )
                SetControl( BIT_WAIT, true );
            break;
        default:
            break;
    }
}
void TankController::UpdateLocalPlayerInput()
{
    float JOY_THRESHOLD = 50.f; // 0.001525975f; <-- apparently some minimum threshold
    switch (m_input) {
        case NoLocalInput:
            break;
        case Keyboard:
            SetControl( BIT_FWD,    sf::Keyboard::isKeyPressed(sf::Keyboard::Up) );
            SetControl( BIT_BACK,   sf::Keyboard::isKeyPressed(sf::Keyboard::Down) );
            SetControl( BIT_LEFT,   sf::Keyboard::isKeyPressed(sf::Keyboard::Left) );
            SetControl( BIT_RIGHT,  sf::Keyboard::isKeyPressed(sf::Keyboard::Right) );
            SetControl( BIT_TUR_L,  sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket) );
            SetControl( BIT_TUR_R,  sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket) );
            SetControl( BIT_FIRE,   sf::Keyboard::isKeyPressed(sf::Keyboard::BackSlash) );
            break;
        case GameController:
            SetControl( BIT_FWD,    (sf::Joystick::getAxisPosition(m_localJoy,sf::Joystick::Y) > JOY_THRESHOLD) );
            SetControl( BIT_BACK,   (sf::Joystick::getAxisPosition(m_localJoy,sf::Joystick::Y) < -JOY_THRESHOLD) );
            SetControl( BIT_LEFT,   (sf::Joystick::getAxisPosition(m_localJoy,sf::Joystick::X) < -JOY_THRESHOLD) );
            SetControl( BIT_RIGHT,  (sf::Joystick::getAxisPosition(m_localJoy,sf::Joystick::X) > JOY_THRESHOLD) );
            SetControl( BIT_TUR_L,  (sf::Joystick::isButtonPressed(m_localJoy,0)) );
            SetControl( BIT_TUR_R,  (sf::Joystick::isButtonPressed(m_localJoy,2)) );
            SetControl( BIT_FIRE,   (sf::Joystick::isButtonPressed(m_localJoy,1)) );
            break;
        case TouchScreen:
            break;
    }
    // if no controls, wait
    SetControl( BIT_WAIT, (controlBits == 0) );
}

// TankShot implementation

void TankShot::ShotInit()
{
    shot.setOrigin(1.f, 4.f);
    shot.setSize(sf::Vector2f(2.f,12.f));
    shot.setFillColor(sf::Color::White);
    shot.setOutlineThickness(1.f);
    shot.setOutlineColor(sf::Color(180,128,16,255));
    shot.setPosition(m_posX, m_posY);
    shot.setRotation(m_rot);
    m_speed = 381.f;
    m_power = 38.1f;
    m_life = 1.f;
}
void TankShot::Shoot( const float& x, const float& y, const float& r )
{
    m_posX = x;
    m_posY = y;
    m_rot = r;
    ShotInit();
    active = true;
}
float& TankShot::GetSpeed()
{
    return m_speed;
}
void TankShot::SetSpeed( const float& speed )
{
    m_speed = speed;
}
float& TankShot::GetPower()
{
    return m_power;
}
void TankShot::SetPower( const float& power )
{
    m_power = power;
}
float& TankShot::GetLife()
{
    return m_life;
}
void TankShot::SetLife( const float& lifetime )
{
    m_life = lifetime;
}
bool TankShot::isShotVFXActive()
{
    return m_impactVFX.active;
}

void TankShot::UpdateShot( float timeDelta )
{
    if ( m_impactVFX.active )
        m_impactVFX.UpdateEmitter( timeDelta );
    if ( m_sparksVFX.active )
        m_sparksVFX.UpdateEmitter( timeDelta );
    if ( !active )
        return;
    float shotMoveFwdX = sin(m_rot * DEG2RAD) * M_PI;
    float shotMoveFwdY = cos(m_rot * DEG2RAD) * M_PI;
    m_posX += shotMoveFwdX * m_speed * timeDelta;
    m_posY -= shotMoveFwdY * m_speed * timeDelta;
    m_life -= timeDelta;
    if ( m_life < 0.f )
        Detonate();
    else
    {
        shot.setPosition(m_posX, m_posY);
        shot.setRotation(m_rot);
    }
}
void TankShot::Detonate()
{
    m_impactVFX.LaunchVFX( m_posX, m_posY, m_rot );
    m_sparksVFX.LaunchVFX( m_posX, m_posY, m_rot );
    active = false;
    m_life = 0.f;
}
void TankShot::DrawShot( sf::RenderWindow& window )
{
    if ( active )
        window.draw( shot );
}
void TankShot::DrawShotVFX( sf::RenderWindow& window )
{
    if ( m_sparksVFX.active )
        m_sparksVFX.DrawParticles( window );
    if ( m_impactVFX.active )
        m_impactVFX.DrawParticles( window );
}

// Tank implementation

void Tank::TankInit()
{
    m_posX = 0.f;
    m_posY = 0.f;
    m_baseR = 0.f;
    m_turretR = 0.f;
    m_tankMoving = false;
    m_turretMoving = false;
    // TODO: set tank sfx loops for idle and turret
    SetTankColor( DEF_TANK_COLOR );
    for ( int i=0; i<4; i++ ) { shots[i].active = false; }
}
void Tank::TankReset()
{
    controller.SetActiveState(true);
    SetArmor(100.f);
    m_dust.active = true;
    m_exhaust.active = true;
}
bool& Tank::IsTankMoving()
{
    return m_tankMoving;
}
bool& Tank::IsTurretMoving()
{
    return m_turretMoving;
}
bool& Tank::GetActiveState()
{
    return m_active;
}
void Tank::SetActiveState( const bool& active )
{
    m_active = active;
}
unsigned int Tank::GetTankID()
{
    return tankID;
}
void Tank::SetTankID( unsigned int id )
{
    tankID = id;
}
void Tank::SetController( const TankController& c )
{
    controller = c;
}
void Tank::SetPosition( const float& x, const float& y )
{
    m_posX = x;
    m_posY = y;
}
void Tank::SetBaseRotation( const float& r )
{
    m_baseR = r;
}
void Tank::SetTurretRotation( const float& r )
{
    m_turretR = r;
}
void Tank::SetTankScale( const float& scale )
{
    m_localScale = scale;
}
void Tank::SetMoveSpeed( const float& speed )
{
    m_moveSpeed = speed;
}
void Tank::SetRotSpeed( const float& speed )
{
    m_rotSpeed = speed;
}
float& Tank::GetShotTimer()
{
    return m_shotTimer;
}
void Tank::SetShotTimer( const float& time )
{
    m_shotTimer = time;
}
void Tank::SetTankColor( const sf::Color& color )
{
    m_color = color;
    m_killDebris.SetDebrisColor(color);
}
void Tank::SetTankDustColor( const sf::Color& color )
{
    m_dust.SetVFXDustColor( color );
}
sf::Sprite& Tank::GetBaseSprite()
{
    return m_base;
}
sf::Sprite& Tank::GetTurretSprite()
{
    return m_turret;
}
sf::Sprite& Tank::GetShotVFXSprite()
{
    return m_shotVFX;
}
void Tank::SetSprites( const sf::Texture& baseSprite, const sf::Texture& turretSprite, const sf::Texture& shotVFXSprite )
{
    m_base.setTexture(baseSprite);
    m_base.setOrigin(TANK_ORIGIN);
    m_base.setColor(m_color);
    m_turret.setTexture(turretSprite);
    m_turret.setOrigin(TANK_ORIGIN);
    m_turret.setColor(m_color);
    m_shotVFX.setTexture(shotVFXSprite);
    m_shotVFX.setOrigin(SHOTVFX_ORIGIN);
    m_shotVFX.setColor(DEF_SHOTVFX_COLOR);
}
void Tank::SetSpriteScale( const float& globalScale )
{
    m_base.setScale(m_localScale * globalScale, m_localScale * globalScale);
    m_turret.setScale(m_localScale * globalScale, m_localScale * globalScale);
    m_shotVFX.setScale(m_localScale * globalScale, m_localScale * globalScale);
}
void Tank::TransformTank()
{
    m_base.setPosition(m_posX, m_posY);
    m_base.setRotation(m_baseR);
    m_turret.setPosition(m_posX, m_posY);
    m_turret.setRotation(m_baseR + m_turretR);
}
void Tank::TransformShotVFX()
{
    m_shotVFX.setPosition(m_posX, m_posY);
    m_shotVFX.setRotation(m_baseR + m_turretR);
}
void Tank::UpdateSFXLoops( const float& timeDelta )
{
    if ( m_tankMoving )
    {
        if ( m_idleLoop == -1 )
            m_idleLoop = LaunchLoopIdle( tankID, IDLE_MIN_VOLUME, IDLE_MIN_PITCH );
        else
        {
            m_idleVol += IDLE_SHIFT_SPEED * timeDelta;
            if ( m_idleVol > IDLE_MAX_VOLUME )
                m_idleVol = IDLE_MAX_VOLUME;
            m_idlePitch += IDLE_SHIFT_SPEED * timeDelta;
            if ( m_idlePitch > IDLE_MAX_PITCH )
                m_idlePitch = IDLE_MAX_PITCH;
            TouchSFXLoop( m_idleLoop, m_idleVol, m_idlePitch, false );
        }
    }
    else if ( m_idleLoop > -1 )
    {
        m_idleVol -= IDLE_SHIFT_SPEED * timeDelta;
        if ( m_idleVol < IDLE_MIN_VOLUME )
            m_idleVol = IDLE_MIN_VOLUME;
        m_idlePitch -= IDLE_SHIFT_SPEED * timeDelta;
        if ( m_idlePitch < IDLE_MIN_PITCH )
            m_idlePitch = IDLE_MIN_PITCH;
        TouchSFXLoop( m_idleLoop, m_idleVol, m_idlePitch, false );
    }
    if ( m_turretMoving )
    {
        if ( m_turretLoop == -1 )
            m_turretLoop = LaunchLoopTurret( tankID, 61.8f, .8f );
    }
    else if ( m_turretLoop > -1 )
    {
        TouchSFXLoop( m_turretLoop, 61.8f, .8f, true );
        m_turretLoop = -1;
    }
}
void Tank::KillSFXLoops()
{
    if ( m_idleLoop > -1 )
    {
        TouchSFXLoop( m_idleLoop, m_idleVol, m_idlePitch, true );
        m_idleLoop = -1;
    }
    if ( m_turretLoop > -1 )
    {
        TouchSFXLoop( m_turretLoop, 61.8f, .8f, true );
        m_turretLoop = -1;
    }
}
void Tank::UpdateTank( const float& timeDelta )
{
    controller.Update();
    m_tankMoving = false;
    m_turretMoving = false;
    for ( int i=0; i<4; i++ ) { shots[i].UpdateShot( timeDelta ); }
    m_dust.VFXTankDustEmit( controller.GetControl( BIT_FWD ) || controller.GetControl( BIT_BACK ) ||
                                controller.GetControl( BIT_LEFT ) || controller.GetControl( BIT_RIGHT ) );
    if ( (int)(timeDelta*10000.f) % 2 == 0 ) m_dustOffset.x *= -1.f; // REVIEW: find another way
    m_dust.UpdateEmitter( timeDelta, m_base.getPosition(), m_base.getRotation(), m_dustOffset );
    m_exhaust.UpdateEmitter( timeDelta, m_base.getPosition(), m_base.getRotation(), m_exhaustOffset );
    m_killStain.UpdateEmitter( timeDelta );
    m_killRing.UpdateEmitter( timeDelta );
    m_killDebris.UpdateEmitter( timeDelta );
    m_killFire.UpdateEmitter( timeDelta );
    m_killSparks.UpdateEmitter( timeDelta );
    m_killFlash.UpdateEmitter( timeDelta );
    if ( !m_active )
        return;
    float tankMoveFwdX = sin(m_baseR * DEG2RAD) * M_PI;
    float tankMoveFwdY = cos(m_baseR * DEG2RAD) * M_PI;
    // handle spacial sound listener direction
    //sf::Listener::setDirection(tankMoveFwdX,tankMoveFwdY,0.f); // REVIEW: not intuitive?
    if ( controller.GetControl( BIT_FWD ) )
    {
        m_posX += tankMoveFwdX * m_moveSpeed * timeDelta;
        m_posY -= tankMoveFwdY * m_moveSpeed * timeDelta;
        m_tankMoving = true;
    }
    if ( controller.GetControl( BIT_BACK ) )
    {
        m_posX -= tankMoveFwdX * m_moveSpeed * timeDelta;
        m_posY += tankMoveFwdY * m_moveSpeed * timeDelta;
        m_tankMoving = true;
    }
    if ( controller.GetControl( BIT_LEFT ) )
    {
        m_baseR -= m_rotSpeed * timeDelta;
        if ( !m_tankMoving )
            m_baseR -= m_rotSpeed * timeDelta;
        m_tankMoving = true;
    }
    if ( controller.GetControl( BIT_RIGHT ) )
    {
        m_baseR += m_rotSpeed * timeDelta;
        if ( !m_tankMoving )
            m_baseR += m_rotSpeed * timeDelta;
        m_tankMoving = true;
    }
    if ( controller.GetControl( BIT_TUR_L ) )
    {
        m_turretR -= m_rotSpeed * timeDelta;
        m_turretMoving = true;
    }
    if ( controller.GetControl( BIT_TUR_R ) )
    {
        m_turretR += m_rotSpeed * timeDelta;
        m_turretMoving = true;
    }
    TransformTank();
    /*
    if ( controller.GetControllerType() == LocalPlayer ) {
        // handle spacial sound and non-player tank sound
        // NOTE: Listener updated in main game loop on sound update
        //LocalTankEngage( m_tankMoving, m_turretMoving ); // temp - now handled in this translation unit
    }
    */
    UpdateSFXLoops( timeDelta );
    if ( m_shotFrame == 0 && controller.GetControl( BIT_FIRE ) )
    {
        bool shotAvailable = false;
        for ( int i=0; i<4; i++ ) {
            if ( !shots[i].active && !shots[i].isShotVFXActive() )
            {
                shots[i].Shoot( m_posX, m_posY, (m_baseR + m_turretR) );
                shotAvailable = true;
                break;
            }
        }
        if ( shotAvailable )
        {
            SetShotTimer( 1.f );
            TransformShotVFX();
            LaunchSFXShot(GetBaseSprite().getPosition());
        }
    }
    if ( m_shotTimer > 0.f )
    {
        m_shotTimer -= timeDelta * m_animRate;
        m_shotFrame = (int)( m_shotTimer * (float)MAX_SHOTVFX_FRAMES );
        switch (m_shotFrame) {
            case 6:
                SetSprites(TexTankBase(), TexTankTurret(), TexVFXShot1());
                break;
            case 5:
                SetSprites(TexTankBase(), TexTurretShot1(), TexVFXShot2());
                break;
            case 4:
                SetSprites(TexTankBase(), TexTurretShot2(), TexVFXShot3());
                break;
            case 3:
                SetSprites(TexTankBase(), TexTurretShot3(), TexVFXShot4());
                break;
            case 2:
                SetSprites(TexTankBase(), TexTurretShot4(), TexVFXShot5());
                break;
            default:
                SetSprites(TexTankBase(), TexTankTurret(), TexVFXShot6());
                break;
        }
    }
}
float& Tank::GetArmor()
{
    return m_armor;
}
void Tank::SetArmor( const float& armor )
{
    m_armor = armor;
    m_exhaust.VFXTankExhauseDamage( 1.f - ( armor / 100.f ) );
}
bool Tank::DamageTank( const float& damage )
{
    SetArmor( m_armor - damage );
    if ( m_armor <= 0.f )
    {
        DestroyTank();
        return true;
    }
    return false;
}
void Tank::DestroyTank()
{
    // REVIEW: signal to player class they have died
    controller.SetActiveState(false);
    SetActiveState(false);
    SetSprites( TexTankBase(), TexTankTurret(), TexVFXShot1() );
    m_dust.VFXTankDustEmit(false);
    m_exhaust.VFXTankExhaustEmit(false);
    // boom
    m_killStain.LaunchVFX( m_posX, m_posY, m_baseR );
    m_killRing.LaunchVFX( m_posX, m_posY, m_baseR );
    m_killDebris.LaunchVFX( m_posX, m_posY, m_baseR );
    m_killFire.LaunchVFX( m_posX, m_posY, m_baseR );
    m_killSparks.LaunchVFX( m_posX, m_posY, m_baseR );
    m_killFlash.LaunchVFX( m_posX, m_posY, m_baseR );
}
void Tank::ScoreAward( Tank target )
{
    // score to player
    controller.controllerScore += 1;
}
void Tank::DrawTankDustVFX( sf::RenderWindow& window )
{
    m_dust.DrawParticles( window );
}
void Tank::DrawTankExhaustVFX( sf::RenderWindow& window )
{
    m_exhaust.DrawParticles( window );
}
void Tank::DrawKillUnderVFX( sf::RenderWindow& window )
{
    m_killStain.DrawParticles( window );
    m_killRing.DrawParticles( window );
    m_killDebris.DrawParticles( window );
}
void Tank::DrawKillOverVFX( sf::RenderWindow& window )
{
    m_killFire.DrawParticles( window );
    m_killSparks.DrawParticles( window );
    m_killFlash.DrawParticles( window );
}

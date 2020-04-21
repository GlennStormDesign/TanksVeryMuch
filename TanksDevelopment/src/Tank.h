#pragma once

// Tank Handling

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

    bool GetActiveState()
    {
        return m_active;
    }
    void SetActiveState( const bool& active )
    {
        m_active = active;
        if ( !active )
        {
            Reset();
            SetControl( BIT_WAIT, true );
        }
    }
    bool GetControl( const std::bitset<8>& mask )
    {
        return ( ( controlBits & mask ) == mask );
    }
    void SetControl( const std::bitset<8>& mask, const bool& val )
    {
        controlBits = (val) ? (controlBits | mask) : (controlBits & (~mask));
    }
    TankControllerType GetControllerType()
    {
        return m_type;
    }
    void SetControllerType( const TankControllerType& type )
    {
        m_type = type;
    }
    LocalInputType GetInputType()
    {
        return m_input;
    }
    void SetInputType( const LocalInputType& input )
    {
        m_input = input;
    }
    void Reset() { controlBits = 0; }
    void Update()
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
    void UpdateLocalPlayerInput()
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

                debugText = "DEBUG -";
                debugText += " JOYID ";
                debugText += m_localJoy;
                debugText += " NAME ";
                debugText += sf::Joystick::getIdentification(m_localJoy).name;
                debugText += " PRODID ";
                debugText += sf::Joystick::getIdentification(m_localJoy).productId;
                debugText += " JOYX ";
                debugText += sf::Joystick::getAxisPosition(m_localJoy,sf::Joystick::X);
                debugText += " JOYY ";
                debugText += sf::Joystick::getAxisPosition(m_localJoy,sf::Joystick::Y);
                debugText += " JOYZ ";
                debugText += sf::Joystick::getAxisPosition(m_localJoy,sf::Joystick::Z);
                debugText += " JOYR ";
                debugText += sf::Joystick::getAxisPosition(m_localJoy,sf::Joystick::R);
                debugText += " JOYU ";
                debugText += sf::Joystick::getAxisPosition(m_localJoy,sf::Joystick::U);
                debugText += " JOYV ";
                debugText += sf::Joystick::getAxisPosition(m_localJoy,sf::Joystick::V);

                break;
            case TouchScreen:
                break;
        }
        // if no controls, wait
        SetControl( BIT_WAIT, (controlBits == 0) );
    }
};

class TankShot {
public:
    unsigned int shotID = 0;
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

    void ShotInit()
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
    void Shoot( const float& x, const float& y, const float& r )
    {
        m_posX = x;
        m_posY = y;
        m_rot = r;
        ShotInit();
        active = true;
    }
    unsigned int GetShotID()
    {
        return shotID;
    }
    void SetShotID( unsigned int id )
    {
        shotID = id;
    }
    float& GetSpeed()
    {
        return m_speed;
    }
    void SetSpeed( const float& speed )
    {
        m_speed = speed;
    }
    float& GetPower()
    {
        return m_power;
    }
    void SetPower( const float& power )
    {
        m_power = power;
    }
    float& GetLife()
    {
        return m_life;
    }
    void SetLife( const float& lifetime )
    {
        m_life = lifetime;
    }

    void UpdateShot( float timeDelta )
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
    void Detonate()
    {
        m_impactVFX.LaunchVFX( m_posX, m_posY, m_rot );
        m_sparksVFX.LaunchVFX( m_posX, m_posY, m_rot );
        active = false;
        m_life = 0.f;
    }
    void DrawShot( sf::RenderWindow& window )
    {
        if ( active )
            window.draw( shot );
    }
    void DrawShotVFX( sf::RenderWindow& window )
    {
        if ( m_sparksVFX.active )
            m_sparksVFX.DrawParticles( window );
        if ( m_impactVFX.active )
            m_impactVFX.DrawParticles( window );
    }
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
    // REVIEW: per TankScene vector of tanks, need copy constructor?
    Tank( const Tank &t )
    {
        tankID = t.tankID;
        controller = t.controller;
        fwdMove = t.fwdMove;
        baseRot = t.baseRot;
        turretRot = t.turretRot;
        shots = new TankShot[4]; // TEST: make shot (pointer), then copy values?
        *shots = *t.shots;
        m_active = t.m_active; // wow, copy constructor ignores private modifier? (automatic friend?)
        m_posX = t.m_posX;
        m_posY = t.m_posY;
        m_baseR = t.m_baseR;
        m_turretR = t.m_turretR;
        m_tankMoving = t.m_tankMoving;
        m_turretMoving = t.m_turretMoving;
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

    void TankInit()
    {
        m_posX = 0.f;
        m_posY = 0.f;
        m_baseR = 0.f;
        m_turretR = 0.f;
        m_tankMoving = false;
        m_turretMoving = false;
        SetTankColor( DEF_TANK_COLOR );
        for ( int i=0; i<4; i++ ) { shots[i].active = false; }
    }
    bool& IsTankMoving()
    {
        return m_tankMoving;
    }
    bool& IsTurretMoving()
    {
        return m_turretMoving;
    }
    bool& GetActiveState()
    {
        return m_active;
    }
    void SetActiveState( const bool& active )
    {
        m_active = active;
    }
    unsigned int GetTankID()
    {
        return tankID;
    }
    void SetTankID( unsigned int id )
    {
        tankID = id;
    }
    void SetController( const TankController& c )
    {
        controller = c;
    }
    void SetPosition( const float& x, const float& y )
    {
        m_posX = x;
        m_posY = y;
    }
    void SetBaseRotation( const float& r )
    {
        m_baseR = r;
    }
    void SetTurretRotation( const float& r )
    {
        m_turretR = r;
    }
    void SetTankScale( const float& scale )
    {
        m_localScale = scale;
    }
    void SetMoveSpeed( const float& speed )
    {
        m_moveSpeed = speed;
    }
    void SetRotSpeed( const float& speed )
    {
        m_rotSpeed = speed;
    }
    float& GetShotTimer()
    {
        return m_shotTimer;
    }
    void SetShotTimer( const float& time )
    {
        m_shotTimer = time;
    }
    void SetTankColor( const sf::Color& color )
    {
        m_color = color;
        m_killDebris.SetDebrisColor(color);
    }
    void SetTankDustColor( const sf::Color& color )
    {
        m_dust.SetVFXDustColor( color );
    }
    sf::Sprite& GetBaseSprite()
    {
        return m_base;
    }
    sf::Sprite& GetTurretSprite()
    {
        return m_turret;
    }
    sf::Sprite& GetShotVFXSprite()
    {
        return m_shotVFX;
    }
    void SetSprites( const sf::Texture& baseSprite, const sf::Texture& turretSprite, const sf::Texture& shotVFXSprite )
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
    void SetSpriteScale( const float& globalScale )
    {
        m_base.setScale(m_localScale * globalScale, m_localScale * globalScale);
        m_turret.setScale(m_localScale * globalScale, m_localScale * globalScale);
        m_shotVFX.setScale(m_localScale * globalScale, m_localScale * globalScale);
    }
    void TransformTank()
    {
        m_base.setPosition(m_posX, m_posY);
        m_base.setRotation(m_baseR);
        m_turret.setPosition(m_posX, m_posY);
        m_turret.setRotation(m_baseR + m_turretR);
    }
    void TransformShotVFX()
    {
        m_shotVFX.setPosition(m_posX, m_posY);
        m_shotVFX.setRotation(m_baseR + m_turretR);
    }
    void UpdateTank( const float& timeDelta )
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
        if ( controller.GetControllerType() == LocalPlayer ) {
            // REVIEW: handle spacial sound and non-player tank sound (SoundSource)
            sfxMgr.pIdleEngaged = m_tankMoving;
            sfxMgr.pTurretEngaged = m_turretMoving;
            sfxMgr.SFXLoopUpdate(timeDelta);
        }
        if ( m_shotFrame == 0 && controller.GetControl( BIT_FIRE ) )
        {
            bool shotAvailable = false;
            for ( int i=0; i<4; i++ ) {
                if ( !shots[i].active )
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
                sfxMgr.LaunchSFXShot();
            }
        }
        if ( m_shotTimer > 0.f )
        {
            m_shotTimer -= timeDelta * m_animRate;
            m_shotFrame = (int)( m_shotTimer * (float)MAX_SHOTVFX_FRAMES );
            switch (m_shotFrame) {
                case 6:
                    SetSprites(texMgr.texTankBase, texMgr.texTankTurret, texMgr.texVFXShot1);
                    break;
                case 5:
                    SetSprites(texMgr.texTankBase, texMgr.texTurretShot1, texMgr.texVFXShot2);
                    break;
                case 4:
                    SetSprites(texMgr.texTankBase, texMgr.texTurretShot2, texMgr.texVFXShot3);
                    break;
                case 3:
                    SetSprites(texMgr.texTankBase, texMgr.texTurretShot3, texMgr.texVFXShot4);
                    break;
                case 2:
                    SetSprites(texMgr.texTankBase, texMgr.texTurretShot4, texMgr.texVFXShot5);
                    break;
                default:
                    SetSprites(texMgr.texTankBase, texMgr.texTankTurret, texMgr.texVFXShot6);
                    break;
            }
        }
    }
    float& GetArmor()
    {
        return m_armor;
    }
    void SetArmor( const float& armor )
    {
        m_armor = armor;
        m_exhaust.VFXTankExhauseDamage( 1.f - ( armor / 100.f ) );
    }
    bool DamageTank( const float& damage )
    {
        SetArmor( m_armor - damage );
        if ( m_armor <= 0.f )
        {
            DestroyTank();
            return true;
        }
        return false;
    }
    void DestroyTank()
    {
        // REVIEW: signal to player class they have died
        controller.SetActiveState(false);
        SetActiveState(false);
        SetSprites( texMgr.texTankBase, texMgr.texTankTurret, texMgr.texVFXShot1 );
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
    void ScoreAward( Tank target )
    {
        // score to player
        controller.controllerScore += 1;
    }
    void DrawTankDustVFX( sf::RenderWindow& window )
    {
        m_dust.DrawParticles( window );
    }
    void DrawTankExhaustVFX( sf::RenderWindow& window )
    {
        m_exhaust.DrawParticles( window );
    }
    void DrawKillUnderVFX( sf::RenderWindow& window )
    {
        m_killStain.DrawParticles( window );
        m_killRing.DrawParticles( window );
        m_killDebris.DrawParticles( window );
    }
    void DrawKillOverVFX( sf::RenderWindow& window )
    {
        m_killFire.DrawParticles( window );
        m_killSparks.DrawParticles( window );
        m_killFlash.DrawParticles( window );
    }
};

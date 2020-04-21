#pragma once

// VFX Handling

const float ENTITY_MIN_VELOCITY = 0.001f;

class Entity {
public:
    float x = 0.f;
    float y = 0.f;
    float r = 0.f;
private:
    bool m_visible = true; // visible = active
    sf::Color m_color = sf::Color::White;
    float m_drag = 0.f;
    float m_angularDrag = 0.f;
    float m_fwdVelocity = 0.f;
    float m_angleVelocity = 0.f;
    float m_lifeTime = -1.f; // -1 = endless
    float m_maxLife = -1.f;
    float m_startScale = 1.f;
    bool m_spriteSpin = true; // entity rot not affected by angle vel or drag
    float m_spriteAngle = 0.f;
    sf::Sprite m_sprite;
public:
    Entity() { }
    ~Entity() { }

    bool& GetVisible() { return m_visible; }
    sf::Color& GetColor() { return m_color; }
    float GetLife() { return (m_lifeTime/m_maxLife); }
    float& GetStartScale() { return m_startScale; }
    void SetVisible( const bool& show ) { m_visible = show; }
    void SetColor( const sf::Color& color ) { m_color = color; }
    void SetTexture( const sf::Texture& entityTexture ) { m_sprite.setTexture(entityTexture); }
    void SetDrag( const float& drag ) { m_drag = drag; }
    void SetAngularDrag( const float& angleDrag ) { m_angularDrag = angleDrag; }
    void SetVelocity( const float& velocity ) { m_fwdVelocity = velocity; }
    void SetAngleVelocity( const float& velocity ) { m_angleVelocity = velocity; }
    void SetLife( const float& lifeTime ) { m_lifeTime = lifeTime; m_maxLife = lifeTime; }
    void SetStartScale( const float& scale ) { m_startScale = scale; }
    void SetSpriteSpin( const bool& spin ) { m_spriteSpin = spin; }
    void SetSpriteAngle( const float& angle ) { m_spriteAngle = angle; }
    sf::Sprite& GetSprite() { return m_sprite; }

    void UpdateEntity( const float& timeDelta )
    {
        if ( !m_visible )
            return;
        if ( m_fwdVelocity != 0.f )
        {
            x += m_fwdVelocity * ( sin(r * DEG2RAD) * M_PI ) * timeDelta;
            y -= m_fwdVelocity * ( cos(r * DEG2RAD) * M_PI ) * timeDelta;
            m_fwdVelocity *= (1.f-m_drag);
            if ( abs(m_fwdVelocity) <= ENTITY_MIN_VELOCITY )
                m_fwdVelocity = 0.f;
        }
        if ( m_angleVelocity != 0.f )
        {
            if ( m_spriteSpin )
                m_spriteAngle += m_angleVelocity * timeDelta;
            else
                r += m_angleVelocity * timeDelta;
            m_angleVelocity *= (1.f-m_angularDrag);
            if ( abs(m_angleVelocity) <= ENTITY_MIN_VELOCITY )
                m_angleVelocity = 0.f;
        }
        m_sprite.setPosition(sf::Vector2f(x,y));
        if ( m_spriteSpin )
            m_sprite.setRotation(m_spriteAngle);
        else
            m_sprite.setRotation(r);
        if ( m_lifeTime > 0.f )
        {
            m_lifeTime -= timeDelta;
            if ( m_lifeTime <= 0.f )
                EntityDeath();
        }
    }
private:
    void EntityDeath() { SetVisible( false ); SetVelocity( 0.f ); SetAngleVelocity( 0.f ); SetLife( 0.f ); }
};

struct ParticleValueFloat {
    float baseValue;
    float maxValue;
};

float EvalFloat( const float& fMin, const float& fMax, const float& progress )
{
    return ( fMin + ( progress * abs( fMax - fMin ) ) );
}

float RangeFloat( const float& fMin, const float& fMax )
{
    return ( fMin + ( static_cast<float>( rand() % 100 ) / 100 ) * abs( fMax - fMin ) );
}

struct ParticleValueColor {
    sf::Color baseValue;
    sf::Color maxValue;
};

// overload multiply operator to find color product (IGNOREME: already available in SFML, but depreciated?)
/*
sf::Color operator* ( const sf::Color& c1, const sf::Color& c2 )
{
    return sf::Color( ( ( c1.r / 255 ) * ( c2.r / 255 ) * 255 ),
                      ( ( c1.g / 255 ) * ( c2.g / 255 ) * 255 ),
                      ( ( c1.b / 255 ) * ( c2.b / 255 ) * 255 ),
                      ( ( c1.a / 255 ) * ( c2.a / 255 ) * 255 ) );
}
*/

sf::Color EvalColor( const sf::Color& cMin, const sf::Color& cMax, const float& progress )
{
    return sf::Color( ( progress * ( cMax.r - cMin.r ) + cMin.r ),
                      ( progress * ( cMax.g - cMin.g ) + cMin.g ),
                      ( progress * ( cMax.b - cMin.b ) + cMin.b ),
                      ( progress * ( cMax.a - cMin.a ) + cMin.a ) );
}

sf::Color RangeColor( const sf::Color& cMin, const sf::Color& cMax )
{
    return sf::Color( ( ( ( ( rand() % 100 ) / 100 ) * ( cMax.r - cMin.r ) ) + cMin.r ),
                      ( ( ( ( rand() % 100 ) / 100 ) * ( cMax.g - cMin.g ) ) + cMin.g ),
                      ( ( ( ( rand() % 100 ) / 100 ) * ( cMax.b - cMin.b ) ) + cMin.b ),
                      ( ( ( ( rand() % 100 ) / 100 ) * ( cMax.a - cMin.a ) ) + cMin.a ) );
}

class ParticleEmitter {
public:
    bool active = true;
    bool playing = true;
    bool emitting = true;
    float emitX, emitY, emitR;
protected:
    std::vector<Entity> m_particles; // REVIEW: getting away w/o (deep) copy constructor b/c it contains simple members?
    int m_maxParticles = 12;
    float m_emitTime = -1.f; // endless emission
    float m_emitTimer = -1.f;
    float m_emitRate = 3.f;
    float m_emitNextTime = 0.f;
    bool m_emitStarted = false;
    bool m_recycleParticles = true;
    int m_emitIndex = 0;
    sf::Texture m_particleTexture;
    int m_particleTexDivisions = 1;
    //bool m_particleTexAnimated = false;
    ParticleValueFloat m_particleLife;
    ParticleValueFloat m_particleScale;
    ParticleValueFloat m_particleLifeScale;
    ParticleValueFloat m_particleStartPosition;
    ParticleValueFloat m_particleSpeed;
    ParticleValueFloat m_particleDrag;
    ParticleValueFloat m_particleStartRotation;
    ParticleValueFloat m_particleAngleSpeed;
    ParticleValueFloat m_particleAngleDrag;
    ParticleValueColor m_particleStartColor;
    ParticleValueColor m_particleLifeGradient;
public:
    ParticleEmitter() { EmitInit(); }
    ParticleEmitter( const float& x, const float& y, const float& r,
                        const sf::Texture& texture )
        : emitX(x), emitY(y), emitR(r), m_particleTexture(texture) { EmitInit(); }
    ~ParticleEmitter() { /* TODO: cleanup each particle */ }

    virtual void VFXInit() { /* set parameters for individual vfx in subclass */ }

    void EmitInit()
    {
        m_particleLife.baseValue = 3.f;
        m_particleLife.maxValue = 3.f;
        m_particleScale.baseValue = 1.f;
        m_particleScale.maxValue = 1.f;
        m_particleLifeScale.baseValue = 1.f;
        m_particleLifeScale.maxValue = 1.f;
        m_particleStartPosition.baseValue = 0.f;
        m_particleStartPosition.maxValue = 0.f;
        m_particleSpeed.baseValue = 10.f;
        m_particleSpeed.maxValue = 10.f;
        m_particleDrag.baseValue = 0.f;
        m_particleDrag.maxValue = 0.f;
        m_particleStartRotation.baseValue = 0.f;
        m_particleStartRotation.maxValue = 0.f;
        m_particleAngleSpeed.baseValue = 0.f;
        m_particleAngleSpeed.maxValue = 0.f;
        m_particleAngleDrag.baseValue = 0.f;
        m_particleAngleDrag.maxValue = 0.f;
        m_particleStartColor.baseValue = sf::Color::White;
        m_particleStartColor.maxValue = sf::Color::White;
        m_particleLifeGradient.baseValue = sf::Color::White;
        m_particleLifeGradient.maxValue = sf::Color::White;

        VFXInit();

        m_emitTimer = m_emitTime;
        m_emitNextTime = ( 1.f / m_emitRate );
        m_particles.reserve(m_maxParticles);
        for ( int i=0; i<m_maxParticles; i++ ) {
            m_particles[i].x = emitX;
            m_particles[i].y = emitY;
            m_particles[i].r = emitR;
            m_particles[i].SetTexture(m_particleTexture);
            // center origin
            int px = m_particleTexture.getSize().x;
            if ( m_particleTexDivisions > 1 )
                px /= m_particleTexDivisions;
            m_particles[i].GetSprite().setOrigin( (px/2), (px/2) );
            m_particles[i].SetVisible(false);
            m_particles[i].SetColor( RangeColor( m_particleStartColor.baseValue, m_particleStartColor.maxValue ) );
        }
    }

    void LaunchVFX( const float& x, const float& y, const float& r )
    {
        if ( active )
            return;
        emitX = x;
        emitY = y;
        emitR = r;
        active = true;
    }
    void UpdateEmitter( const float& timeDelta )
    {
        if ( !active )
            return; // REVIEW: cleanup if not active?
        if ( playing )
        {
            if ( emitting )
            {
                // update emission
                m_emitNextTime -= timeDelta;
                if ( m_emitNextTime < 0.f )
                {
                    m_emitNextTime = 0.f;
                    int startIndex = 0;
                    if ( !m_recycleParticles )
                        startIndex = m_emitIndex;
                    for ( int i=startIndex; i<m_maxParticles; i++ )
                    {
                        if ( !m_particles[i].GetVisible() )
                        {
                            EmitOne( i );
                            m_emitIndex = i;
                            m_emitStarted = true;
                            // need to continue emission (more than just one) if next emit time is less than 1/60 sec
                            m_emitNextTime += ( 1.f / m_emitRate );
                            if ( m_emitNextTime < (1.f/60.f) )
                                continue;
                            else
                                break;
                        }
                    }
                    if ( m_emitIndex == (m_maxParticles-1) )
                    {
                        if ( !m_recycleParticles )
                            emitting = false;
                        else
                            m_emitIndex = 0;
                    }
                }
                if ( m_emitTime != -1.f )
                {
                    m_emitTimer -= timeDelta;
                    if ( m_emitTimer < 0.f )
                    {
                        m_emitTimer = m_emitTime; // reset
                        emitting = false;
                    }
                }
            }
            // update particles
            int countAlive = 0;
            for ( int i=0; i<m_maxParticles; i++ )
            {
                m_particles[i].UpdateEntity(timeDelta);
                float l = m_particles[i].GetLife();
                if ( l > 0.f )
                    countAlive++;
                float ss = m_particles[i].GetStartScale();
                float s = EvalFloat( (m_particleLifeScale.baseValue * ss), (m_particleLifeScale.maxValue * ss), (1.f-l) );
                sf::Vector2f sz( s, s );
                m_particles[i].GetSprite().setScale( sz );
                sf::Color startC = m_particles[i].GetColor() * m_particleLifeGradient.baseValue;
                sf::Color endC = m_particles[i].GetColor() * m_particleLifeGradient.maxValue;
                m_particles[i].GetSprite().setColor( EvalColor( startC, endC, (1.f-l) ) );
            }
            if ( m_emitStarted && playing && countAlive == 0 )
                playing = false;
        }
        else {
            // reset
            active = false;
            playing = true;
            emitting = true;
            m_emitIndex = 0;
            m_emitTimer = m_emitTime;
            m_emitNextTime = 0.f;
            m_emitStarted = false;
        }
    }
    void UpdateEmitter( const float& timeDelta, const sf::Vector2f& parentPos, const float& parentRot, const sf::Vector2f& offset )
    {
        // update as an attached emitter
        // transform offset using parentPos and parentRot (set emitX & emitY)
        emitX = parentPos.x;
        emitY = parentPos.y;
        // handle offset.x
        float transX = cos(parentRot * DEG2RAD) * M_PI;
        float transY = sin(parentRot * DEG2RAD) * M_PI;
        emitX += transX * offset.x;
        emitY += transY * offset.x;
        // handle offset.y
        transX = sin(parentRot * DEG2RAD) * M_PI;
        transY = cos(parentRot * DEG2RAD) * M_PI;
        emitX += transX * offset.y;
        emitY -= transY * offset.y;
        emitR = parentRot;
        UpdateEmitter( timeDelta );
    }
    void DrawParticles( sf::RenderWindow& window )
    {
        if ( !active || !playing )
            return;
        for ( int i=0; i<m_maxParticles; i++ ) {
            sf::Sprite s = m_particles[i].GetSprite();
            window.draw( s );
        }
    }
private:
    void EmitOne( const int& index )
    {
        m_particles[index].x = emitX + RangeFloat( m_particleStartPosition.baseValue, m_particleStartPosition.maxValue );
        m_particles[index].y = emitY + RangeFloat( m_particleStartPosition.baseValue, m_particleStartPosition.maxValue );
        m_particles[index].r = emitR + RangeFloat( m_particleStartRotation.baseValue, m_particleStartRotation.maxValue );
        m_particles[index].SetColor( RangeColor( m_particleStartColor.baseValue, m_particleStartColor.maxValue ) );
        m_particles[index].GetSprite().setColor( m_particles[index].GetColor() );
        sf::IntRect rct;
        int s = 32.f; // REVIEW: find actual texture width in pixels
        if ( m_particleTexDivisions > 1 )
        {
            float sz = static_cast<int>( s / m_particleTexDivisions );
            int rx = ( rand() % m_particleTexDivisions );
            int ry = ( rand() % m_particleTexDivisions );
            rct.left = rx * sz;
            rct.top = ry * sz;
            rct.width = sz;
            rct.height = sz;
            m_particles[index].GetSprite().setTextureRect( rct );
        }
        else
        {
            rct.left = 0.f;
            rct.top = 0.f;
            rct.width = s;
            rct.height = s;
            m_particles[index].GetSprite().setTextureRect( rct );
        }
        float px = RangeFloat( m_particleScale.baseValue, m_particleScale.maxValue );
        m_particles[index].GetSprite().setScale( sf::Vector2f( px, px ) );
        m_particles[index].SetStartScale( px );
        m_particles[index].SetSpriteAngle( RangeFloat( m_particleStartRotation.baseValue, m_particleStartRotation.maxValue ) );
        m_particles[index].SetVelocity( RangeFloat( m_particleSpeed.baseValue, m_particleSpeed.maxValue ) );
        m_particles[index].SetDrag( RangeFloat( m_particleDrag.baseValue, m_particleDrag.maxValue ) );
        m_particles[index].SetAngleVelocity( RangeFloat( m_particleAngleSpeed.baseValue, m_particleAngleSpeed.maxValue ) );
        m_particles[index].SetAngularDrag( RangeFloat( m_particleAngleDrag.baseValue, m_particleAngleDrag.maxValue ) );
        m_particles[index].SetLife( RangeFloat( m_particleLife.baseValue, m_particleLife.maxValue ) );
        m_particles[index].SetVisible(true);
    }
};

class VFXShotImpact : public ParticleEmitter {
public:
    VFXShotImpact( const float& x, const float& y, const float& r ) { ParticleEmitter( x, y, r, texMgr.texParticleDust ); EmitInit(); }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = texMgr.texParticleDust;
        m_maxParticles = 12;
        m_emitTime = .381f;
        m_emitRate = 240.f;
        m_recycleParticles = false;
        m_particleTexDivisions = 4;
        m_particleLife.baseValue = .618f;
        m_particleLife.maxValue = 2.f;
        m_particleScale.baseValue = 1.f;
        m_particleScale.maxValue = 2.f;
        m_particleLifeScale.baseValue = 1.f;
        m_particleLifeScale.maxValue = 7.f;
        m_particleStartPosition.baseValue = 0.f;
        m_particleStartPosition.maxValue = 0.f;
        m_particleSpeed.baseValue = 80.f;
        m_particleSpeed.maxValue = 100.f;
        m_particleDrag.baseValue = .2f;
        m_particleDrag.maxValue = .3f;
        m_particleStartRotation.baseValue = -180.f;
        m_particleStartRotation.maxValue = 180.f;
        m_particleAngleSpeed.baseValue = -90.f;
        m_particleAngleSpeed.maxValue = 90.f;
        m_particleAngleDrag.baseValue = 0.f;
        m_particleAngleDrag.maxValue = 0.1f;
        m_particleStartColor.baseValue = sf::Color(128,128,128,255);
        m_particleStartColor.maxValue = sf::Color(200,200,200,255);
        m_particleLifeGradient.baseValue = sf::Color::White;
        m_particleLifeGradient.maxValue = sf::Color(255,255,255,0);
    }
};

class VFXShotSparks : public ParticleEmitter {
public:
    VFXShotSparks( const float& x, const float& y, const float& r ) { ParticleEmitter( x, y, r, texMgr.texParticleSparks ); EmitInit(); }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = texMgr.texParticleSparks;
        m_maxParticles = 8;
        m_emitTime = .381f;
        m_emitRate = 100.f;
        m_recycleParticles = false;
        m_particleTexDivisions = 4;
        m_particleLife.baseValue = .1f;
        m_particleLife.maxValue = .381f;
        m_particleScale.baseValue = 2.f;
        m_particleScale.maxValue = 1.f;
        m_particleLifeScale.baseValue = 1.f;
        m_particleLifeScale.maxValue = .618f;
        m_particleStartPosition.baseValue = 0.f;
        m_particleStartPosition.maxValue = 0.f;
        m_particleSpeed.baseValue = 80.f;
        m_particleSpeed.maxValue = 160.f;
        m_particleDrag.baseValue = 0.f;
        m_particleDrag.maxValue = .01f;
        m_particleStartRotation.baseValue = 120.f;
        m_particleStartRotation.maxValue = 240.f;
        m_particleAngleSpeed.baseValue = 0.f;
        m_particleAngleSpeed.maxValue = 0.f;
        m_particleAngleDrag.baseValue = 0.f;
        m_particleAngleDrag.maxValue = 0.f;
        m_particleStartColor.baseValue = sf::Color(255,255,200,255);
        m_particleStartColor.maxValue = sf::Color(255,200,200,255);
        m_particleLifeGradient.baseValue = sf::Color::White;
        m_particleLifeGradient.maxValue = sf::Color(255,255,255,128);
    }
};

class VFXKillStain : public ParticleEmitter {
public:
    VFXKillStain( const float& x, const float& y, const float& r ) { ParticleEmitter( x, y, r, texMgr.texMaskRadial ); EmitInit(); }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = texMgr.texMaskRadial;
        m_maxParticles = 1;
        m_emitTime = .381f;
        m_emitRate = 100.f;
        m_recycleParticles = false;
        m_particleTexDivisions = 1;
        m_particleLife.baseValue = 6.18f;
        m_particleLife.maxValue = 10.f;
        m_particleScale.baseValue = 3.81f;
        m_particleScale.maxValue = 6.18f;
        m_particleLifeScale.baseValue = 2.f;
        m_particleLifeScale.maxValue = 2.f;
        m_particleStartPosition.baseValue = -8.f;
        m_particleStartPosition.maxValue = 8.f;
        m_particleSpeed.baseValue = 0.f;
        m_particleSpeed.maxValue = 0.f;
        m_particleDrag.baseValue = 0.f;
        m_particleDrag.maxValue = 0.f;
        m_particleStartRotation.baseValue = 0.f;
        m_particleStartRotation.maxValue = 0.f;
        m_particleAngleSpeed.baseValue = 0.f;
        m_particleAngleSpeed.maxValue = 0.f;
        m_particleAngleDrag.baseValue = 0.f;
        m_particleAngleDrag.maxValue = 0.f;
        m_particleStartColor.baseValue = sf::Color(0,0,0,200);
        m_particleStartColor.maxValue = sf::Color(0,0,0,128);
        m_particleLifeGradient.baseValue = sf::Color::White;
        m_particleLifeGradient.maxValue = sf::Color(255,255,255,0);
    }
};

class VFXKillRing : public ParticleEmitter {
public:
    VFXKillRing( const float& x, const float& y, const float& r ) { ParticleEmitter( x, y, r, texMgr.texMaskRing ); EmitInit(); }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = texMgr.texMaskRing;
        m_maxParticles = 1;
        m_emitTime = .381f;
        m_emitRate = 100.f;
        m_recycleParticles = false;
        m_particleTexDivisions = 1;
        m_particleLife.baseValue = .381f;
        m_particleLife.maxValue = .618f;
        m_particleScale.baseValue = 0.618f;
        m_particleScale.maxValue = 1.f;
        m_particleLifeScale.baseValue = 1.f;
        m_particleLifeScale.maxValue = 38.1f;
        m_particleStartPosition.baseValue = 0.f;
        m_particleStartPosition.maxValue = 0.f;
        m_particleSpeed.baseValue = 0.f;
        m_particleSpeed.maxValue = 0.f;
        m_particleDrag.baseValue = 0.f;
        m_particleDrag.maxValue = 0.f;
        m_particleStartRotation.baseValue = 0.f;
        m_particleStartRotation.maxValue = 0.f;
        m_particleAngleSpeed.baseValue = 0.f;
        m_particleAngleSpeed.maxValue = 0.f;
        m_particleAngleDrag.baseValue = 0.f;
        m_particleAngleDrag.maxValue = 0.f;
        m_particleStartColor.baseValue = sf::Color(240,240,220,128);
        m_particleStartColor.maxValue = sf::Color(240,240,220,128);
        m_particleLifeGradient.baseValue = sf::Color::White;
        m_particleLifeGradient.maxValue = sf::Color(255,255,255,0);
    }
};

class VFXKillFlash : public ParticleEmitter {
public:
    VFXKillFlash( const float& x, const float& y, const float& r ) { ParticleEmitter( x, y, r, texMgr.texMaskRadial ); EmitInit(); }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = texMgr.texMaskRadial;
        m_maxParticles = 1;
        m_emitTime = .381f;
        m_emitRate = 100.f;
        m_recycleParticles = false;
        m_particleTexDivisions = 1;
        m_particleLife.baseValue = .1f;
        m_particleLife.maxValue = .1f;
        m_particleScale.baseValue = 3.81f;
        m_particleScale.maxValue = 6.18f;
        m_particleLifeScale.baseValue = .381f;
        m_particleLifeScale.maxValue = 1.f;
        m_particleStartPosition.baseValue = 0.f;
        m_particleStartPosition.maxValue = 0.f;
        m_particleSpeed.baseValue = 0.f;
        m_particleSpeed.maxValue = 0.f;
        m_particleDrag.baseValue = 0.f;
        m_particleDrag.maxValue = 0.f;
        m_particleStartRotation.baseValue = 0.f;
        m_particleStartRotation.maxValue = 0.f;
        m_particleAngleSpeed.baseValue = 0.f;
        m_particleAngleSpeed.maxValue = 0.f;
        m_particleAngleDrag.baseValue = 0.f;
        m_particleAngleDrag.maxValue = 0.f;
        m_particleStartColor.baseValue = sf::Color(255,240,200,255);
        m_particleStartColor.maxValue = sf::Color(255,255,240,255);
        m_particleLifeGradient.baseValue = sf::Color::White;
        m_particleLifeGradient.maxValue = sf::Color(255,255,255,128);
    }
};

class VFXKillSparks : public ParticleEmitter {
public:
    VFXKillSparks( const float& x, const float& y, const float& r ) { ParticleEmitter( x, y, r, texMgr.texParticleSparks ); EmitInit(); }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = texMgr.texParticleSparks;
        m_maxParticles = 16;
        m_emitTime = .381f;
        m_emitRate = 320.f;
        m_recycleParticles = false;
        m_particleTexDivisions = 4;
        m_particleLife.baseValue = .2f;
        m_particleLife.maxValue = .381f;
        m_particleScale.baseValue = 1.f;
        m_particleScale.maxValue = 1.f;
        m_particleLifeScale.baseValue = 2.f;
        m_particleLifeScale.maxValue = 1.f;
        m_particleStartPosition.baseValue = 0.f;
        m_particleStartPosition.maxValue = 0.f;
        m_particleSpeed.baseValue = 160.f;
        m_particleSpeed.maxValue = 320.f;
        m_particleDrag.baseValue = 0.f;
        m_particleDrag.maxValue = .01f;
        m_particleStartRotation.baseValue = -180.f;
        m_particleStartRotation.maxValue = 180.f;
        m_particleAngleSpeed.baseValue = 0.f;
        m_particleAngleSpeed.maxValue = 0.f;
        m_particleAngleDrag.baseValue = 0.f;
        m_particleAngleDrag.maxValue = 0.f;
        m_particleStartColor.baseValue = sf::Color(255,255,200,255);
        m_particleStartColor.maxValue = sf::Color(255,200,200,255);
        m_particleLifeGradient.baseValue = sf::Color::White;
        m_particleLifeGradient.maxValue = sf::Color(255,255,255,128);
    }
};

class VFXKillFire : public ParticleEmitter {
public:
    VFXKillFire( const float& x, const float& y, const float& r ) { ParticleEmitter( x, y, r, texMgr.texParticleDust ); EmitInit(); }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = texMgr.texParticleDust;
        m_maxParticles = 20;
        m_emitTime = 2.f;
        m_emitRate = 8.f;
        m_recycleParticles = true;
        m_particleTexDivisions = 4;
        m_particleLife.baseValue = 3.81f;
        m_particleLife.maxValue = 6.18f;
        m_particleScale.baseValue = 2.7f;
        m_particleScale.maxValue = 3.81f;
        m_particleLifeScale.baseValue = 1.f;
        m_particleLifeScale.maxValue = 6.18f;
        m_particleStartPosition.baseValue = -12.f;
        m_particleStartPosition.maxValue = 12.f;
        m_particleSpeed.baseValue = 8.f;
        m_particleSpeed.maxValue = 24.f;
        m_particleDrag.baseValue = 0.05f;
        m_particleDrag.maxValue = .1f;
        m_particleStartRotation.baseValue = -180.f;
        m_particleStartRotation.maxValue = 180.f;
        m_particleAngleSpeed.baseValue = -120.f;
        m_particleAngleSpeed.maxValue = 120.f;
        m_particleAngleDrag.baseValue = 0.0f;
        m_particleAngleDrag.maxValue = 0.05f;
        m_particleStartColor.baseValue = sf::Color(255,200,80,240);
        m_particleStartColor.maxValue = sf::Color(255,128,32,240);
        m_particleLifeGradient.baseValue = sf::Color(255,255,255,255);
        m_particleLifeGradient.maxValue = sf::Color(8,8,0,0);
    }
};

class VFXKillDebris : public ParticleEmitter {
    sf::Color m_tankColor;
public:
    VFXKillDebris( const float& x, const float& y, const float& r, const sf::Color& tankColor ) { m_tankColor = tankColor; ParticleEmitter( x, y, r, texMgr.texParticleDebris ); EmitInit(); }
    void SetDebrisColor( const sf::Color& tankColor )
    {
        m_tankColor = tankColor;
        m_particleStartColor.baseValue = m_tankColor;
        m_particleStartColor.maxValue = m_tankColor;
    }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = texMgr.texParticleDebris;
        m_maxParticles = 8;
        m_emitTime = .381f;
        m_emitRate = 100.f;
        m_recycleParticles = false;
        m_particleTexDivisions = 2;
        m_particleLife.baseValue = 6.81f;
        m_particleLife.maxValue = 8.f;
        m_particleScale.baseValue = 3.f;
        m_particleScale.maxValue = 3.f;
        m_particleLifeScale.baseValue = 1.f;
        m_particleLifeScale.maxValue = 1.f;
        m_particleStartPosition.baseValue = -16.f;
        m_particleStartPosition.maxValue = 16.f;
        m_particleSpeed.baseValue = 160.f;
        m_particleSpeed.maxValue = 640.f;
        m_particleDrag.baseValue = 0.2f;
        m_particleDrag.maxValue = 0.618f;
        m_particleStartRotation.baseValue = -180.f;
        m_particleStartRotation.maxValue = 180.f;
        m_particleAngleSpeed.baseValue = -90.f;
        m_particleAngleSpeed.maxValue = 90.f;
        m_particleAngleDrag.baseValue = 0.381f;
        m_particleAngleDrag.maxValue = 0.618f;
        m_particleStartColor.baseValue = m_tankColor;
        m_particleStartColor.maxValue = m_tankColor;
        m_particleLifeGradient.baseValue = sf::Color(180,180,180,255);
        m_particleLifeGradient.maxValue = sf::Color(128,128,128,128);
    }
};

class VFXTankDust : public ParticleEmitter {
public:
    VFXTankDust( const float& x, const float& y, const float& r ) { ParticleEmitter( x, y, r, texMgr.texParticleDust ); EmitInit(); }
    void SetVFXDustColor( const sf::Color& color ) { m_particleLifeGradient.baseValue = color; m_particleLifeGradient.maxValue *= color; }
    void VFXTankDustEmit( const bool& emit )
    {
        emitting = emit;
        active = true;
    }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = texMgr.texParticleDust;
        m_maxParticles = 16;
        m_emitTime = -1.f;
        m_emitRate = 4.f;
        m_recycleParticles = true;
        m_particleTexDivisions = 4;
        m_particleLife.baseValue = 1.f;
        m_particleLife.maxValue = 3.18f;
        m_particleScale.baseValue = 1.f;
        m_particleScale.maxValue = 2.f;
        m_particleLifeScale.baseValue = 1.f;
        m_particleLifeScale.maxValue = 4.f;
        m_particleStartPosition.baseValue = -4.f;
        m_particleStartPosition.maxValue = 4.f;
        m_particleSpeed.baseValue = 1.f;
        m_particleSpeed.maxValue = 8.f;
        m_particleDrag.baseValue = .0618f;
        m_particleDrag.maxValue = .1f;
        m_particleStartRotation.baseValue = -180.f;
        m_particleStartRotation.maxValue = 180.f;
        m_particleAngleSpeed.baseValue = -120.f;
        m_particleAngleSpeed.maxValue = 120.f;
        m_particleAngleDrag.baseValue = 0.f;
        m_particleAngleDrag.maxValue = 0.1f;
        m_particleStartColor.baseValue = sf::Color(160,150,140,200);
        m_particleStartColor.maxValue = sf::Color(140,128,110,128);
        m_particleLifeGradient.baseValue = sf::Color::White;
        m_particleLifeGradient.maxValue = sf::Color(255,255,255,0);
    }
};

class VFXTankExhaust : public ParticleEmitter {
    float m_startExhaustRate = 4.f;
    float m_damagedExhaustRate = 12.f;
    float m_startExhaustLife = 2.f;
    float m_damagedExhaustLife = 3.33f;
    float m_startExhaustScale = 1.f;
    float m_damagedExhaustScale = 2.f;
    sf::Color m_startExhaustColor = sf::Color(80,80,100,128);
    sf::Color m_damagedExhaustColor = sf::Color(0,0,0,255);
public:
    VFXTankExhaust( const float& x, const float& y, const float& r ) { ParticleEmitter( x, y, r, texMgr.texParticleDust ); EmitInit(); }
    void VFXTankExhaustEmit( const bool& emit )
    {
        emitting = emit;
        active = true;
    }
    void VFXTankExhauseDamage( const float& damage )
    {
        m_emitRate = EvalFloat( m_startExhaustRate, m_damagedExhaustRate, damage );
        m_particleLife.baseValue = EvalFloat( m_startExhaustLife, m_damagedExhaustLife, damage );
        m_particleScale.baseValue = EvalFloat( m_startExhaustScale, m_damagedExhaustScale, damage );
        m_particleStartColor.baseValue = EvalColor( m_startExhaustColor, m_damagedExhaustColor, damage );
    }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = texMgr.texParticleDust;
        m_maxParticles = 16;
        m_emitTime = -1.f;
        m_emitRate = 4.f;
        m_recycleParticles = true;
        m_particleTexDivisions = 4;
        m_particleLife.baseValue = 2.f;
        m_particleLife.maxValue = 3.81f;
        m_particleScale.baseValue = 1.f;
        m_particleScale.maxValue = 3.81f;
        m_particleLifeScale.baseValue = 1.f;
        m_particleLifeScale.maxValue = 3.81f;
        m_particleStartPosition.baseValue = -4.f;
        m_particleStartPosition.maxValue = 4.f;
        m_particleSpeed.baseValue = 1.f;
        m_particleSpeed.maxValue = 8.f;
        m_particleDrag.baseValue = .0618f;
        m_particleDrag.maxValue = .1f;
        m_particleStartRotation.baseValue = -180.f;
        m_particleStartRotation.maxValue = 180.f;
        m_particleAngleSpeed.baseValue = -120.f;
        m_particleAngleSpeed.maxValue = 120.f;
        m_particleAngleDrag.baseValue = 0.f;
        m_particleAngleDrag.maxValue = 0.1f;
        m_particleStartColor.baseValue = sf::Color(80,80,100,128);
        m_particleStartColor.maxValue = sf::Color(64,64,80,128);
        m_particleLifeGradient.baseValue = sf::Color::White;
        m_particleLifeGradient.maxValue = sf::Color(255,255,255,0);
    }
};

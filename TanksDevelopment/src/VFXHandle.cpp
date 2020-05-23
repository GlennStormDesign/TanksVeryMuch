#include <cmath>

#include <SFML/Graphics.hpp>

#include "TankCore.h"
#include "ImageHandle.h"
#include "VFXHandle.h"

// VFX Definitions

// Entity implementation

bool& Entity::GetVisible() { return m_visible; }
sf::Color& Entity::GetColor() { return m_color; }
float Entity::GetLife() { return (m_lifeTime/m_maxLife); }
float& Entity::GetStartScale() { return m_startScale; }
void Entity::SetVisible( const bool& show ) { m_visible = show; }
void Entity::SetColor( const sf::Color& color ) { m_color = color; }
void Entity::SetTexture( const sf::Texture& entityTexture ) { m_sprite.setTexture(entityTexture); }
void Entity::SetDrag( const float& drag ) { m_drag = drag; }
void Entity::SetAngularDrag( const float& angleDrag ) { m_angularDrag = angleDrag; }
void Entity::SetVelocity( const float& velocity ) { m_fwdVelocity = velocity; }
void Entity::SetAngleVelocity( const float& velocity ) { m_angleVelocity = velocity; }
void Entity::SetLife( const float& lifeTime ) { m_lifeTime = lifeTime; m_maxLife = lifeTime; }
void Entity::SetStartScale( const float& scale ) { m_startScale = scale; }
void Entity::SetSpriteSpin( const bool& spin ) { m_spriteSpin = spin; }
void Entity::SetSpriteAngle( const float& angle ) { m_spriteAngle = angle; }
sf::Sprite& Entity::GetSprite() { return m_sprite; }

void Entity::UpdateEntity( const float& timeDelta )
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

void Entity::EntityDeath() { SetVisible( false ); SetVelocity( 0.f ); SetAngleVelocity( 0.f ); SetLife( 0.f ); }

float EvalFloat( const float& fMin, const float& fMax, const float& progress )
{
    return ( fMin + ( progress * abs( fMax - fMin ) ) );
}

float RangeFloat( const float& fMin, const float& fMax )
{
    return ( fMin + ( static_cast<float>( rand() % 100 ) / 100 ) * abs( fMax - fMin ) );
}

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

// ParticleEmitter implementation

void ParticleEmitter::EmitInit()
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

void ParticleEmitter::LaunchVFX( const float& x, const float& y, const float& r )
{
    if ( active )
        return;
    emitX = x;
    emitY = y;
    emitR = r;
    active = true;
}
void ParticleEmitter::UpdateEmitter( const float& timeDelta )
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
void ParticleEmitter::UpdateEmitter( const float& timeDelta, const sf::Vector2f& parentPos, const float& parentRot, const sf::Vector2f& offset )
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
void ParticleEmitter::DrawParticles( sf::RenderWindow& window )
{
    if ( !active || !playing )
        return;
    for ( int i=0; i<m_maxParticles; i++ ) {
        sf::Sprite s = m_particles[i].GetSprite();
        window.draw( s );
    }
}

void ParticleEmitter::ClearParticleVector()
{
    m_particles.clear();
}

void ParticleEmitter::EmitOne( const int& index )
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

// individual VFX classes defined in header

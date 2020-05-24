#pragma once

#include "ImageHandle.h"

// VFX Declarations

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

    bool& GetVisible();
    sf::Color& GetColor();
    float GetLife();
    float& GetStartScale();
    void SetVisible( const bool& show );
    void SetColor( const sf::Color& color );
    void SetTexture( const sf::Texture& entityTexture );
    void SetDrag( const float& drag );
    void SetAngularDrag( const float& angleDrag );
    void SetVelocity( const float& velocity );
    void SetAngleVelocity( const float& velocity );
    void SetLife( const float& lifeTime );
    void SetStartScale( const float& scale );
    void SetSpriteSpin( const bool& spin );
    void SetSpriteAngle( const float& angle );
    sf::Sprite& GetSprite();

    void UpdateEntity( const float& timeDelta );
private:
    void EntityDeath();
};

struct ParticleValueFloat {
    float baseValue;
    float maxValue;
};

float EvalFloat( const float& fMin, const float& fMax, const float& progress );
float RangeFloat( const float& fMin, const float& fMax );

struct ParticleValueColor {
    sf::Color baseValue;
    sf::Color maxValue;
};

sf::Color EvalColor( const sf::Color& cMin, const sf::Color& cMax, const float& progress );
sf::Color RangeColor( const sf::Color& cMin, const sf::Color& cMax );

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

    void EmitInit();

    void LaunchVFX( const float& x, const float& y, const float& r );
    void UpdateEmitter( const float& timeDelta );
    void UpdateEmitter( const float& timeDelta, const sf::Vector2f& parentPos, const float& parentRot, const sf::Vector2f& offset );
    void DrawParticles( sf::RenderWindow& window );

    void ClearParticleVector();
private:
    void EmitOne( const int& index );
};

class VFXShotImpact : public ParticleEmitter {
public:
    VFXShotImpact( const float& x, const float& y, const float& r ) { EmitInit(); }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = TexParticleDust();
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
    VFXShotSparks( const float& x, const float& y, const float& r ) { EmitInit(); }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = TexParticleSparks();
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
    VFXKillStain( const float& x, const float& y, const float& r ) { EmitInit(); }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = TexMaskRadial();
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
    VFXKillRing( const float& x, const float& y, const float& r ) { EmitInit(); }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = TexMaskRing();
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
    VFXKillFlash( const float& x, const float& y, const float& r ) { EmitInit(); }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = TexMaskRadial();
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
    VFXKillSparks( const float& x, const float& y, const float& r ) { EmitInit(); }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = TexParticleSparks();
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
    VFXKillFire( const float& x, const float& y, const float& r ) { EmitInit(); }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = TexParticleDust();
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
    VFXKillDebris( const float& x, const float& y, const float& r, const sf::Color& tankColor ) { SetDebrisColor( tankColor ); EmitInit(); }
    void SetDebrisColor( const sf::Color& tankColor )
    {
        m_tankColor = tankColor;
        m_particleStartColor.baseValue = m_tankColor;
        m_particleStartColor.maxValue = m_tankColor;
    }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = TexParticleDebris();
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
    VFXTankDust( const float& x, const float& y, const float& r ) { EmitInit(); }
    void SetVFXDustColor( const sf::Color& color ) { m_particleLifeGradient.baseValue = color; m_particleLifeGradient.maxValue *= color; }
    void VFXTankDustEmit( const bool& emit )
    {
        emitting = emit;
        active = true;
    }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = TexParticleDust();
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
    VFXTankExhaust( const float& x, const float& y, const float& r ) { EmitInit(); }
    void VFXTankExhaustEmit( const bool& emit )
    {
        emitting = emit;
        active = true;
    }
    void VFXTankExhaustDamage( const float& damage )
    {
        m_emitRate = EvalFloat( m_startExhaustRate, m_damagedExhaustRate, damage );
        m_particleLife.baseValue = EvalFloat( m_startExhaustLife, m_damagedExhaustLife, damage );
        m_particleScale.baseValue = EvalFloat( m_startExhaustScale, m_damagedExhaustScale, damage );
        m_particleStartColor.baseValue = EvalColor( m_startExhaustColor, m_damagedExhaustColor, damage );
    }
    void VFXInit() override
    {
        active = false;
        m_particleTexture = TexParticleDust();
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

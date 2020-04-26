#pragma once

#include <SFML/Graphics.hpp>

#include "TankCore.h"
#include "VFXHandle.h"

// SceneObject Declarations

enum ObjectType {
    Default,
    Decoration,
    Animated,
    Trigger,
    Obstacle,
    Destructable
};

class SceneObject {
public:
    unsigned int objectID = 0;
    bool active = true; // use to skip update
    bool visible = true; // use to skip draw
    ObjectType type = Default;
protected:
	// keep object transform separate from sprite
	sf::Vector2f m_objPos = sf::Vector2f(0.f,0.f);
	float m_objRot = 0.f;
	//SceneObject m_parent;
public:
    SceneObject() { ObjectInit(); }
    virtual ~SceneObject() { } // virtual destructor for proper deletion of subclasses via pointer
    virtual SceneObject* clone() const { } // defined in subclasses

    virtual void SceneObjectInit() { } // define in subclasses

    void ObjectInit();

    unsigned int& GetObjectID();
    void SetObjectID( const unsigned int& id );
    sf::Vector2f& GetObjPos();
    void SetObjPos( const sf::Vector2f& pos );
    float& GetObjRot();
    void SetObjRot( const float& rot );

    virtual void SceneObjectUpdate( const float& timeDelta ) { } // define in subclasses
    virtual void DrawSceneObject( sf::RenderWindow& window, const sf::Vector2f& viewPos )  { } // define in subclasses
private:
};

class SceneDecoration : public SceneObject {
public:
private:
    sf::Image m_defaultImage; // TEST: texture instead?
    sf::Texture m_texture;
    sf::Sprite m_sprite;
public:
    SceneDecoration() { ObjectInit(); }
    ~SceneDecoration() { }
    SceneObject* clone() const override { return new SceneDecoration( *this ); }

    void SceneObjectInit() override
    {
        type = Decoration;
    }

    sf::Image& GetBaseImage();
    void SetBaseImage( const sf::Image& img );
    sf::Sprite& GetSprite();
    void SetSprite( sf::Sprite sprite );

    void DrawSceneObject( sf::RenderWindow& window, const sf::Vector2f& viewPos ) override
    {
        m_sprite.setPosition( GetObjPos() );
        m_sprite.setRotation( GetObjRot() );
        window.draw( m_sprite );
    }
private:
};

class AnimatedDecoration : public SceneDecoration {
public:
private:
    std::vector<sf::Image> m_imageSequence; // TEST: texture instead?
    bool m_animLoop = true;
    float m_animRate = 0.083f;
    float m_animTime = 0.f;
    unsigned int m_animFrame = 0;
public:
    AnimatedDecoration() { ObjectInit(); }
    ~AnimatedDecoration() { }
    SceneObject* clone() const override { return new AnimatedDecoration( *this ); }

    void SceneObjectInit() override
    {
        type = Animated;
    }

    void SceneObjectUpdate( const float& timeDelta ) override { /* increment frame */ }

    void SetAnimSequence( const std::vector<sf::Image>& seq, const bool& loop, const float& rate );
private:
};

class CollidableObject : public SceneObject {
public:
private:
    // TEST: migrate GetHitBox() from Tank.h? use this? alongside?
    sf::FloatRect m_hitbox = sf::FloatRect(0.f,0.f,0.f,0.f);
	float m_mass = 1.f; // REVIEW: evaluate mass value against tank and shot behavior
public:
    CollidableObject() { ObjectInit(); }
    ~CollidableObject() { }

    sf::FloatRect GetHitBox();
    void SetHitBox( sf::FloatRect box );

    virtual void CollisionTrigger( const sf::Vector2f& hitVector, const float& hitForce ) { /* subclass define */ } // REVIEW: define collider object as type enum
private:
};

class SceneTrigger : public CollidableObject {
public:
private:
    // TODO: target to signal upon trigger
public:
    SceneTrigger() { ObjectInit(); }
    ~SceneTrigger() { }
    SceneObject* clone() const override { return new SceneTrigger( *this ); }

    void SceneObjectInit() override
    {
        type = Trigger;
    }

    void CollisionTrigger( const sf::Vector2f& hitVector, const float& hitForce ) override
    {
        // TEST: trigger receiver class?
    }
private:
};

class SceneObstacle : public CollidableObject {
public:
private:
    sf::Sprite m_sprite;
public:
    SceneObstacle() { ObjectInit(); }
    ~SceneObstacle() { }
    SceneObject* clone() const override { return new SceneObstacle( *this ); }

    void SceneObjectInit() override
    {
        type = Obstacle;
    }

    sf::Sprite& GetSprite();
    void SetSprite( sf::Sprite sprite );

    void CollisionTrigger( const sf::Vector2f& hitVector, const float& hitForce ) override
    {
        // TODO: physics push
        // TODO: physics push back as resistance
    }

    void DrawSceneObject( sf::RenderWindow& window, const sf::Vector2f& viewPos ) override
    {
        m_sprite.setPosition( GetObjPos() );
        m_sprite.setRotation( GetObjRot() );
        window.draw( m_sprite );
    }
private:
};

class SceneDestructable : public CollidableObject {
public:
private:
    std::vector<sf::Image> m_damagedImage; // a progression of damage
    float m_durability = 100.f;
    std::vector<ParticleEmitter> m_destroyVFX;
public:
    SceneDestructable() { ObjectInit(); }
    ~SceneDestructable() { }
    SceneObject* clone() const override { return new SceneDestructable( *this ); }

    void SceneObjectInit() override
    {
        type = Destructable;
    }

    void SetDamageImages( const std::vector<sf::Image>& images );
    float& GetDurability();
    void SetDurability( const float& durability );
    void SetDestroyVFX( const std::vector<ParticleEmitter>& destroyVFX );

    bool TakeDamage( float damageAmount );
private:
    void Destroy();
};

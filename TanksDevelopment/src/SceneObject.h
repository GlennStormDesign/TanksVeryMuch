#pragma once

#include <SFML/Graphics.hpp>

#include "TankCore.h"
#include "Tank.h"
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

// modules of functionality can be used to compose SceneObject
class DisplayObject {
public:
protected:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
public:
    void SetTexture( const sf::Texture& texture );
    sf::Sprite& GetSprite();
    void SetSprite( sf::Sprite sprite );

    void DrawSceneObject( sf::RenderWindow& window, const sf::Vector2f& viewPos );
protected:
};

class AnimatedObject {
public:
protected:
    std::vector<sf::Image> m_imageSequence; // REVIEW: image or texture sequence
    bool m_animLoop = true;
    float m_animRate = 0.083f;
    float m_animTime = 0.f;
    unsigned int m_animFrame = 0;
public:
    virtual void SetAnimSequence( const std::vector<sf::Image>& seq, const bool& loop, const float& rate );
protected:
};

class TriggerTarget {
public:
private:
public:
    virtual void DoTrigger();
private:
};

class TriggerableObject {
public:
protected:
    TriggerTarget m_target; // TODO: connect to trigger events via CollisionTrigger and ShotTrigger
public:
protected:
};

class CollidableObject {
public:
protected:
    sf::FloatRect m_hitbox = sf::FloatRect(0.f,0.f,0.f,0.f);
	float m_mass = 1.f; // REVIEW: evaluate mass value against tank and shot behavior
public:
protected:
};

class DestructableObject {
public:
protected:
    std::vector<sf::Image> m_damagedImage; // a progression of damage
    float m_durability = 100.f;
    std::vector<ParticleEmitter> m_destroyVFX;
public:
protected:
};

class SceneObject : public DisplayObject, AnimatedObject, TriggerableObject, CollidableObject, DestructableObject {
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

    // general scene object
    unsigned int& GetObjectID();
    void SetObjectID( const unsigned int& id );
    sf::Vector2f& GetObjPos();
    void SetObjPos( const sf::Vector2f& pos );
    float& GetObjRot();
    void SetObjRot( const float& rot );

    void CollisionTrigger( const Tank& colliderTank );
    void ShotTrigger( const TankShot& colliderShot );

    sf::FloatRect GetHitBox();
    void SetHitBox( sf::FloatRect box );

    void CollisionEvent( const sf::Vector2f& hitVector, const float& hitForce );

    void SetDamageImages( const std::vector<sf::Image>& images );
    float& GetDurability();
    void SetDurability( const float& durability );
    void SetDestroyVFX( const std::vector<ParticleEmitter>& destroyVFX );

    bool TakeDamage( float damageAmount );
    void DestroyObject();

    void SceneObjectUpdate( const float& timeDelta );
protected:
private:
};

class SceneDecoration : public SceneObject {
public:
private:
public:
    SceneDecoration() { ObjectInit(); }
    ~SceneDecoration() { }
    SceneObject* clone() const override { return new SceneDecoration( *this ); }

    void SceneObjectInit() override
    {
        type = Decoration;
    }
private:
};

class AnimatedDecoration : public SceneObject {
public:
private:
public:
    AnimatedDecoration() { ObjectInit(); }
    ~AnimatedDecoration() { }
    SceneObject* clone() const override { return new AnimatedDecoration( *this ); }

    void SceneObjectInit() override
    {
        type = Animated;
    }
private:
};

class SceneTrigger : public SceneObject {
public:
private:
public:
    SceneTrigger() { ObjectInit(); }
    ~SceneTrigger() { }
    SceneObject* clone() const override { return new SceneTrigger( *this ); }

    void SceneObjectInit() override
    {
        type = Trigger;
    }
private:
};

class SceneObstacle : public SceneObject {
public:
private:
public:
    SceneObstacle() { ObjectInit(); }
    ~SceneObstacle() { }
    SceneObject* clone() const override { return new SceneObstacle( *this ); }

    void SceneObjectInit() override
    {
        type = Obstacle;
    }
private:
};

class SceneDestructable : public SceneObject {
public:
private:
public:
    SceneDestructable() { ObjectInit(); }
    ~SceneDestructable() { }
    SceneObject* clone() const override { return new SceneDestructable( *this ); }

    void SceneObjectInit() override
    {
        type = Destructable;
    }
private:
};

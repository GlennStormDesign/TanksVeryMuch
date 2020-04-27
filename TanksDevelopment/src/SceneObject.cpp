#include "SceneObject.h"

// SceneObject Definitions

// DisplayObject implementation

void DisplayObject::SetTexture( const sf::Texture& texture )
{
    m_texture = texture;
    m_sprite.setTexture(m_texture);
    m_sprite.setScale( globalScale, globalScale );
    m_sprite.setOrigin( sf::Vector2f(16.f, 16.f) );
}
sf::Sprite& DisplayObject::GetSprite()
{
    return m_sprite;
}
void DisplayObject::SetSprite( sf::Sprite sprite )
{
    m_sprite = sprite;
}

void DisplayObject::DrawSceneObject( sf::RenderWindow& window, const sf::Vector2f& viewPos )
{
    window.draw( m_sprite );
}

// AnimatedObject implementation

void AnimatedObject::SetAnimSequence( const std::vector<sf::Image>& seq, const bool& loop, const float& rate )
{
    m_imageSequence = seq;
    m_animLoop = loop;
    m_animRate = rate;
}

// ColliderObject implementation

sf::FloatRect ColliderObject::GetHitBox()
{
    return m_hitbox;
}
void ColliderObject::SetHitBox( sf::FloatRect box )
{
    m_hitbox = box;
}

void ColliderObject::CollisionTrigger( const sf::Vector2f& hitVector, const float& hitForce ) { }

// SceneObject implementation

void SceneObject::ObjectInit()
{
    // general config
    SceneObjectInit();
    // specific config
}

unsigned int& SceneObject::GetObjectID()
{
    return objectID;
}
void SceneObject::SetObjectID( const unsigned int& id )
{
    objectID = id;
}
sf::Vector2f& SceneObject::GetObjPos()
{
    return m_objPos;
}
void SceneObject::SetObjPos( const sf::Vector2f& pos )
{
    m_objPos = pos;
}
float& SceneObject::GetObjRot()
{
    return m_objRot;
}
void SceneObject::SetObjRot( const float& rot )
{
    m_objRot = rot;
}

// SceneDecoration implementation (subclass of SceneObject)

// AnimatedDecoration implementation (subclass of SceneDecoration)

// CollidableObject implementation (subclass of SceneObject)

/*
sf::FloatRect CollidableObject::GetHitBox()
{
    return m_hitbox;
}
void CollidableObject::SetHitBox( sf::FloatRect box )
{
    m_hitbox = box;
}
*/

// SceneTrigger implementation (subclass of CollidableObject)

// SceneObstacle implementation (subclass of CollidableObject)

// SceneDestructable implementation (subclass of CollidableObject)

void SceneDestructable::SetDamageImages( const std::vector<sf::Image>& images )
{
    m_damagedImage = images;
}
float& SceneDestructable::GetDurability()
{
    return m_durability;
}
void SceneDestructable::SetDurability( const float& durability )
{
    m_durability = durability;
}
void SceneDestructable::SetDestroyVFX( const std::vector<ParticleEmitter>& destroyVFX )
{
    m_destroyVFX = destroyVFX;
}

bool SceneDestructable::TakeDamage( float damageAmount ) { return false; } // change image, if durability less than zero, destroy
void SceneDestructable::Destroy() { } // switch image tex, launch vfx

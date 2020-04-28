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

// CollidableObject implementation

void CollidableObject::CollisionTrigger( const sf::Vector2f& hitVector, const float& hitForce ) { }

// DestructableObject implementation

bool DestructableObject::TakeDamage( float damageAmount ) { return false; } // change image, if durability less than zero, destroy
void DestructableObject::DestroyObject() { } // switch image tex, launch vfx

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

void SceneObject::SceneObjectUpdate( const float& timeDelta )
{
    m_sprite.setPosition( GetObjPos() );
    m_sprite.setRotation( GetObjRot() );
    // REVIEW: to manage origin of sprite in relation to hitbox
    sf::FloatRect retBox = m_hitbox;
    retBox.left = m_objPos.x - (m_sprite.getOrigin().x / 2);
    retBox.top = m_objPos.y - (m_sprite.getOrigin().y / 2);
    m_hitbox = retBox;
}

sf::FloatRect SceneObject::GetHitBox()
{
    return m_hitbox;
}
void SceneObject::SetHitBox( sf::FloatRect box )
{
    m_hitbox = box;
}

void SceneObject::SetDamageImages( const std::vector<sf::Image>& images )
{
    m_damagedImage = images;
}
float& SceneObject::GetDurability()
{
    return m_durability;
}
void SceneObject::SetDurability( const float& durability )
{
    m_durability = durability;
}
void SceneObject::SetDestroyVFX( const std::vector<ParticleEmitter>& destroyVFX )
{
    m_destroyVFX = destroyVFX;
}

// SceneDecoration implementation (subclass of SceneObject)

// AnimatedDecoration implementation (subclass of SceneDecoration)

// SceneTrigger implementation (subclass of CollidableObject)

// SceneObstacle implementation (subclass of CollidableObject)

// SceneDestructable implementation (subclass of CollidableObject)


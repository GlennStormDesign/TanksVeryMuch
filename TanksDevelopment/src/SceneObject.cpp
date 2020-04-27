#include "SceneObject.h"

// SceneObject Definitions

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

sf::Image& SceneDecoration::GetBaseImage()
{
    return m_defaultImage;
}
/*
void SceneDecoration::SetBaseImage( const sf::Image& img )
{
    m_defaultImage = img;
    // TEST: separate into other function?
    m_texture.loadFromImage( m_defaultImage );
    m_sprite.setTexture( m_texture );
    m_sprite.setScale( globalScale, globalScale );
    m_sprite.setOrigin( sf::Vector2f(16.f, 16.f) );
}
*/
void SceneDecoration::SetTexture( const sf::Texture& texture )
{
    m_texture = texture;
    m_sprite.setTexture( m_texture );
    m_sprite.setScale( globalScale, globalScale );
    m_sprite.setOrigin( sf::Vector2f(16.f, 16.f) );
}
sf::Sprite& SceneDecoration::GetSprite()
{
    return m_sprite;
}
void SceneDecoration::SetSprite( sf::Sprite sprite )
{
    m_sprite = sprite;
}

// AnimatedDecoration implementation (subclass of SceneDecoration)

void AnimatedDecoration::SetAnimSequence( const std::vector<sf::Image>& seq, const bool& loop, const float& rate )
{
    m_imageSequence = seq;
    m_animLoop = loop;
    m_animRate = rate;
}

// CollidableObject implementation (subclass of SceneObject)

sf::FloatRect CollidableObject::GetHitBox()
{
    return m_hitbox;
}
void CollidableObject::SetHitBox( sf::FloatRect box )
{
    m_hitbox = box;
}

// SceneTrigger implementation (subclass of CollidableObject)

// SceneObstacle implementation (subclass of CollidableObject)

sf::Sprite& SceneObstacle::GetSprite()
{
    return m_sprite;
}
void SceneObstacle::SetSprite( sf::Sprite sprite )
{
    m_sprite = sprite;
    m_sprite.setScale( globalScale, globalScale );
    m_sprite.setOrigin( sf::Vector2f(16.f, 16.f) );
}

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

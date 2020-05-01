#include <SFML/Graphics.hpp>

#include "ImageHandle.h"

// Image Definitions

static TextureManager texMgr;

// TextureManager interface

extern const sf::Texture& TexTankBase()
{
    return texMgr.texTankBase;
}
extern const sf::Texture& TexTankTurret()
{
    return texMgr.texTankTurret;
}
extern const sf::Texture& TexTurretShot1()
{
    return texMgr.texTurretShot1;
}
extern const sf::Texture& TexTurretShot2()
{
    return texMgr.texTurretShot2;
}
extern const sf::Texture& TexTurretShot3()
{
    return texMgr.texTurretShot3;
}
extern const sf::Texture& TexTurretShot4()
{
    return texMgr.texTurretShot4;
}
extern const sf::Texture& TexVFXShot1()
{
    return texMgr.texVFXShot1;
}
extern const sf::Texture& TexVFXShot2()
{
    return texMgr.texVFXShot2;
}
extern const sf::Texture& TexVFXShot3()
{
    return texMgr.texVFXShot3;
}
extern const sf::Texture& TexVFXShot4()
{
    return texMgr.texVFXShot4;
}
extern const sf::Texture& TexVFXShot5()
{
    return texMgr.texVFXShot5;
}
extern const sf::Texture& TexVFXShot6()
{
    return texMgr.texVFXShot6;
}
extern const sf::Texture& TexParticleDebris()
{
    return texMgr.texParticleDebris;
}
extern const sf::Texture& TexParticleDust()
{
    return texMgr.texParticleDust;
}
extern const sf::Texture& TexParticleSparks()
{
    return texMgr.texParticleSparks;
}
extern const sf::Texture& TexMaskNoise()
{
    return texMgr.texMaskNoise;
}
extern const sf::Texture& TexMaskRadial()
{
    return texMgr.texMaskRadial;
}
extern const sf::Texture& TexMaskRing()
{
    return texMgr.texMaskRing;
}
extern const sf::Texture& TexSoilBase()
{
    return texMgr.texSoilBase;
}
extern const sf::Texture& TexSoilRock()
{
    return texMgr.texSoilRock;
}
extern const sf::Texture& TexSoilRoot()
{
    return texMgr.texSoilRoot;
}
extern const sf::Texture& TexSoilPebble()
{
    return texMgr.texSoilPebble;
}
extern const sf::Texture& TexSoilGrass()
{
    return texMgr.texSoilGrass;
}
extern const sf::Texture& TexSoilFlower()
{
    return texMgr.texSoilFlower;
}
extern const sf::Texture& TexSnowBase()
{
    return texMgr.texSnowBase;
}
extern const sf::Texture& TexSnowCracks()
{
    return texMgr.texSnowCracks;
}
extern const sf::Texture& TexSnowGrass()
{
    return texMgr.texSnowGrass;
}
extern const sf::Texture& TexSnowIce()
{
    return texMgr.texSnowIce;
}
extern const sf::Texture& TexSandBase()
{
    return texMgr.texSandBase;
}
extern const sf::Texture& TexSandWaves()
{
    return texMgr.texSandWaves;
}
extern const sf::Texture& TexSandRocks()
{
    return texMgr.texSandRocks;
}
extern const sf::Texture& TexSandGrass()
{
    return texMgr.texSandGrass;
}
extern const sf::Texture& TexMeadowBase()
{
    return texMgr.texMeadowBase;
}
extern const sf::Texture& TexMeadowRoots()
{
    return texMgr.texMeadowRoots;
}
extern const sf::Texture& TexMeadowGrass()
{
    return texMgr.texMeadowGrass;
}
extern const sf::Texture& TexMeadowFlowers()
{
    return texMgr.texMeadowFlowers;
}
extern const sf::Texture& TexObjectBush()
{
    return texMgr.texObjectBush;
}
extern const sf::Texture& TexObjectRock()
{
    return texMgr.texObjectRock;
}
extern const sf::Texture& TexUIElement()
{
    return texMgr.texUIElement;
}

// TextureManager implementation

void TextureManager::TankTextureInit()
{
    texTankBase.loadFromFile("image/TankBase_Temp.png", TANKS_TEX_SIZE);
    texTankTurret.loadFromFile("image/TankTurret_Temp.png", TANKS_TEX_SIZE);
    texTurretShot1.loadFromFile("image/TankTurret_Temp_Shot1.png", TANKS_TEX_SIZE);
    texTurretShot2.loadFromFile("image/TankTurret_Temp_Shot2.png", TANKS_TEX_SIZE);
    texTurretShot3.loadFromFile("image/TankTurret_Temp_Shot3.png", TANKS_TEX_SIZE);
    texTurretShot4.loadFromFile("image/TankTurret_Temp_Shot4.png", TANKS_TEX_SIZE);
}
void TextureManager::VFXTextureInit()
{
    texVFXShot1.loadFromFile("image/VFX_Shot1.png", TANKS_TEX_SIZE);
    texVFXShot2.loadFromFile("image/VFX_Shot2.png", TANKS_TEX_SIZE);
    texVFXShot3.loadFromFile("image/VFX_Shot3.png", TANKS_TEX_SIZE);
    texVFXShot4.loadFromFile("image/VFX_Shot4.png", TANKS_TEX_SIZE);
    texVFXShot5.loadFromFile("image/VFX_Shot5.png", TANKS_TEX_SIZE);
    texVFXShot6.loadFromFile("image/VFX_Shot6.png", TANKS_TEX_SIZE);
}
void TextureManager::ParticleTextureInit()
{
    texParticleDebris.loadFromFile("image/Particle_Debris.png", TANKS_TEX_SIZE);
    texParticleDust.loadFromFile("image/Particle_Dust.png", TANKS_TEX_SIZE);
    texParticleSparks.loadFromFile("image/Particle_Sparks.png", TANKS_TEX_SIZE);
}
void TextureManager::MaskTextureInit()
{
    texMaskNoise.loadFromFile("image/Mask_Noise.png", TANKS_TEX_SIZE);
    texMaskRadial.loadFromFile("image/Mask_Radial.png", TANKS_TEX_SIZE);
    texMaskRing.loadFromFile("image/Mask_Ring.png", TANKS_TEX_SIZE);
}
void TextureManager::TerrainTextureInit()
{
    texSoilComposite.loadFromFile("image/Terrain_Soil.png", TANKS_TEX_SIZE); // temp
    texSoilBase.loadFromFile("image/Terrain_Soil_Base.png", TANKS_TEX_SIZE);
    texSoilRock.loadFromFile("image/Terrain_Soil_Rock.ong", TANKS_TEX_SIZE);
    texSoilRoot.loadFromFile("image/Terrain_Soil_Root.png", TANKS_TEX_SIZE);
    texSoilPebble.loadFromFile("image/Terrain_Soil_Pebble.png", TANKS_TEX_SIZE);
    texSoilGrass.loadFromFile("image/Terrain_Soil_Grass.png", TANKS_TEX_SIZE);
    texSoilFlower.loadFromFile("image/Terrain_Soil_Flower.png", TANKS_TEX_SIZE);
    texSnowBase.loadFromFile("image/Terrain_Snow_Base.png", TANKS_TEX_SIZE);
    texSnowCracks.loadFromFile("image/Terrain_Snow_Cracks.png", TANKS_TEX_SIZE);
    texSnowGrass.loadFromFile("image/Terrain_Snow_Grass.png", TANKS_TEX_SIZE);
    texSnowIce.loadFromFile("image/Terrain_Snow_Ice.png", TANKS_TEX_SIZE);
    texSandBase.loadFromFile("image/Terrain_Sand_Base.png", TANKS_TEX_SIZE);
    texSandWaves.loadFromFile("image/Terrain_Sand_Waves.png", TANKS_TEX_SIZE);
    texSandRocks.loadFromFile("image/Terrain_Sand_Rocks.png", TANKS_TEX_SIZE);
    texSandGrass.loadFromFile("image/Terrain_Sand_Grass.png", TANKS_TEX_SIZE);
    texMeadowBase.loadFromFile("image/Terrain_Meadow_Base.png", TANKS_TEX_SIZE);
    texMeadowRoots.loadFromFile("image/Terrain_Meadow_Roots.png", TANKS_TEX_SIZE);
    texMeadowGrass.loadFromFile("image/Terrain_Meadow_Grass.png", TANKS_TEX_SIZE);
    texMeadowFlowers.loadFromFile("image/Terrain_Meadow_Flowers.png", TANKS_TEX_SIZE);
}
void TextureManager::ObjectTextureInit()
{
    texObjectBush.loadFromFile("image/Object_Bush.png", TANKS_TEX_SIZE);
    texObjectRock.loadFromFile("image/Object_Rock.png", TANKS_TEX_SIZE);
}
void TextureManager::UITextureInit()
{
    texUIElement.loadFromFile("image/UI_Element.png", TANKS_TEX_SIZE);
}

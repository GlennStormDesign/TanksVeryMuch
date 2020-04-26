#include <SFML/Graphics.hpp>

#include "ImageHandle.h"

// Image Definitions

// TextureManager implementation

void TextureManager::TankTextureInit()
{
    texTankBase.loadFromFile("image/TankBase_Temp.png", tanksTexSize);
    texTankTurret.loadFromFile("image/TankTurret_Temp.png", tanksTexSize);
    texTurretShot1.loadFromFile("image/TankTurret_Temp_Shot1.png", tanksTexSize);
    texTurretShot2.loadFromFile("image/TankTurret_Temp_Shot2.png", tanksTexSize);
    texTurretShot3.loadFromFile("image/TankTurret_Temp_Shot3.png", tanksTexSize);
    texTurretShot4.loadFromFile("image/TankTurret_Temp_Shot4.png", tanksTexSize);
}
void TextureManager::VFXTextureInit()
{
    texVFXShot1.loadFromFile("image/VFX_Shot1.png", tanksTexSize);
    texVFXShot2.loadFromFile("image/VFX_Shot2.png", tanksTexSize);
    texVFXShot3.loadFromFile("image/VFX_Shot3.png", tanksTexSize);
    texVFXShot4.loadFromFile("image/VFX_Shot4.png", tanksTexSize);
    texVFXShot5.loadFromFile("image/VFX_Shot5.png", tanksTexSize);
    texVFXShot6.loadFromFile("image/VFX_Shot6.png", tanksTexSize);
}
void TextureManager::ParticleTextureInit()
{
    texParticleDebris.loadFromFile("image/Particle_Debris.png", tanksTexSize);
    texParticleDust.loadFromFile("image/Particle_Dust.png", tanksTexSize);
    texParticleSparks.loadFromFile("image/Particle_Sparks.png", tanksTexSize);
}
void TextureManager::MaskTextureInit()
{
    texMaskNoise.loadFromFile("image/Mask_Noise.png", tanksTexSize);
    texMaskRadial.loadFromFile("image/Mask_Radial.png", tanksTexSize);
    texMaskRing.loadFromFile("image/Mask_Ring.png", tanksTexSize);
}
void TextureManager::TerrainTextureInit()
{
    texSoilComposite.loadFromFile("image/Terrain_Soil.png", tanksTexSize); // temp
    texSoilBase.loadFromFile("image/Terrain_Soil_Base.png", tanksTexSize);
    texSoilRock.loadFromFile("image/Terrain_Soil_Rock.ong", tanksTexSize);
    texSoilRoot.loadFromFile("image/Terrain_Soil_Root.png", tanksTexSize);
    texSoilPebble.loadFromFile("image/Terrain_Soil_Pebble.png", tanksTexSize);
    texSoilGrass.loadFromFile("image/Terrain_Soil_Grass.png", tanksTexSize);
    texSoilFlower.loadFromFile("image/Terrain_Soil_Flower.png", tanksTexSize);
    texSnowBase.loadFromFile("image/Terrain_Snow_Base.png", tanksTexSize);
    texSnowCracks.loadFromFile("image/Terrain_Snow_Cracks.png", tanksTexSize);
    texSnowGrass.loadFromFile("image/Terrain_Snow_Grass.png", tanksTexSize);
    texSnowIce.loadFromFile("image/Terrain_Snow_Ice.png", tanksTexSize);
    texSandBase.loadFromFile("image/Terrain_Sand_Base.png", tanksTexSize);
    texSandWaves.loadFromFile("image/Terrain_Sand_Waves.png", tanksTexSize);
    texSandRocks.loadFromFile("image/Terrain_Sand_Rocks.png", tanksTexSize);
    texSandGrass.loadFromFile("image/Terrain_Sand_Grass.png", tanksTexSize);
    texMeadowBase.loadFromFile("image/Terrain_Meadow_Base.png", tanksTexSize);
    texMeadowRoots.loadFromFile("image/Terrain_Meadow_Roots.png", tanksTexSize);
    texMeadowGrass.loadFromFile("image/Terrain_Meadow_Grass.png", tanksTexSize);
    texMeadowFlowers.loadFromFile("image/Terrain_Meadow_Flowers.png", tanksTexSize);
}
void TextureManager::ObjectTextureInit()
{
    texObjectBush.loadFromFile("image/Object_Bush.png", tanksTexSize);
    texObjectRock.loadFromFile("image/Object_Rock.png", tanksTexSize);
}
void TextureManager::UITextureInit()
{
    texUIElement.loadFromFile("image/UI_Element.png", tanksTexSize);
}

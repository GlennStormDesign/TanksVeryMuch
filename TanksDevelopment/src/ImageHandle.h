#pragma once

// Image Handling

sf::IntRect tanksTexSize = sf::IntRect(0, 0, 32, 32);

class TextureManager {
public:
    sf::Texture texTankBase, texTankTurret;
    sf::Texture texTurretShot1, texTurretShot2, texTurretShot3, texTurretShot4;
    sf::Texture texVFXShot1, texVFXShot2, texVFXShot3, texVFXShot4, texVFXShot5, texVFXShot6;
    sf::Texture texParticleDebris, texParticleDust, texParticleSparks;
    sf::Texture texMaskNoise, texMaskRadial, texMaskRing;
    sf::Texture texSoilComposite;
    sf::Texture texSoilBase, texSoilRock, texSoilRoot, texSoilPebble, texSoilGrass, texSoilFlower;
    sf::Texture texSnowBase, texSnowCracks, texSnowGrass, texSnowIce;
    sf::Texture texSandBase, texSandWaves, texSandRocks, texSandGrass;
    sf::Texture texMeadowBase, texMeadowRoots, texMeadowGrass, texMeadowFlowers;
    sf::Texture texUIElement;

    sf::Sprite spPlayerBase, spPlayerTurret;
    sf::Sprite spVFXShot;
    sf::Sprite spTerrain;
private:
public:
    TextureManager() { TankTextureInit(); VFXTextureInit(); ParticleTextureInit(); MaskTextureInit(); TerrainTextureInit(); UITextureInit(); }
    ~TextureManager() { }
private:
    void TankTextureInit()
    {
        texTankBase.loadFromFile("image/TankBase_Temp.png", tanksTexSize);
        texTankTurret.loadFromFile("image/TankTurret_Temp.png", tanksTexSize);
        texTurretShot1.loadFromFile("image/TankTurret_Temp_Shot1.png", tanksTexSize);
        texTurretShot2.loadFromFile("image/TankTurret_Temp_Shot2.png", tanksTexSize);
        texTurretShot3.loadFromFile("image/TankTurret_Temp_Shot3.png", tanksTexSize);
        texTurretShot4.loadFromFile("image/TankTurret_Temp_Shot4.png", tanksTexSize);
    }
    void VFXTextureInit()
    {
        texVFXShot1.loadFromFile("image/VFX_Shot1.png", tanksTexSize);
        texVFXShot2.loadFromFile("image/VFX_Shot2.png", tanksTexSize);
        texVFXShot3.loadFromFile("image/VFX_Shot3.png", tanksTexSize);
        texVFXShot4.loadFromFile("image/VFX_Shot4.png", tanksTexSize);
        texVFXShot5.loadFromFile("image/VFX_Shot5.png", tanksTexSize);
        texVFXShot6.loadFromFile("image/VFX_Shot6.png", tanksTexSize);
    }
    void ParticleTextureInit()
    {
        texParticleDebris.loadFromFile("image/Particle_Debris.png", tanksTexSize);
        texParticleDust.loadFromFile("image/Particle_Dust.png", tanksTexSize);
        texParticleSparks.loadFromFile("image/Particle_Sparks.png", tanksTexSize);
    }
    void MaskTextureInit()
    {
        texMaskNoise.loadFromFile("image/Mask_Noise.png", tanksTexSize);
        texMaskRadial.loadFromFile("image/Mask_Radial.png", tanksTexSize);
        texMaskRing.loadFromFile("image/Mask_Ring.png", tanksTexSize);
    }
    void TerrainTextureInit()
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
    void UITextureInit()
    {
        texUIElement.loadFromFile("image/UI_Element.png", tanksTexSize);
    }
};

static TextureManager texMgr;

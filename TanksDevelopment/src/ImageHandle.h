#pragma once

// Image Declarations

static sf::IntRect tanksTexSize = sf::IntRect(0, 0, 32, 32);

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
    void TankTextureInit();
    void VFXTextureInit();
    void ParticleTextureInit();
    void MaskTextureInit();
    void TerrainTextureInit();
    void UITextureInit();
};

static TextureManager texMgr;

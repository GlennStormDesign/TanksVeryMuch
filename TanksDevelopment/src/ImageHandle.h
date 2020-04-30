#pragma once

// Image Declarations

static const sf::IntRect TANKS_TEX_SIZE = sf::IntRect(0, 0, 32, 32);

extern const sf::Texture& TexTankBase(); // tank base
extern const sf::Texture& TexTankTurret(); // tank turret
extern const sf::Texture& TexTurretShot1(); // turret shot
extern const sf::Texture& TexTurretShot2();
extern const sf::Texture& TexTurretShot3();
extern const sf::Texture& TexTurretShot4();
extern const sf::Texture& TexVFXShot1(); // vfx shot
extern const sf::Texture& TexVFXShot2();
extern const sf::Texture& TexVFXShot3();
extern const sf::Texture& TexVFXShot4();
extern const sf::Texture& TexVFXShot5();
extern const sf::Texture& TexVFXShot6();
extern const sf::Texture& TexParticleDebris(); // particle
extern const sf::Texture& TexParticleDust();
extern const sf::Texture& TexParticleSparks();
extern const sf::Texture& TexMaskNoise(); // mask
extern const sf::Texture& TexMaskRadial();
extern const sf::Texture& TexMaskRing();
extern const sf::Texture& TexSoilBase(); // terrain soil
extern const sf::Texture& TexSoilRock();
extern const sf::Texture& TexSoilRoot();
extern const sf::Texture& TexSoilPebble();
extern const sf::Texture& TexSoilGrass();
extern const sf::Texture& TexSoilFlower();
extern const sf::Texture& TexSnowBase(); // terrain snow
extern const sf::Texture& TexSnowCracks();
extern const sf::Texture& TexSnowGrass();
extern const sf::Texture& TexSnowIce();
extern const sf::Texture& TexSandBase(); // terrain sand
extern const sf::Texture& TexSandWaves();
extern const sf::Texture& TexSandRocks();
extern const sf::Texture& TexSandGrass();
extern const sf::Texture& TexMeadowBase(); // terrain meadow
extern const sf::Texture& TexMeadowRoots();
extern const sf::Texture& TexMeadowGrass();
extern const sf::Texture& TexMeadowFlowers();
extern const sf::Texture& TexObjectBush(); // scene object
extern const sf::Texture& TexObjectRock();
extern const sf::Texture& TexUIElement(); // ui

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
    sf::Texture texObjectBush, texObjectRock;
    sf::Texture texUIElement;
private:
public:
    TextureManager() {
        TankTextureInit();
        VFXTextureInit();
        ParticleTextureInit();
        MaskTextureInit();
        TerrainTextureInit();
        ObjectTextureInit();
        UITextureInit(); }
    ~TextureManager() { }
private:
    void TankTextureInit();
    void VFXTextureInit();
    void ParticleTextureInit();
    void MaskTextureInit();
    void TerrainTextureInit();
    void ObjectTextureInit();
    void UITextureInit();
};

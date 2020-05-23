#pragma once

#include "TankCore.h"
#include "ImageHandle.h"

// Terrain Declarations

const sf::Color DEFAULT_DUST_COLOR = sf::Color( 160, 150, 140, 200 );

struct TerrainLayer {
    sf::Image       image; // instead of storing sf::Texture, using sf::Image to preserve alpha
    sf::Vector2u    noiseOffset = sf::Vector2u( 0, 0 );
};

class TerrainSubstance {
public:
protected:
    std::vector<TerrainLayer>   m_layers;
    int                         m_maxLayers = 0;
    sf::Color                   m_dustColor = DEFAULT_DUST_COLOR;
public:
    TerrainSubstance() { }
    ~TerrainSubstance() { }

    virtual void TerrainInit() { /* set parameters for individual terrain in subclass */ }

    void SubstanceInit();

    int GetMaxLayers() { return m_maxLayers; }
    void SetMaxLayers( const unsigned int& maxLayers ) { m_maxLayers = maxLayers; }
    sf::Color GetDustColor() { return m_dustColor; }
    void SetDustColor( const sf::Color& color ) { m_dustColor = color; }
    TerrainLayer GetLayer( const unsigned int& layerIndex ) { return m_layers[layerIndex]; }
    void ClearLayers();
protected:
    void AddLayer( const sf::Texture& tex );
};

class TerrainManager {
public:
private:
    TerrainSubstance m_substance;
    sf::RenderTexture m_renderTexture;
    sf::Sprite m_sprite;
    sf::Uint8* m_noiseMap = new sf::Uint8[1024]; // 32x32 alpha values 0-255
    sf::Shader m_tileBlendShader;
    bool m_shaderLoaded = false;
    float m_vwRangeX = 1024.f * globalScale; // REVIEW: associate with # of tiles and resolution of tile
    float m_vwRangeY = 1024.f * globalScale;
    sf::Vector2f m_viewPos = sf::Vector2f(0.f,0.f);
    sf::Vector2f m_viewOffset = sf::Vector2f(0.f,0.f);
    float m_drawThreshold = 1.25f; // >0.5, <1.f for testing, REVIEW: 1.25 is change (check not centered in 1024 * globalScale)
public:
    TerrainManager() { Init(); }
    ~TerrainManager() { }

    void Init();
    void SetViewOffset( const sf::Vector2f& vwOffset );

    void DrawTerrain( sf::RenderWindow& window, const sf::Vector2f& viewPos );

    TerrainSubstance GetTerrain();
    void SetTerrain( const TerrainSubstance& terrain );
private:
    const sf::Uint8 GetNoiseInt( const int& x, const int& y );
    const float GetNoiseFloat( const int& x, const int& y );
    const float GetNoiseSmoothed( const float& baseVal, const int& x, const int& y );
    void ComposeTerrain();
};

class SubstanceSoil : public TerrainSubstance {
public:
    SubstanceSoil() { TerrainSubstance(); SubstanceInit(); }
    void TerrainInit() override
    {
        SetMaxLayers( 6 );
        // base (soil), rocks, roots, pebble, grass, flowers
        AddLayer( TexSoilBase() );
        AddLayer( TexSoilRock() );
        AddLayer( TexSoilRoot() );
        AddLayer( TexSoilPebble() );
        AddLayer( TexSoilGrass() );
        AddLayer( TexSoilFlower() );
        SetDustColor( DEFAULT_DUST_COLOR );
    }
};

class SubstanceSnow : public TerrainSubstance {
public:
    SubstanceSnow() { TerrainSubstance(); SubstanceInit(); }
    void TerrainInit() override
    {
        SetMaxLayers( 4 );
        // base (snow), cracks, grass, ice
        AddLayer( TexSnowBase() );
        AddLayer( TexSnowCracks() );
        AddLayer( TexSnowGrass() );
        AddLayer( TexSnowIce() );
        SetDustColor( sf::Color( 220, 240, 255, 200 ) );
    }
};

class SubstanceSand : public TerrainSubstance {
public:
    SubstanceSand() { TerrainSubstance(); SubstanceInit(); }
    void TerrainInit() override
    {
        SetMaxLayers( 4 );
        // base (sand), waves, rocks, grass
        AddLayer( TexSandBase() );
        AddLayer( TexSandWaves() );
        AddLayer( TexSandRocks() );
        AddLayer( TexSandGrass() );
        SetDustColor( sf::Color( 220, 210, 140, 200 ) );
    }
};

class SubstanceMeadow : public TerrainSubstance {
public:
    SubstanceMeadow() { TerrainSubstance(); SubstanceInit(); }
    void TerrainInit() override
    {
        SetMaxLayers( 4 );
        // base (sod), roots, grass, flowers
        AddLayer( TexMeadowBase() );
        AddLayer( TexMeadowRoots() );
        AddLayer( TexMeadowGrass() );
        AddLayer( TexMeadowFlowers() );
        SetDustColor( sf::Color( DEFAULT_DUST_COLOR ) );
    }
};

extern void SetTerrainViewOffset( const sf::Vector2f& vwOffset );
extern void DrawTerrain( sf::RenderWindow& window, const sf::Vector2f& viewPos );
extern TerrainSubstance GetTerrain();
extern void SetTerrain( const TerrainSubstance& terrain );

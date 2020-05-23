#include <cmath>

#include <SFML/Graphics.hpp>

#include "Terrain.h"

// Terrain Definitions

static TerrainManager terrainMgr;

// TerrainManager interface

extern void SetTerrainViewOffset( const sf::Vector2f& vwOffset )
{
    terrainMgr.SetViewOffset( vwOffset );
}
extern void DrawTerrain( sf::RenderWindow& window, const sf::Vector2f& viewPos )
{
    terrainMgr.DrawTerrain( window, viewPos );
}
extern TerrainSubstance GetTerrain()
{
    return terrainMgr.GetTerrain();
}
extern void SetTerrain( const TerrainSubstance& terrain )
{
    terrainMgr.SetTerrain( terrain );
}

// TerrainSubstance implementation

void TerrainSubstance::SubstanceInit()
{
    // general values
    TerrainInit(); // specific values
    // configuration
}

void TerrainSubstance::AddLayer( const sf::Texture& tex )
{
    TerrainLayer t;
    t.image = tex.copyToImage();
    t.noiseOffset = sf::Vector2u( (rand()%32),(rand()%32) );
    m_layers.push_back( t );
    // cleanup
    delete t.image.getPixelsPtr();
}

void TerrainSubstance::ClearLayers()
{
    m_layers.clear();
}

// TerrainManager implementation

void TerrainManager::Init()
{
    int index = 0;
    for ( int y=0; y<32; y++ )
    {
        for ( int x=0; x<32; x++ )
        {
            // REVIEW: cannot copyToImage() from reference (or copy of Texture) returned by TexMaskNoise()
            // m_noiseMap[(index++)] = TexMaskNoise().copyToImage().getPixel(x,y).a;
            sf::Texture noiseTemp;
            noiseTemp.loadFromFile("image/Mask_Noise.png", TANKS_TEX_SIZE);
            m_noiseMap[(index++)] = noiseTemp.copyToImage().getPixel(x,y).a;
        }
    }
    m_shaderLoaded = m_tileBlendShader.loadFromFile( "src/TerrainTileBlend.glsl", sf::Shader::Fragment );
    sf::ContextSettings settings;
    settings.depthBits = 8;
    m_renderTexture.create(32*32*globalScale, 32*32*globalScale, settings);
    m_sprite.setTextureRect( sf::IntRect(0, 0, 32*32*globalScale, 32*32*globalScale) );
    m_sprite.setOrigin(0.f, 0.f);
    m_sprite.setPosition(0.f, 0.f);
}
void TerrainManager::SetViewOffset( const sf::Vector2f& vwOffset ) { m_viewOffset = vwOffset; } // set prior to main loop

void TerrainManager::DrawTerrain( sf::RenderWindow& window, const sf::Vector2f& viewPos )
{
    m_viewPos = viewPos;
    sf::Vector2f checkCenter = ( m_viewPos + m_viewOffset );
    // set check center to closest absolute increment of range (uses cmath)
    checkCenter.x = m_vwRangeX * ( round( checkCenter.x/m_vwRangeX ) );
    checkCenter.y = m_vwRangeY * ( round( checkCenter.y/m_vwRangeY ) );
    for ( int i=(checkCenter.x-m_vwRangeX); i<=(checkCenter.x+m_vwRangeX); i+=(m_vwRangeX) )
    {
        for ( int n=(checkCenter.y-m_vwRangeY); n<=(checkCenter.y+m_vwRangeY); n+=(m_vwRangeY) )
        {
            if ( abs( i - ( m_viewPos.x + m_viewOffset.x ) ) <= (m_vwRangeX * m_drawThreshold) )
            {
                if ( abs( n - ( m_viewPos.y + m_viewOffset.y ) ) <= (m_vwRangeY * m_drawThreshold) )
                {
                    m_sprite.setPosition( i, n );
                    window.draw( m_sprite );
                }
            }
        }
    }
}

const TerrainSubstance& TerrainManager::GetTerrain() { return m_substance; }
void TerrainManager::SetTerrain( const TerrainSubstance& terrain ) { m_substance = terrain; ComposeTerrain(); }

const sf::Uint8& TerrainManager::GetNoiseInt( const int& x, const int& y )
{
    return ( m_noiseMap[( ( ( y % 32 ) * 32 ) + ( x % 32 ) )] );
}
const float TerrainManager::GetNoiseFloat( const int& x, const int& y )
{
    return ( GetNoiseInt(x,y) / 255.f );
}
const float TerrainManager::GetNoiseSmoothed( const float& baseVal, const int& x, const int& y )
{
    return ( ( baseVal + GetNoiseFloat(x,y) ) / 2.f );
}
void TerrainManager::ComposeTerrain()
{
    m_renderTexture.clear();
    sf::Texture terrainLayer;
    sf::Vector2u layerOffset = sf::Vector2u(0,0);
    sf::Sprite layerSprite;
    // treat layer 0 (base layer) as a single 1024x1024 repeated tile
    terrainLayer.loadFromImage(m_substance.GetLayer(0).image);
    terrainLayer.setRepeated(true);
    terrainLayer.setSmooth(false);
    layerSprite.setTextureRect(sf::IntRect(0,0,32*32*globalScale,32*32*globalScale));
    layerSprite.setTexture(terrainLayer);
    // draw base layer
    m_renderTexture.draw(layerSprite);
    // cleanup texture pointer
    delete layerSprite.getTexture();
    // compose terrain layers by re-using texture and sprite as 32x32 non-repeated tiles
    layerSprite.setTexture(terrainLayer);
    layerSprite.setTextureRect(sf::IntRect(0,0,32,32));
    layerSprite.setOrigin(0.f, 0.f);
    layerSprite.setScale(globalScale, globalScale);
    layerSprite.setPosition(0.f, 0.f);
    for ( int l=1; l<m_substance.GetMaxLayers(); l++ )
    {
        terrainLayer.loadFromImage(m_substance.GetLayer(l).image); // image preserves alpha
        layerOffset = m_substance.GetLayer(l).noiseOffset;
        for ( int n=0; n<32; n++ )
        {
            for ( int i=0; i<32; i++ )
            {
                layerSprite.setPosition( (i * 32 * globalScale), (n * 32 * globalScale) );
                sf::Color pixelAlpha = sf::Color::White;
                int getX, getY;
                getX = ( ( i + layerOffset.x ) % 32 );
                getY = ( ( n + layerOffset.y ) % 32 );
                pixelAlpha.a = GetNoiseInt( getX, getY );
                // blend if shader loaded
                if ( sf::Shader::isAvailable() && m_shaderLoaded )
                {
                    // blending across tile to surrounding eight mask pixel alpha values
                    // 1 2 3
                    // 4 X 5
                    // 6 7 8
                    // configure and utilize tileBlend shader
                    m_tileBlendShader.setUniform("tSample", terrainLayer);
                    float centerA = GetNoiseFloat( getX, getY );
                    float topleftA = GetNoiseSmoothed( centerA, (getX-1), (getY-1) );
                    float topA = GetNoiseSmoothed( centerA, getX, (getY-1) );
                    float toprightA = GetNoiseSmoothed( centerA, (getX+1), (getY-1) );
                    float leftA = GetNoiseSmoothed( centerA, (getX-1), getY );
                    float rightA = GetNoiseSmoothed( centerA, (getX+1), getY );
                    float bottomleftA = GetNoiseSmoothed( centerA, (getX-1), (getY+1) );
                    float bottomA = GetNoiseSmoothed( centerA, getX, (getY+1) );
                    float bottomrightA = GetNoiseSmoothed( centerA, (getX+1), (getY+1) );
                    m_tileBlendShader.setUniform("tileAlphaCenter", centerA);
                    m_tileBlendShader.setUniform("tileAlphaTopLeft", topleftA);
                    m_tileBlendShader.setUniform("tileAlphaTop", topA);
                    m_tileBlendShader.setUniform("tileAlphaTopRight", toprightA);
                    m_tileBlendShader.setUniform("tileAlphaLeft", leftA);
                    m_tileBlendShader.setUniform("tileAlphaRight", rightA);
                    m_tileBlendShader.setUniform("tileAlphaBottomLeft", bottomleftA);
                    m_tileBlendShader.setUniform("tileAlphaBottom", bottomA);
                    m_tileBlendShader.setUniform("tileAlphaBottomRight", bottomrightA);
                    m_renderTexture.draw( layerSprite, &m_tileBlendShader );
                }
                else
                {
                    // tile blend shader not found, just draw tile with tile center alpha value
                    layerSprite.setColor( pixelAlpha );
                    m_renderTexture.draw( layerSprite );
                }
            }
        }
        // cleanup texture pointer
        delete layerSprite.getTexture();
    }
    // finalize terrain sprite
    m_renderTexture.display();
    m_sprite.setTexture( m_renderTexture.getTexture() );
}

// TerrainSubstance classes defined in header

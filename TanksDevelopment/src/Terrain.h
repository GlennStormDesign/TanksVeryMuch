#pragma once

// Terrain Handling

const sf::Color DEFAULT_DUST_COLOR = sf::Color( 160, 150, 140, 200 );

struct TerrainLayer {
    sf::Image       image; // instead of storying sf::Texture, using sf::Image to preserve alpha
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

    void SubstanceInit()
    {
        // general values
        TerrainInit(); // specific values
        // configuration
    }

    int GetMaxLayers() { return m_maxLayers; }
    void SetMaxLayers( const unsigned int& maxLayers ) { m_maxLayers = maxLayers; }
    sf::Color GetDustColor() { return m_dustColor; }
    void SetDustColor( const sf::Color& color ) { m_dustColor = color; }
    TerrainLayer GetLayer( const unsigned int& layerIndex ) { return m_layers[layerIndex]; }
protected:
    void AddLayer( const sf::Texture& tex )
    {
        TerrainLayer t;
        t.image = tex.copyToImage();
        t.noiseOffset = sf::Vector2u( (rand()%32),(rand()%32) );
        m_layers.push_back( t );
    }
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
    // TODO: handle view zooming functionality with view range values
    float m_vwRangeX = 1024.f * globalScale; // TODO: associate with # of tiles and resolution of tile
    float m_vwRangeY = 1024.f * globalScale;
    sf::Vector2f m_viewPos = sf::Vector2f(0.f,0.f);
    sf::Vector2f m_viewOffset = sf::Vector2f(0.f,0.f);
    float m_drawThreshold = 1.0f; // >0.5, <1.f for testing
public:
    TerrainManager() { Init(); }
    ~TerrainManager() { }

    void Init()
    {
        int index = 0;
        for ( int y=0; y<32; y++ )
        {
            for ( int x=0; x<32; x++ )
            {
                m_noiseMap[(index++)] = texMgr.texMaskNoise.copyToImage().getPixel(x,y).a;
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
    void SetViewOffset( const sf::Vector2f& vwOffset ) { m_viewOffset = vwOffset; } // set prior to main loop

    void DrawTerrain( sf::RenderWindow& window, const sf::Vector2f& viewPos )
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

    TerrainSubstance GetTerrain() { return m_substance; }
    void SetTerrain( const TerrainSubstance& terrain ) { m_substance = terrain; ComposeTerrain(); }
private:
    const sf::Uint8 GetNoiseInt( const int& x, const int& y )
    {
        return ( m_noiseMap[( ( ( y % 32 ) * 32 ) + ( x % 32 ) )] );
    }
    const float GetNoiseFloat( const int& x, const int& y )
    {
        return ( GetNoiseInt(x,y) / 255.f );
    }
    const float GetNoiseSmoothed( const float& baseVal, const int& x, const int& y )
    {
        return ( ( baseVal + GetNoiseFloat(x,y) ) / 2.f );
    }
    void ComposeTerrain()
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
        }
        // finalize terrain sprite
        m_renderTexture.display();
        m_sprite.setTexture( m_renderTexture.getTexture() );
    }
};

class SubstanceSoil : public TerrainSubstance {
public:
    SubstanceSoil() { TerrainSubstance(); SubstanceInit(); }
    void TerrainInit() override
    {
        SetMaxLayers( 6 );
        // base (dirt), rock, root, pebble, grass, flower
        AddLayer( texMgr.texSoilBase );
        AddLayer( texMgr.texSoilRock );
        AddLayer( texMgr.texSoilRoot);
        AddLayer( texMgr.texSoilPebble );
        AddLayer( texMgr.texSoilGrass );
        AddLayer( texMgr.texSoilFlower );
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
        AddLayer( texMgr.texSnowBase );
        AddLayer( texMgr.texSnowCracks );
        AddLayer( texMgr.texSnowGrass );
        AddLayer( texMgr.texSnowIce );
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
        AddLayer( texMgr.texSandBase );
        AddLayer( texMgr.texSandWaves );
        AddLayer( texMgr.texSandRocks );
        AddLayer( texMgr.texSandGrass );
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
        AddLayer( texMgr.texMeadowBase );
        AddLayer( texMgr.texMeadowRoots );
        AddLayer( texMgr.texMeadowGrass );
        AddLayer( texMgr.texMeadowFlowers );
        SetDustColor( sf::Color( DEFAULT_DUST_COLOR ) );
    }
};

static TerrainManager terrainMgr;

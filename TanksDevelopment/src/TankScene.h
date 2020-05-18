#pragma once

#include <string>

#include "AudioHandle.h"
#include "UIHandle.h"
#include "Tank.h"
#include "Terrain.h"
#include "SceneObject.h"

// TankScene Declarations

// Tank Scenes

// handle scene stats, handle player stats

// primary job of scene is to take over updating and rendering of scene elements
//  x terrain
//  x tanks
//  x shots
//  x vfx
//  ~ decoration / animated deco
//  x obstacles
//  ~ destructables
// which should allow coordination of layers, batching, etc

// secondary job of scene is to manage collisions between tanks and tanks, and shots and tanks
// to do this, the scene needs a pool of tanks to work with
// as tanks are added to the scene, they are added to the pool
// if removed, they need to be removed from the pool
// then, the scene can be updated each tick for potential collisions to manage

// can also manage tank collision with obstacles, destructables and triggers (scene objects in an object pool)

struct SceneStats {
    bool isSceneActive = false;
    float sceneActiveTime = 0.f;
    unsigned int maxPlayers = 0;
    unsigned int currentPlayers = 0;
};

struct PlayerStats {
    unsigned int playerTankID = 0;
    std::string playerName = "PLAYER";
    bool isPlayerActive = false;
    float playerActiveTime = 0.f;
    unsigned int tankKills = 0;
    unsigned int shotsFired = 0;
    unsigned int timesRespawned = 0;
};

// used for matching collide-able references in remove functions
bool operator== ( const Tank& l, const Tank& r );
bool operator== ( const SceneObject& l, const SceneObject& r );
bool operator== ( const PlayerStats& l, const PlayerStats& r );

enum SceneType { Sandbox, Cinematic, Campaign, FreeBattle, NetBattle };

class TankScene {
public:
    SceneStats stats;
private:
protected:
    SceneType m_type = Sandbox;
    TerrainSubstance m_terrain;
    std::vector<Tank> m_tankPool;
    unsigned int m_tankIndex = 0;
    std::vector<SceneObject*> m_objectPool; // vector of pointers to accommodate various subclass objects
    unsigned int m_objIndex = 0;
    std::vector<PlayerStats> m_playerPool;
    unsigned int m_playerIndex = 0;
public:
    TankScene() { }
    virtual ~TankScene() { } // remove all in tank pool and shot pool

    virtual void LevelInit() { } // level configuration in subclasses

    void SceneInit();

    void UnloadScene();
    void LoadScene( TankScene* level );

    SceneType& GetSceneType();
    void SetSceneType( const SceneType& type );

    void AddTank( Tank t );
    void AddTank( Tank t, sf::Color c );
    void RemoveTank( Tank& t );
    Tank& GetTank( const unsigned int& index );
    Tank& GetLocalPlayerTank();
    unsigned int GetActiveTankCount();
    int GetTotalTankCount();
    void AddObject( SceneObject o );
    void RemoveObject( const SceneObject& o );
    void SetObject( const unsigned int& index, const SceneObject& o );
    const SceneObject& GetObject( const unsigned int& index );
    void AddPlayer( PlayerStats p );
    void RemovePlayer( const PlayerStats p );
    PlayerStats& GetPlayer( const unsigned int& index );
    PlayerStats& GetLocalPlayer();

    void UpdateScene( const float& timeDelta );

    void DrawScene( sf::RenderWindow& window );
private:
    virtual void UpdateLevel( const float& timeDelta ) { }
};

extern void NewScene( TankScene* level );
extern void UnloadScene();
extern void LoadScene( TankScene* level );
extern bool GetSceneActive();
extern void SetSceneActive( const bool& active );
extern SceneType& GetSceneType();
extern void SetSceneType( const SceneType& type );
extern void AddTank( Tank t );
extern void AddTank( Tank t, sf::Color c );
extern void RemoveTank( Tank& t );
extern Tank& GetTank( const unsigned int& index );
extern Tank& GetLocalPlayerTank();
extern unsigned int GetActiveTankCount();
extern int GetTotalTankCount();
extern void AddObject( SceneObject o );
extern void RemoveObject( const SceneObject& o );
extern void SetObject( const unsigned int& index, const SceneObject& o );
extern const SceneObject& GetObject( const unsigned int& index );
extern void AddPlayer( PlayerStats p );
extern void RemovePlayer( const PlayerStats p );
extern PlayerStats& GetPlayer( const unsigned int& index );
extern PlayerStats& GetLocalPlayer();
extern void UpdateScene( const float& timeDelta );
extern void DrawScene( sf::RenderWindow& window );

// level definitions

class TestTankScene : public TankScene {
public:
    TestTankScene() { SceneInit(); }
    void LevelInit() override
    {
        // level specific config
        m_type = Sandbox;
        // tanks
        m_tankPool.reserve(4);
        Tank tempTank = Tank( LocalPlayer, 512.f, 512.f, 0.f, 1.f );
        AddTank( tempTank, DEF_TANK_COLOR );
        tempTank = Tank( Drone, 640.f, 320.f, 0.f, 1.f );
        AddTank( tempTank, sf::Color(128.f, 16.f, 32.f, 255.f) );
        tempTank = Tank( Drone, 420.f, 420.f, 180.f, 1.f );
        AddTank( tempTank, sf::Color(200.f,200.f,32.f,255.f) );
        tempTank = Tank( Drone, 420.f, 580.f, 180.f, 1.f );
        AddTank( tempTank, sf::Color(16.f,32.f,200.f,255.f) );
        // terrain
        m_terrain = SubstanceSoil();
        SetTerrainViewOffset( -(GetLocalPlayerTank().GetBaseSprite().getPosition()) );

        // scene
        m_objectPool.reserve(3);

        SceneDecoration tempDeco;
        sf::Sprite tmpSprite;
        tmpSprite.setTexture( TexObjectBush() );
        tmpSprite.setScale( globalScale, globalScale );
        tmpSprite.setOrigin( 16.f, 16.f );
        tempDeco.SetSprite( tmpSprite );
        tempDeco.SetObjPos( sf::Vector2f( 512.f, 600.f ) );
        tempDeco.SetObjectID( m_objIndex++ );
        m_objectPool.push_back( tempDeco.clone() );
        //AddObject( tempDeco );

        SceneObstacle tempObstacle;
        tmpSprite.setTexture( TexObjectRock() );
        tempObstacle.SetSprite( tmpSprite );
        tempObstacle.SetHitBox( GetHitBox( tmpSprite, 0.618f ) );
        tempObstacle.SetObjPos( sf::Vector2f( 512.f, 400.f ) );
        tempObstacle.SetObjectID( m_objIndex++ );
        m_objectPool.push_back( tempObstacle.clone() );
        //AddObject( tempObstacle );

        SceneTrigger tempTrigger;
        tempTrigger.SetObjPos( sf::Vector2f( 0.f, 0.f ) );
        tmpSprite.setTexture( TexMaskRadial() );
        tempTrigger.SetSprite( tmpSprite );
        tempTrigger.SetObjectID( m_objIndex++ );
        m_objectPool.push_back( tempTrigger.clone() );

        // scene setup
        stats.maxPlayers = 1;
    }

    void UpdateLevel( const float& timeDelta ) override;
};

class TutorialGameScene : public TankScene {
public:
    TutorialGameScene() { SceneInit(); }
    void LevelInit() override
    {
        // level specific config
        m_type = Campaign;
        // tanks
        m_tankPool.reserve(2);
        Tank tempTank = Tank( LocalPlayer, 512.f, 512.f, 0.f, 1.f );
        AddTank( tempTank, DEF_TANK_COLOR );
        tempTank = Tank( Drone, -100.f, -100.f, 0.f, 1.f );
        AddTank( tempTank, sf::Color(128.f, 16.f, 32.f, 255.f) );
        // terrain
        m_terrain = SubstanceMeadow();
        SetTerrainViewOffset( -(GetLocalPlayerTank().GetBaseSprite().getPosition()) );

        // scene
        m_objectPool.reserve(3);

        SceneDecoration tempDeco;
        sf::Sprite tmpSprite;
        tmpSprite.setTexture( TexObjectBush() );
        tmpSprite.setScale( globalScale, globalScale );
        tmpSprite.setOrigin( 16.f, 16.f );
        tempDeco.SetSprite( tmpSprite );
        tempDeco.SetObjPos( sf::Vector2f( 512.f, 600.f ) );
        tempDeco.SetObjectID( m_objIndex++ );
        m_objectPool.push_back( tempDeco.clone() );
        //AddObject( tempDeco );

        SceneObstacle tempObstacle;
        tmpSprite.setTexture( TexObjectRock() );
        tempObstacle.SetSprite( tmpSprite );
        tempObstacle.SetHitBox( GetHitBox( tmpSprite, 0.618f ) );
        tempObstacle.SetObjPos( sf::Vector2f( 512.f, 400.f ) );
        tempObstacle.SetObjectID( m_objIndex++ );
        m_objectPool.push_back( tempObstacle.clone() );
        //AddObject( tempObstacle );

        SceneTrigger tempTrigger;
        tempTrigger.SetObjPos( sf::Vector2f( 0.f, 0.f ) );
        tmpSprite.setTexture( TexMaskRadial() );
        tempTrigger.SetSprite( tmpSprite );
        tempTrigger.SetObjectID( m_objIndex++ );
        m_objectPool.push_back( tempTrigger.clone() );

        // scene setup
        stats.maxPlayers = 1;
    }

    void UpdateLevel( const float& timeDelta ) override;
};

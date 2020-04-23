#pragma once

#include <string>

#include "AudioHandle.h"
#include "Tank.h"
#include "Terrain.h"
#include "SceneObject.h"

// TankScene Declarations

// Tank Scenes

// handle scene stats, handle player stats

// primary job of scene is to take over updating and rendering of scene elements
// which should allow coordination of layers, batching, etc
//  x terrain
//  . tanks
//  . shots
//  . vfx
//  decoration / animated deco
//  obstacles
//  destructables

// secondary job of scene is to manage collisions between tanks and tanks, and shots and tanks
// to do this, the scene needs a pool of tanks and a pool of shots to work with
// as tanks and shots are added to the scene, they are added to the pools
// if removed, they need to be removed from the pools
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
bool operator== ( const TankShot& l, const TankShot& r );
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
    std::vector<TankShot> m_shotPool;
    unsigned int m_shotIndex = 0;
    std::vector<SceneObject> m_objectPool;
    unsigned int m_objIndex = 0;
    std::vector<PlayerStats> m_playerPool;
    unsigned int m_playerIndex = 0;
public:
    TankScene() { /*SceneInit();*/ } // REVIEW: unlike ParticleEmitter class, this constructor does not call SceneInit()?
    ~TankScene() { /* remove all in tank pool and shot pool */ }

    virtual void LevelInit() { /* level configuration in subclasses */ }

    void SceneInit();

    void UnloadScene();
    void LoadScene( const TankScene& level );

    SceneType& GetSceneType();
    void SetSceneType( const SceneType& type );

    void AddTank( Tank& t );
    void RemoveTank( Tank& t );
    Tank& GetTank( const unsigned int& index );
    Tank& GetLocalPlayerTank();
    unsigned int GetActiveTankCount();
    void AddShot( TankShot& s );
    void RemoveShot( const TankShot& s );
    TankShot& GetShot( const unsigned int& index );
    void AddObject( SceneObject& o );
    void RemoveObject( const SceneObject& o );
    SceneObject& GetObject( const unsigned int& index );
    void AddPlayer( PlayerStats& p, const unsigned int& tankID );
    void RemovePlayer( const PlayerStats p );
    PlayerStats& GetPlayer( const unsigned int& index );
    PlayerStats& GetLocalPlayer();

    void UpdateScene( const float& timeDelta );

    void DrawScene( sf::RenderWindow& window, const sf::Vector2f& viewPos );
private:
};

class TestTankScene : public TankScene {
public:
    TestTankScene() { TankScene(); SceneInit(); }
    void LevelInit() override
    {
        // level specific config
        m_type = Sandbox;
        // terrain
        m_terrain = SubstanceSoil();
        // tanks
        // REVIEW: the following belongs in a simple add tank function, or the like
        m_tankPool.reserve(4);

        m_tankPool.push_back( Tank( LocalPlayer, 512.f, 512.f, 0.f, 1.f ) );
        m_tankPool[0].SetSprites( texMgr.texTankBase, texMgr.texTankTurret, texMgr.texVFXShot1 );
        m_tankPool[0].SetSpriteScale( globalScale );
        m_tankPool[0].SetTankDustColor( m_terrain.GetDustColor() );
        //AddTank( m_tankPool[0] );

        m_tankPool.push_back( Tank( Drone, 640.f, 320.f, 0.f, 1.f ) );
        m_tankPool[1].SetTankColor( sf::Color(128.f, 16.f, 32.f, 255.f) );
        m_tankPool[1].SetSprites( texMgr.texTankBase, texMgr.texTankTurret, texMgr.texVFXShot1 );
        m_tankPool[1].SetSpriteScale( globalScale );
        m_tankPool[1].SetTankDustColor( m_terrain.GetDustColor() );

        m_tankPool.push_back( Tank( Drone, 420.f, 420.f, 180.f, 1.f ) );
        m_tankPool[2].SetTankColor( sf::Color(200.f,200.f,32.f,255.f) );
        m_tankPool[2].SetSprites( texMgr.texTankBase, texMgr.texTankTurret, texMgr.texVFXShot1 );
        m_tankPool[2].SetSpriteScale( globalScale );
        m_tankPool[2].SetTankDustColor( m_terrain.GetDustColor() );

        m_tankPool.push_back( Tank( Drone, 420.f, 580.f, 180.f, 1.f ) );
        m_tankPool[3].SetTankColor( sf::Color(16.f,32.f,200.f,255.f) );
        m_tankPool[3].SetSprites( texMgr.texTankBase, texMgr.texTankTurret, texMgr.texVFXShot1 );
        m_tankPool[3].SetSpriteScale( globalScale );
        m_tankPool[3].SetTankDustColor( m_terrain.GetDustColor() );

        // shots (not necessary, automatic with tanks)
        // scene objects
        // player setup
        // scene setup
        stats.maxPlayers = 1;
    }
};

static TankScene currentScene;

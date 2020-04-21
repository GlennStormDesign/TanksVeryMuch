#pragma once

// Scene Handling

// Tank Scenes

// handle scene stats, handle player stats

// primary job of scene is to take over updating and rendering of scene elements
// which should allow coordination of layers, batching, etc
//  terrain
//  tanks
//  shots
//  vfx
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
bool operator== ( const Tank& l, const Tank& r )
{
    return (l.tankID == r.tankID);
}
bool operator== ( const TankShot& l, const TankShot& r )
{
    return (l.shotID == r.shotID);
}
bool operator== ( const SceneObject& l, const SceneObject& r )
{
    return (l.objectID == r.objectID);
}
bool operator== ( const PlayerStats& l, const PlayerStats& r )
{
    return (l.playerTankID == r.playerTankID);
}

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

    void SceneInit()
    {
        // general configuration
        m_tankIndex = time(NULL);
        m_shotIndex = time(NULL);
        m_objIndex = time(NULL);
        m_playerIndex = time(NULL);

        LevelInit();

        // specific configuration
        // TODO: set terrainMgr view offset
        terrainMgr.SetTerrain( m_terrain );

        // launch scene
        // TODO: remove once LoadScene() is implemented
        stats.isSceneActive = true;
    }

    void UnloadScene()
    {
        if ( stats.isSceneActive )
        {
            stats.isSceneActive = false;
            // handle stats (recording, etc)
            // cleanup
        }
    }
    void LoadScene( const TankScene& level )
    {
        UnloadScene();
        // validate
        // initialize
        // launch
        SceneStats newStats;
        stats = newStats;
        stats.isSceneActive = true;
    }

    SceneType& GetSceneType()
    {
        return m_type;
    }
    void SetSceneType( const SceneType& type )
    {
        m_type = type;
    }

    void AddTank( Tank& t ) // REVISE: passing local var tank no worky
    {
        // TODO: check if max players will be exceeded
        m_tankPool.push_back(t); // TEST: investigate using emplace_back with complex constructor
        t.SetTankID( m_tankIndex++ );
        if ( t.controller.GetControllerType() == LocalPlayer ) // TODO: handle remote tank player
        {
            PlayerStats ps; // REVISE: passing local var player stats no worky
            AddPlayer( ps, t.GetTankID() ); // FIXME:
        }
    }
    void RemoveTank( Tank& t )
    {
        if ( t.controller.GetControllerType() == LocalPlayer ) // TODO: handle remote tank player
        {
            RemovePlayer( GetLocalPlayer() );
        }
        m_tankPool.erase( std::remove( m_tankPool.begin(), m_tankPool.end(), t ) );
    }
    Tank& GetTank( const unsigned int& index )
    {
        return m_tankPool[index];
    }
    Tank& GetLocalPlayerTank()
    {
        unsigned int idx = 0;
        for ( int i=0; i<m_tankPool.size(); i++ )
        {
            if ( m_tankPool[i].controller.GetControllerType() == LocalPlayer )
            {
                idx = i;
                break;
            }
        }
        return m_tankPool[idx];
    }
    unsigned int GetActiveTankCount()
    {
        unsigned int cnt = 0;
        for ( int i=0; i<m_tankPool.size(); i++ )
        {
            if ( m_tankPool[i].GetActiveState() )
            {
                cnt++;
            }
        }
        return cnt;
    }
    void AddShot( TankShot& s )
    {
        m_shotPool.push_back(s);
        s.SetShotID( m_shotIndex++ );
    }
    void RemoveShot( const TankShot& s )
    {
        m_shotPool.erase( std::remove( m_shotPool.begin(), m_shotPool.end(), s ) );
    }
    TankShot& GetShot( const unsigned int& index )
    {
        return m_shotPool[index];
    }
    void AddObject( SceneObject& o )
    {
        m_objectPool.push_back(o);
        o.SetObjectID( m_objIndex++ );
    }
    void RemoveObject( const SceneObject& o )
    {
        m_objectPool.erase( std::remove( m_objectPool.begin(), m_objectPool.end(), o ) );
    }
    SceneObject& GetObject( const unsigned int& index )
    {
        return m_objectPool[index];
    }
    void AddPlayer( PlayerStats& p, const unsigned int& tankID )
    {
        m_playerPool.push_back(p);
        p.playerTankID = tankID;
        p.isPlayerActive = true;
        stats.currentPlayers++;
    }
    void RemovePlayer( const PlayerStats p )
    {
        m_playerPool.erase( std::remove( m_playerPool.begin(), m_playerPool.end(), p ) );
        stats.currentPlayers--;
    }
    PlayerStats& GetPlayer( const unsigned int& index )
    {
        return m_playerPool[index];
    }
    PlayerStats& GetLocalPlayer()
    {
        PlayerStats returnPlayer;
        for ( int i=0; i<m_tankPool.size(); i++ )
        {
            if ( m_tankPool[i].controller.GetControllerType() == LocalPlayer )
            {
                unsigned int tid = m_tankPool[i].GetTankID();
                for ( int p=0; i<m_playerPool.size(); i++ )
                {
                    if ( m_playerPool[p].playerTankID == tid )
                        returnPlayer = m_playerPool[p];
                }
            }
        }
        return returnPlayer;
    }

    void UpdateScene( const float& timeDelta )
    {
        // handle scene stats
        if ( stats.isSceneActive )
        {
            stats.sceneActiveTime += timeDelta;
            // TODO: other scene stats
        }
        // handle player stats
        for ( int p=0; p<m_playerPool.size(); p++ )
        {
            if ( m_playerPool[p].isPlayerActive )
            {
                m_playerPool[p].playerActiveTime += timeDelta;
                // TODO: other player stats by shots, kills and respawns
            }
        }
        // handle player jump in/out (netbattle)
        // update tanks
        for ( int i=0; i<m_tankPool.size(); i++ ) // REVIEW [why this fails?] for ( Tank& t : m_tankPool )
        {
            m_tankPool[i].UpdateTank(timeDelta);
        }
        // perform collision checks among tanks
        // TODO: devise method that does not duplicate this check between tanks
        for ( int i=0; i<m_tankPool.size(); i++ )
        {
            if ( !m_tankPool[i].GetActiveState() )
                continue;
            for ( int n=0; n<m_tankPool.size(); n++ )
            {
                if ( !m_tankPool[n].GetActiveState() || i == n )
                    continue;
                else
                {
                    // handle bumping collision tank moves
                    sf::FloatRect hitBox = GetHitBox(m_tankPool[n].GetBaseSprite(), 0.381f);
                    if ( m_tankPool[i].GetBaseSprite().getGlobalBounds().intersects(hitBox) )
                    {
                        sf::Vector2f pos, other;
                        pos = m_tankPool[i].GetBaseSprite().getPosition();
                        other = m_tankPool[n].GetBaseSprite().getPosition();
                        m_tankPool[i].SetPosition( pos.x + ((pos.x-other.x)*timeDelta), pos.y + ((pos.y-other.y)*timeDelta) );
                        // TODO: could do other movement at same time, if check not duplicated
                    }
                }
            }
        }
        // perform collision checks between shots and tanks
        //   handle shot detonations and tank damage
        // TODO: think about a way to combine with the above iteration through pool
        for ( int i=0; i<m_tankPool.size(); i++ )
        {
            for ( int n=0; n<m_tankPool.size(); n++ )
            {
                if ( i != n )
                {
                    sf::FloatRect hitBox = GetHitBox(m_tankPool[n].GetBaseSprite(), 0.381f);
                    for ( int s=0; s<4; s++ ) {
                        if ( m_tankPool[n].GetActiveState() && m_tankPool[i].shots[s].active && m_tankPool[i].shots[s].shot.getGlobalBounds().intersects(hitBox) )
                        {
                            if ( m_tankPool[n].DamageTank(m_tankPool[i].shots[s].GetPower()) )
                            {
                                // TODO: other tank sfx loop kill
                                if ( !GetLocalPlayerTank().GetActiveState() )
                                    sfxMgr.SFXLoopKill();
                                sfxMgr.LaunchSFXKill();
                                if ( GetActiveTankCount() == 1 )
                                {
                                    if ( GetLocalPlayerTank().GetActiveState() )
                                        musicMgr.LaunchMusicSting(Win, false);
                                    else
                                        musicMgr.LaunchMusicSting(Lose, false);
                                    musicMgr.LaunchMusicLoop(Silent, true);
                                }
                                else if ( !GetLocalPlayerTank().GetActiveState() )
                                {
                                    musicMgr.LaunchMusicSting(Lose, false);
                                    musicMgr.LaunchMusicLoop(Silent, true);
                                }
                                m_tankPool[n].controller.SetActiveState(false); // unnecessary : see DestroyTank()
                            }
                            else
                                sfxMgr.LaunchSFXImpact();
                            m_tankPool[i].shots[s].Detonate();
                        }
                    }
                }
            }
        }
        // perform scene object updates (animated deco)
        // perform scene object collision checks tanks or shots (collidable, destructable, trigger)
        // TODO: set view port position to match local player tank position
    }

    void DrawScene( sf::RenderWindow& window, const sf::Vector2f& viewPos )
    {
        // do draw calls for terrain, tanks, shots and vfx
        terrainMgr.DrawTerrain( window, viewPos );
        // REVIEW: [sanity check below] draw layers appropriately ( all under vfx, then all tank dust, etc)
        for ( int t=0; t<m_tankPool.size(); t++ )
            m_tankPool[t].DrawKillUnderVFX( window );
        for ( int t=0; t<m_tankPool.size(); t++ )
            m_tankPool[t].DrawTankDustVFX( window );
        for ( int t=0; t<m_tankPool.size(); t++ )
            if ( m_tankPool[t].GetActiveState() )
                window.draw( m_tankPool[t].GetBaseSprite() );
        for ( int t=0; t<m_tankPool.size(); t++ )
            for ( int i=0; i<4; i++ )
            {
                m_tankPool[t].shots[i].DrawShot( window );
            }
        for ( int t=0; t<m_tankPool.size(); t++ )
            if ( m_tankPool[t].GetActiveState() )
                window.draw( m_tankPool[t].GetTurretSprite() );
        for ( int t=0; t<m_tankPool.size(); t++ )
            m_tankPool[t].DrawTankExhaustVFX( window );
        for ( int t=0; t<m_tankPool.size(); t++ )
            if (m_tankPool[t].GetActiveState() && m_tankPool[t].GetShotTimer() > 0.f)
                window.draw( m_tankPool[t].GetShotVFXSprite() );
        for ( int t=0; t<m_tankPool.size(); t++ )
            for ( int i=0; i<4; i++ )
            {
                m_tankPool[t].shots[i].DrawShotVFX( window );
            }
        for ( int t=0; t<m_tankPool.size(); t++ )
            m_tankPool[t].DrawKillOverVFX( window );
        // do draw calls for scene objects
    }
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
        // TODO: the following belongs in a simple add tank function, or the like
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

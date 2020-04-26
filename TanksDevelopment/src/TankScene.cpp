#include "TankScene.h"

// TankScene Definitions

bool operator== ( const Tank& l, const Tank& r )
{
    return (l.tankID == r.tankID);
}
bool operator== ( const SceneObject& l, const SceneObject& r )
{
    return (l.objectID == r.objectID);
}
bool operator== ( const PlayerStats& l, const PlayerStats& r )
{
    return (l.playerTankID == r.playerTankID);
}

// TankScene implementation

void TankScene::SceneInit()
{
    // general configuration
    m_tankIndex = time(NULL);
    m_objIndex = time(NULL);
    m_playerIndex = time(NULL);

    LevelInit();

    // specific configuration
    // REVIEW: set terrainMgr view offset
    terrainMgr.SetTerrain( m_terrain );

    // launch scene
    // REVIEW: remove once LoadScene() is implemented
    stats.isSceneActive = true;
}

void TankScene::UnloadScene()
{
    if ( stats.isSceneActive )
    {
        stats.isSceneActive = false;
        // handle stats (recording, etc)
        // cleanup
    }
}
void TankScene::LoadScene( const TankScene& level )
{
    UnloadScene();
    // validate
    // initialize
    // launch
    SceneStats newStats;
    stats = newStats;
    stats.isSceneActive = true;
}

SceneType& TankScene::GetSceneType()
{
    return m_type;
}
void TankScene::SetSceneType( const SceneType& type )
{
    m_type = type;
}

// explicitly copying tank here, expecting to create in local scope and pass here
void TankScene::AddTank( Tank t )
{
    // REVIEW: check if max players will be exceeded, handle reject new player
    t.SetSprites( texMgr.texTankBase, texMgr.texTankTurret, texMgr.texVFXShot1 );
    t.SetSpriteScale( globalScale );
    t.SetTankDustColor( m_terrain.GetDustColor() );
    t.SetTankID( m_tankIndex++ );
    if ( t.controller.GetControllerType() == LocalPlayer )
    {
        PlayerStats ps;
        ps.playerTankID = t.GetTankID();
        AddPlayer( ps );
    }
    m_tankPool.push_back(t); // TODO: c++11 enhancement using emplace_back with complex constructor
}
void TankScene::AddTank( Tank t, sf::Color c )
{
    t.SetTankColor( c );
    AddTank( t );
}
void TankScene::RemoveTank( Tank& t )
{
    if ( t.controller.GetControllerType() == LocalPlayer )
    {
        RemovePlayer( GetLocalPlayer() );
    }
    m_tankPool.erase( std::remove( m_tankPool.begin(), m_tankPool.end(), t ) );
}
Tank& TankScene::GetTank( const unsigned int& index )
{
    return m_tankPool[index];
}
Tank& TankScene::GetLocalPlayerTank()
{
    unsigned int idx = 0;
    for ( unsigned int i=0; i<m_tankPool.size(); i++ )
    {
        if ( m_tankPool[i].controller.GetControllerType() == LocalPlayer )
        {
            idx = i;
            break;
        }
    }
    return m_tankPool[idx];
}
unsigned int TankScene::GetActiveTankCount()
{
    unsigned int cnt = 0;
    for ( unsigned int i=0; i<m_tankPool.size(); i++ )
    {
        if ( m_tankPool[i].GetActiveState() )
        {
            cnt++;
        }
    }
    return cnt;
}

void TankScene::AddObject( SceneObject o )
{
    // SceneObject subclasses of different sizes, stored as pointers
    o.SetObjectID( m_objIndex++ );
    m_objectPool.push_back( o.clone() );
}

/*
// REVIEW : with pointers as elements, no worky
void TankScene::RemoveObject( const SceneObject& o )
{
    m_objectPool.erase( std::remove( m_objectPool.begin(), m_objectPool.end(), o ) );
}
*/

void TankScene::SetObject( const unsigned int& index, const SceneObject& o )
{
    delete m_objectPool[index];
    m_objectPool[index] = o.clone();
}

const SceneObject& TankScene::GetObject( const unsigned int& index )
{
    return *m_objectPool[index];
}

void TankScene::AddPlayer( PlayerStats p )
{
    p.isPlayerActive = true;
    m_playerPool.push_back(p);
    stats.currentPlayers++;
}
void TankScene::RemovePlayer( const PlayerStats p )
{
    m_playerPool.erase( std::remove( m_playerPool.begin(), m_playerPool.end(), p ) );
    stats.currentPlayers--;
}
PlayerStats& TankScene::GetPlayer( const unsigned int& index )
{
    return m_playerPool[index];
}
PlayerStats& TankScene::GetLocalPlayer()
{
    PlayerStats returnPlayer;
    for ( unsigned int i=0; i<m_tankPool.size(); i++ )
    {
        if ( m_tankPool[i].controller.GetControllerType() == LocalPlayer )
        {
            unsigned int tid = m_tankPool[i].GetTankID();
            for ( unsigned int p=0; i<m_playerPool.size(); i++ )
            {
                if ( m_playerPool[p].playerTankID == tid )
                    returnPlayer = m_playerPool[p];
            }
        }
    }
    return returnPlayer;
}

void TankScene::UpdateScene( const float& timeDelta )
{
    // handle scene stats
    if ( stats.isSceneActive )
    {
        stats.sceneActiveTime += timeDelta;
        // TODO: other scene stats
    }
    // handle player stats
    for ( unsigned int p=0; p<m_playerPool.size(); p++ )
    {
        if ( m_playerPool[p].isPlayerActive )
        {
            m_playerPool[p].playerActiveTime += timeDelta;
            // TODO: other player stats by shots, kills and respawns
        }
    }
    // handle player jump in/out (net battle)
    // update tanks
    for ( unsigned int i=0; i<m_tankPool.size(); i++ ) // REVIEW [why this fails?] for ( Tank& t : m_tankPool )
    {
        m_tankPool[i].UpdateTank(timeDelta);
    }
    // perform collision checks among tanks
    // REVIEW: devise method that does not duplicate this check between tanks
    for ( unsigned int i=0; i<m_tankPool.size(); i++ )
    {
        if ( !m_tankPool[i].GetActiveState() )
            continue;
        for ( unsigned int n=0; n<m_tankPool.size(); n++ )
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
                    // REVIEW: could do other movement at same time, if check not duplicated
                }
            }
        }
    }
    // perform collision checks between shots and tanks
    //   handle shot detonations and tank damage
    // REVIEW: think about a way to combine with the above iteration through pool
    for ( unsigned int i=0; i<m_tankPool.size(); i++ )
    {
        for ( unsigned int n=0; n<m_tankPool.size(); n++ )
        {
            if ( i != n )
            {
                sf::FloatRect hitBox = GetHitBox(m_tankPool[n].GetBaseSprite(), 0.381f);
                for ( int s=0; s<4; s++ ) {
                    if ( m_tankPool[n].GetActiveState() && m_tankPool[i].shots[s].active && m_tankPool[i].shots[s].shot.getGlobalBounds().intersects(hitBox) )
                    {
                        if ( m_tankPool[n].DamageTank(m_tankPool[i].shots[s].GetPower()) )
                        {
                            // REVIEW: other tank sfx loop kill
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
    for ( unsigned int t=0; t<m_tankPool.size(); t++ )
    {
        sf::FloatRect hitBox = GetHitBox( m_tankPool[t].GetBaseSprite(), 0.381f );
        if ( m_tankPool[t].GetActiveState() )
        {
            for ( unsigned int o=0; o<m_objectPool.size(); o++ )
            {
                if ( m_objectPool[o]->active && m_objectPool[o]->type == Obstacle ) // TODO: trigger and destructable
                {
                    // if ( hitBox.contains( static_cast<SceneObstacle>m_objectPool[o].GetHitBox() ) )
                }
            }
        }
    }
}

void TankScene::DrawScene( sf::RenderWindow& window )
{
    // first reposition view to match player tank position, if active, and re-set view to window
    sf::View v = window.getView();
    if ( GetLocalPlayerTank().GetActiveState() )
        v.setCenter( GetLocalPlayerTank().GetBaseSprite().getPosition() );
    window.setView(v);
    // do draw calls for terrain, tanks, shots and vfx
    terrainMgr.DrawTerrain( window, v.getCenter() );

    // draw scene objects
    // REVIEW: scene object layering
    for ( unsigned int o=0; o<m_objectPool.size(); o++ )
    {
        if ( m_objectPool[o]->visible )
            m_objectPool[o]->DrawSceneObject( window, v.getCenter() );
    }

    // REVIEW: [sanity check below] draw layers appropriately ( all under vfx, then all tank dust, etc)
    for ( unsigned int t=0; t<m_tankPool.size(); t++ )
        m_tankPool[t].DrawKillUnderVFX( window );
    for ( unsigned int t=0; t<m_tankPool.size(); t++ )
        m_tankPool[t].DrawTankDustVFX( window );
    for ( unsigned int t=0; t<m_tankPool.size(); t++ )
        if ( m_tankPool[t].GetActiveState() )
            window.draw( m_tankPool[t].GetBaseSprite() );
    for ( unsigned int t=0; t<m_tankPool.size(); t++ )
        for ( int i=0; i<4; i++ )
        {
            m_tankPool[t].shots[i].DrawShot( window );
        }
    for ( unsigned int t=0; t<m_tankPool.size(); t++ )
        if ( m_tankPool[t].GetActiveState() )
            window.draw( m_tankPool[t].GetTurretSprite() );
    for ( unsigned int t=0; t<m_tankPool.size(); t++ )
        m_tankPool[t].DrawTankExhaustVFX( window );
    for ( unsigned int t=0; t<m_tankPool.size(); t++ )
        if (m_tankPool[t].GetActiveState() && m_tankPool[t].GetShotTimer() > 0.f)
            window.draw( m_tankPool[t].GetShotVFXSprite() );
    for ( unsigned int t=0; t<m_tankPool.size(); t++ )
        for ( int i=0; i<4; i++ )
        {
            m_tankPool[t].shots[i].DrawShotVFX( window );
        }
    for ( unsigned int t=0; t<m_tankPool.size(); t++ )
        m_tankPool[t].DrawKillOverVFX( window );
    // do draw calls for scene objects
}

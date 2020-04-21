#include <math.h>
#include <cmath>
#include <string>
#include <bitset>

#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Audio.hpp>

#include "TankCore.h"
#include "FontHandle.h"
#include "EntryPoint.h"
#include "AudioHandle.h"
// TODO: SoundSource setup
// FIXME: [BUG] Launching silent loop from other 1st time causes instant duck down
#include "ImageHandle.h"
#include "UIHandle.h"
#include "VFXHandle.h"
// FIXME: [BUG] VFXShotImpact fails to launch appropriately on subsequent shots (effectively just one?)
#include "Tank.h"
// FIXME: [BUG] tank idle sfx can 'restart' after player killed (yet, controller and tank are inactive)
#include "Terrain.h"
#include "SceneObject.h"
#include "TankScene.h"
#include "TankGame.h"

//    TANKS VERY MUCH !
// GHS Game -- Spring 2020
// C++, SFML, OpenGL, GLSL

int main()
{
	#ifdef _DEBUG
        debugText = "[DEBUG] ";
        debugText += TANKSVERYMUCH_VERSION;
        debugText += "\n\n";
    #endif

    // rand seed
    srand(time(NULL));

    // splash screen
    if ( !Tanks::Splash() ) {
        // TODO: handle no image or audio access
        return -1;
    }

    // main game setup //

    // window
    sf::RenderWindow rWin( sf::VideoMode( 1024, 576, 256 ), "TANKS VERY MUCH" );
    sf::VideoMode vm( sf::VideoMode::getDesktopMode() );
    rWin.setPosition( sf::Vector2i( (( vm.width / 2 ) - 512 ) , (( vm.height / 2 ) - 320 ) ) );

    // view port
    sf::View vw;
    vw.setSize(1024.f, 576.f);
    vw.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    vw.setCenter(512.f, 512.f);

    // time
    float timeDelta = 0.f;
    sf::Clock frameTimer;
    frameTimer.restart();

    // ui
    // TODO: ui manager class
    // TODO: various ui element definitions

    // scene
    currentScene = TestTankScene();

    // terrain configuration
    // TODO: migrate to TankScene class
    terrainMgr.SetViewOffset( -vw.getCenter() );

    // debug feedback display
    sf::Text debugLine;
    debugLine.setFont(detailFont);
    debugLine.setCharacterSize(32);
    debugLine.setScale(.381f,.381f);
    debugLine.setFillColor(sf::Color::Yellow);
    sf::Vector2f debugLineOffset = sf::Vector2f( -500.f, -288.f );

    // delay music start
    musicMgr.LaunchMusicLoop( Game, true );
    musicMgr.musicChanged = true;

    // main loop
    rWin.setActive();
    rWin.setFramerateLimit(60);
    while ( rWin.isOpen() )
    {
        // time delta
        timeDelta = frameTimer.restart().asSeconds() * globalTime;

        // window events
        sf::Event event;
        while (rWin.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                rWin.close();
            if (event.type == sf::Event::Resized) {
                rWin.setSize( sf::Vector2u( 1024, 576 ) );
                rWin.setPosition( sf::Vector2i( (( vm.width / 2 ) - 512 ) , (( vm.height / 2 ) - 320 ) ) );
            }
        }

        // audio handling
        musicMgr.MusicLoopUpdate( timeDelta );
        musicMgr.MusicStingUpdate( timeDelta );

        // ** audio testing [note: overrides normal audio handling]
        musicMgr.Testing( true );
        sfxMgr.Testing( false, timeDelta );

        // scene update
        // tank and shot collisions with tanks requires knowledge of all tanks in scene
        currentScene.UpdateScene( timeDelta );

        // view follows player tank
        // TODO: migrate this to TankScene, and in the process, eliminate need to pass this on scene update
        if ( currentScene.GetLocalPlayerTank().GetActiveState() )
            vw.setCenter( currentScene.GetLocalPlayerTank().GetBaseSprite().getPosition() );

        // draw calls
        rWin.clear();
        rWin.setView( vw );

        // draw scene
        currentScene.DrawScene( rWin, vw.getCenter() );

        // ui draw

        // debug draw
        debugLine.setString( debugText );
        debugLine.setPosition( vw.getCenter() + debugLineOffset );
        if ( debugLine.getString() != "" )
            rWin.draw( debugLine );
        rWin.display();
    }
}

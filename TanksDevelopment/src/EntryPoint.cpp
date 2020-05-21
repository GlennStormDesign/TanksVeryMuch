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
#include "ImageHandle.h"
#include "UIHandle.h"
#include "VFXHandle.h"
#include "Tank.h"
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
        SetDebugText( FormatDebugHeader() );
    #endif

    // rand seed
    srand(time(NULL));

    // splash screen
    if ( !Tanks::Splash() ) {
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

    // scene
    NewScene( new TestTankScene() ); // sandbox used during splash and menu

    // ui
    UIInit( rWin.getSize() );

    // debug feedback display
    DebugInit( DetailFont() );

    // sfx start
    // REVIEW: how to 'touch' local player tank to start engine idle loop
    // music start
    LaunchMusicLoop( Pause, true );

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

        // audio update
        //sf::Vector3f lPos;
        //lPos.x = GetLocalPlayerTank().GetBaseSprite().getPosition().x;
        //lPos.y = GetLocalPlayerTank().GetBaseSprite().getPosition().y;
        //lPos.z = 0.f;
        //sf::Listener::setPosition( lPos );
        MusicStingUpdate( timeDelta );
        MusicLoopUpdate( timeDelta );
        SFXLoopUpdate( timeDelta );

        // TEMP: audio testing [note: overrides normal audio handling]
        //MusicTesting( true );
        //SFXTesting( false, timeDelta );

        // scene update
        UpdateScene( timeDelta );

        // ui update
        UpdateUI( timeDelta );

        // draw calls
        rWin.clear();

        // draw scene (includes repositioning and setting view)
        DrawScene( rWin );

        // ui draw
        sf::Vector2f uiOffset = GetLocalPlayerTank().GetBaseSprite().getPosition(); // REVIEW: use view position? (cam shake)
        uiOffset.x -= rWin.getSize().x / 2;
        uiOffset.y -= rWin.getSize().y / 2;
        DrawUI( rWin, uiOffset );

#ifndef _DEBUG
        // temp debug in release mode
        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Tilde ) )
        {
            SetDebugText( FormatDebugHeader() );
            AddDebugText("Frame rate: ");
            AddDebugText( FormatFloat( 1.f / timeDelta ) );
            AddDebugText(" fps\n");
            AddDebugText("Active tank count: ");
            AddDebugText( FormatInt( GetActiveTankCount() ) );
            AddDebugText("\n");
            AddDebugText("Local Player Tank Position: ");
            AddDebugText( FormatVector2f( GetLocalPlayerTank().GetBaseSprite().getPosition() ) );
            AddDebugText("\n");
            AddDebugText("Local Player Tank Rotation: ");
            AddDebugText( FormatFloat( GetLocalPlayerTank().GetBaseSprite().getRotation() ) );
            AddDebugText("\n");
            AddDebugText("Mouse Position: ");
            AddDebugText( FormatVector2i( sf::Mouse::getPosition() ) );
        }
        else
            SetDebugText("");
#endif // _DEBUG

        // debug draw
        DrawDebug( rWin );

        // display window
        rWin.display();
    }
}

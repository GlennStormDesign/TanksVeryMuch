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

    // ui

    // scene
    NewScene( TestTankScene() );
    // TEST: if no local player tank, does terrainMgr set correct viewOffset ?

    // debug feedback display
    DebugInit( DetailFont() );

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
        MusicStingUpdate( timeDelta );
        MusicLoopUpdate( timeDelta );
        SFXLoopUpdate( timeDelta );

        // TEMP: audio testing [note: overrides normal audio handling]
        MusicTesting( true );
        SFXTesting( false, timeDelta );

        // scene update
        UpdateScene( timeDelta );

        // draw calls
        rWin.clear();

        // draw scene (includes repositioning and setting view)
        DrawScene( rWin );

        // ui draw
        sf::RenderTexture rt;
        sf::Sprite panel = PanelRect( rt, sf::IntRect( 0, 0, 256, 64 ) );
        panel.setPosition( (GetLocalPlayerTank().GetBaseSprite().getPosition().x - 512 + 256), (GetLocalPlayerTank().GetBaseSprite().getPosition().y - 512 + 640) );
        panel.setColor( sf::Color(255,255,255,128) );
        rWin.draw( panel );
        sf::Text testUI;
        testUI.setFont( HeadingFont() );
        testUI.setColor( sf::Color(0,0,0,128) );
        testUI.setString("This user interface is\nbeginning to look\nlike something."); // TODO: auto-calc new lines using local bound checks
        sf::Vector2f textPos = panel.getPosition();
        textPos.x += ((512 - testUI.getLocalBounds().width-8 ) / 2);
        textPos.y += ((128 - testUI.getLocalBounds().height-8 ) / 2);
        testUI.setPosition( textPos );
        rWin.draw(testUI);

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

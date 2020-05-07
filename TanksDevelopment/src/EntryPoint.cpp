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
        sf::Vector2f uiOffset = GetLocalPlayerTank().GetBaseSprite().getPosition();
        uiOffset.x -= 512;
        uiOffset.y -= 512;

        sf::RenderTexture rt;
        sf::Sprite panel = PanelRect( rt, sf::IntRect( 0, 0, 512, 128 ) );
        panel.setPosition( (uiOffset.x + 256), (uiOffset.y + 640) );
        panel.setColor( sf::Color(255,255,255,128) );
        if ( GetActiveTankCount() == 1 || !GetLocalPlayerTank().GetActiveState() )
            rWin.draw( panel );
        sf::Text testUI;
        testUI.setFont( HeadingFont() );
        testUI.setColor( sf::Color(0,0,0,128) );
        std::string endString = "Last tank standing\n";
        if ( GetActiveTankCount() > 1 && !GetLocalPlayerTank().GetActiveState() )
            endString = "Your tank destroyed\n";
        if ( GetLocalPlayerTank().GetActiveState() )
            endString += "You win!\n";
        else
            endString += "You lose ...\n";
        endString += "Tanks for playing";
        testUI.setString(endString); // TODO: auto-calc new lines using local bound checks
        sf::Vector2f textPos = panel.getPosition();
        textPos.x += ((512 - testUI.getLocalBounds().width-8 ) / 2);
        textPos.y += ((128 - testUI.getLocalBounds().height-8 ) / 2);
        testUI.setPosition( textPos );
        if ( GetActiveTankCount() == 1 || !GetLocalPlayerTank().GetActiveState()  )
            rWin.draw(testUI);

        sf::IntRect labelBounds;
        labelBounds.left = rWin.getSize().x - 256 - 8;
        labelBounds.left += 512 - ( rWin.getSize().x /2 );
        labelBounds.top = 8;
        labelBounds.top += 512 - ( rWin.getSize().y /2 );
        labelBounds.width = 256;
        labelBounds.height = 64;
        panel = PanelRect( rt, labelBounds );
        panel.setPosition( labelBounds.left + uiOffset.x, labelBounds.top + uiOffset.y );
        rWin.draw(panel);
        sf::Text labelText;
        labelText.setFont( TextFont() );
        labelText.setCharacterSize( 28 );
        labelText.setString("Tanks Very Much");
        sf::Vector2f labelPos;
        labelPos.x = labelBounds.left + uiOffset.x;
        labelPos.y = labelBounds.top + uiOffset.y;
        // drop shadow drawn first
        labelText.setColor( sf::Color(128,128,128,255) );
        // [left align]
        //labelPos.x += (labelText.getCharacterSize()*0.04f * 8.f);
        // [end left align]
        // [right align]
        //labelPos.x += ( labelBounds.width ) - (labelText.getCharacterSize()*0.04f * 9.f) - labelText.getLocalBounds().width;
        // [end right align]
        // [center align]
        labelPos.x += ( labelBounds.width /2 ) - ( labelText.getLocalBounds().width/2 );
        // [end center align]
        // (for drop shadow, only half of vertical offset, no horizontal offset)
        labelPos.y += ( labelBounds.height /2 ) - ((labelText.getCharacterSize()*0.115f) /2 )- ( labelText.getLocalBounds().height/2 );
        labelText.setPosition( labelPos );
        rWin.draw(labelText);
        // main text draw
        labelText.setColor( sf::Color::Black );
        labelPos.x = labelBounds.left + uiOffset.x;
        labelPos.y = labelBounds.top + uiOffset.y;
        // [left align]
        //labelPos.x += (labelText.getCharacterSize()*0.04f * 7.f);
        // [end left align]
        // [right align]
        //labelPos.x += ( labelBounds.width ) - (labelText.getCharacterSize()*0.04f * 10.f) - labelText.getLocalBounds().width;
        // [end right align]
        // [center align]
        // NOTE: centering involves reducing space, which is proportional to size and differs among fonts
        labelPos.x += ( labelBounds.width /2 ) - (labelText.getCharacterSize()*0.04f) - ( labelText.getLocalBounds().width/2 );
        // [end center align]
        labelPos.y += ( labelBounds.height /2 ) - (labelText.getCharacterSize()*0.115f) - ( labelText.getLocalBounds().height/2 );
        labelText.setPosition( labelPos );
        rWin.draw(labelText);

        // Heading Notes:
        // Headings can group associated values together for ease of use
        // Font, line size proportion, size, color, alignment can all be a part of a heading
        // This could/should be implemented in FontHandle

        // Field Notes:
        // A field is a solid color rectangle, suitable for tooltip, or simply colorizing a background
        // It will need a rect and a color
        sf::IntRect r = sf::IntRect(300,300,128,64);
        sf::Color c = sf::Color::Blue;
        UIField f( r, c );
        sf::Vector2f uiO = uiOffset; // REFACTOR: Turn this 'zeroed' uiOffset into pixel position of screen space within view
        uiO.x += 512 - (rWin.getSize().x / 2); // NOTE: '512' is the original position the view was set (tank position)
        uiO.y += 512 - (rWin.getSize().y / 2);
        // NOTE: iow, this is viewposition - half window size (need view position as input at all?)
        f.DrawUI(rWin, uiO);

        // Label Notes:
        // A label will need to know the string to display, the rect to use, and the heading

        // Button Notes:
        // A button can use a button heading, and will then need to know the string to display and the rect to use
        // It will also need to know what to do when button is pressed ( an arbitrary action ) (function callback?)

        // Box Notes:
        // A box is a panel with a label heading and at least one button
        // It can use box headings, but it will need to know label string, button strings (+callbacks), and a rect to use

        // Panel Notes:
        // A panel is a vertical series of labels (left) and buttons (right), a label heading and at least one button bottom

        // HUD Notes:
        //

        // Menu Notes:
        //

        // UI Manager Notes:
        // UI Manager should handle creation, update and drawing of ui elements in concert
        // Based on tests above, it can/should handle RenderTexture, layer sorting, button callbacks, ui animation

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

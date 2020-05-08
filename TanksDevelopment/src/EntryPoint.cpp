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
    // (temp)
    bool displayBox = false;
    bool buttonWasPressed = false;
    int confirmCallback = 0;
    float progressBarValue = 0.15f;

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

        // (temp fix for un-focused window) (fold into DrawScene?)
        GetLocalPlayerTank().controller.SetActiveState( rWin.hasFocus() );

        // draw calls
        rWin.clear();

        // draw scene (includes repositioning and setting view)
        DrawScene( rWin );

        // ui draw
        sf::Vector2f uiOffset = GetLocalPlayerTank().GetBaseSprite().getPosition();
        uiOffset.x -= rWin.getSize().x / 2;
        uiOffset.y -= rWin.getSize().y / 2;

        sf::RenderTexture rt;
        sf::Sprite panel = PanelRect( rt, sf::IntRect( 0, 0, 512, 128 ) );
        panel.setPosition( (uiOffset.x + 256), (uiOffset.y + 440) );
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
        labelBounds.top = 8;
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
        UIField f( sf::IntRect(50,50,192,64), sf::Color(200,200,128,255) );
        f.DrawUI(rWin, uiOffset);

        // Frame Notes:
        UIFrame fm( sf::IntRect(50,125,192,64), sf::Color::White );
        fm.DrawUI(rWin, uiOffset);

        // Label Notes:
        // A label will need to know the string to display, the rect to use, the heading, and if draw drop shadow
        HUDLabelWhite fh;
        UILabel l( sf::IntRect(50,200,192,64), sf::Color(255,255,255,128), fh, true, "TESTING" );
        l.DrawUI(rWin, uiOffset);

        // Icon Notes:
        // An image (texture) display, needs rect, color, texture and a scale factor
        UIIcon i( sf::IntRect(50,275,192,64), sf::Color::White, TexVFXShot5(), 2.f );
        i.DrawUI(rWin, uiOffset);

        // Button Notes:
        // A button can use a button heading, and will then need to know the string to display and the rect to use
        // It will also need to know what to do when button is pressed ( an arbitrary action ) (function callback?)
        // REVIEW: condense this series of elements used to build button, into just a call for a button
        ClearLargeHeading bh;
        UILabel bl( sf::IntRect(50,350,192,64), sf::Color::White, bh, false, "PRESS ME" );
        UIButton b( sf::IntRect(50,350,192,64), sf::Color::White, bl );
        //b.SetState(Disabled);
        b.DrawUI(rWin, uiOffset);
        if ( b.GetState() == Active )
        {
            LaunchSFXUIFwd(); // UIButton object state can be used to signal press
            if ( !buttonWasPressed )
            {
                buttonWasPressed = true; // NOTE: all buttons should do this ( toggle only per press )
                displayBox = !displayBox;
            }
        }
        else
            buttonWasPressed = false;

        // Box Notes:
        // A box is a panel with a label heading and at least one button
        // It can use box headings, but it will need to know label string, button strings (+callbacks), and a rect to use
        UIBox bx( sf::IntRect(256,256,512,192), sf::Color::White, "UI Box", "This is a message for you", "OK" );
        // NOTE: using above button for toggle display of this box
        bx.visible = false; //displayBox;
        // REVIEW: no visible UIElement is active?
        bx.DrawUI(rWin, uiOffset);
        confirmCallback = bx.GetCallBack();
        if ( confirmCallback == 1 )
        {
            displayBox = false;
            LaunchSFXUIBack();
        }

        // UI Confirm Box Notes:
        // A box with multiple buttons for accept, (alt) and cancel
        UIConfirm cb( sf::IntRect(256,256,512,192), sf::Color::White, "UI Confirm", "Please confirm this message inquiry", "YES", "MAYBE", "NO" );
        cb.visible = displayBox;
        cb.DrawUI(rWin, uiOffset);
        confirmCallback = cb.GetCallBack();
        if ( confirmCallback == 1 )
        {
            SetDebugText( "Confirm = 1" );
            LaunchSFXUIFwd();
            displayBox = false;
            progressBarValue += 0.15f;
        }
        else if ( confirmCallback == 2 )
        {
            SetDebugText( "Confirm = 2" );
            LaunchSFXImpact();
            displayBox = false;
            progressBarValue += 0.1f;
        }
        else if ( confirmCallback == 3 )
        {
            SetDebugText( "Confirm = 3" );
            LaunchSFXUIBack();
            displayBox = false;
            progressBarValue += 0.05f;
        }
        else
            SetDebugText( FormatDebugHeader() );

        // Progress Bar Notes:
        // A background field and a brighter color field that scales from one side to indicate a proportion
        UIProgressBar p( sf::IntRect(50,425,192,64), sf::Color::Red, sf::Color(128,128,128,64), false, 4 );
        p.DrawUI(rWin,uiOffset);
        p.SetProgress(progressBarValue);

        // HUD Notes:
        // A collection of icons and labels arranged on top of the active game view port

        // Menu Notes:
        // A menu is a vertical series of labels (left) and buttons (right), a label heading and at least one button bottom

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

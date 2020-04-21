#pragma once

// Startup Handling

namespace Tanks {

    bool Splash()
    {
        sf::RenderWindow window( sf::VideoMode( 320, 180 ), "TankSplash", sf::Style::None );
        sf::VideoMode vm( sf::VideoMode::getDesktopMode() );
        window.setPosition( sf::Vector2i( (( vm.width / 2 ) - 160) , (( vm.height / 2 ) - 90) ) );

        // ensure splash is displayed
        sf::Clock safetyClock;
        safetyClock.restart();

        // init fonts
        bool loadOkay = FontInit();

        sf::Text t;
        sf::Text subT;
        sf::Text versionT;
        t.setFont(titleFont);
        sf::Vector2f v = sf::Vector2f();
        v.x = 110;
        v.y = 16;
        t.setPosition(v);
        t.setCharacterSize(30);
        t.setFillColor(sf::Color::White);
        t.setString("Tanks\nVery\nMuch");
        subT = t;
        v.x = 64;
        v.y = 132;
        subT.setPosition(v);
        subT.setCharacterSize(16);
        subT.setFillColor(sf::Color::White);
        subT.setString("GHS_Game_Spring_2020");
        versionT.setFont(textFont);
        v.x = 245;
        v.y = 160;
        versionT.setPosition(v);
        versionT.setCharacterSize(8);
        versionT.setFillColor(sf::Color::White);
        versionT.setString(TANKSVERYMUCH_VERSION);

        sf::SoundBuffer sb;
        if ( !sb.loadFromFile("audio/Music_Sting_Snare.wav") ) {
            // error
            loadOkay = false;
        }
        sf::Sound s;
        s.setBuffer(sb);
        s.play();

        while (window.isOpen() && ( s.getStatus() == s.Playing || safetyClock.getElapsedTime().asSeconds() < 3.5f ) )
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            window.draw(t);
            window.draw(subT);
            window.draw(versionT);
            window.display();
        }

        return loadOkay;
    }

}

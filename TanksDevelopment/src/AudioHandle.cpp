#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "AudioHandle.h"

sf::Clock fxsStep; // temp?

int LaunchSFXLoop( const sf::SoundBuffer& sb )
{
    if ( fxsStep.restart().asSeconds() < MIN_SFX_INTERVAL )
        return -1;
    int slotIndex = -1;
    for ( int i=0; i<MAX_SFX_LOOPS; i++ ) {
        if ( !fxl[i].getLoop() )
        {
            fxl[i].setBuffer( sb );
            fxl[i].setLoop(true);
            fxl[i].play();
            slotIndex = i;
            break;
        }
    }
    return slotIndex;
}

int LaunchSFXLoop( const sf::SoundBuffer& sb, const float& volume, const float& pitch )
{
    if ( fxsStep.restart().asSeconds() < MIN_SFX_INTERVAL )
        return -1;
    int slotIndex = -1;
    for ( int i=0; i<MAX_SFX_LOOPS; i++ ) {
        if ( !fxl[i].getLoop() )
        {
            fxl[i].setBuffer( sb );
            fxl[i].setLoop(true);
            fxl[i].setVolume(volume);
            fxl[i].setPitch(pitch);
            fxl[i].play();
            slotIndex = i;
            break;
        }
    }
    return slotIndex;
}

void TouchSFXLoop( const int& index, const float& volume, const float& pitch, const bool& stop )
{
    fxl[index].setVolume(volume);
    fxl[index].setPitch(pitch);
    if ( stop )
    {
        fxl[index].setLoop(false);
        fxl[index].stop();
        fxl[index].resetBuffer();
    }
}

void LaunchSFXSting( const sf::SoundBuffer& sb )
{
    if ( fxsStep.restart().asSeconds() < MIN_SFX_INTERVAL )
        return;
    for ( int i=0; i<MAX_SFX_STINGS; i++ ) {
        if ( fxs[i].getStatus() == sf::Sound::Stopped )
        {
            fxs[i].setBuffer( sb );
            fxs[i].play();
            break;
        }
    }
}

// AudioMusicManager implementation

void AudioMusicManager::MusicStingUpdate( const float& timeDelta )
{
    // handle music sting
    if ( musicFadeTimer == 0.f && musicFadeLaunched )
    {
        switch (stingMode)
        {
            //case None : msting.setBuffer(); break;
            case Iron : msting.setBuffer(msbIron); break;
            case Snare : msting.setBuffer(msbSnare); break;
            case Win : msting.setBuffer(msbWin); nextMusicMode = Silent; break;
            case Lose : msting.setBuffer(msbLose); nextMusicMode = Silent; break;
            default : break;
        }
        if ( msting.getBuffer() )
        {
            msting.play();
            musicFadeTimer = MUSIC_FADE_DURATION;
            musicFadeLaunched = false;
        }
    }
    else if ( musicFadeTimer > 0.f )
    {
        float progess = ( musicFadeTimer / MUSIC_FADE_DURATION );
        mloop.setVolume( progess * MUSIC_LOOP_MAX_VOLUME );
        if ( msting.getStatus() == sf::Sound::Playing )
            musicFadeTimer -= timeDelta;
        else
            musicFadeTimer += timeDelta;
        if ( musicFadeTimer < 0.f )
        {
            musicFadeTimer = 0.001f; // not zero
            if ( stingMode == Win || stingMode == Lose )
            {
                mloop.stop();
                musicMode = nextMusicMode;
                stingMode = None;
            }
        }
        else if ( musicFadeTimer > MUSIC_FADE_DURATION )
        {
            musicFadeTimer = 0.f;
            mloop.setVolume(MUSIC_LOOP_MAX_VOLUME);
        }
    }
}
void AudioMusicManager::MusicLoopUpdate( const float& timeDelta )
{
    // handle music loop
    if ( musicChanged ) {
        musicChanged = false;
        if ( nextMusicMode != Silent && musicMode == Silent && musicFadeTimer == 0.f && mloop.getVolume() < MUSIC_LOOP_MAX_VOLUME )
            musicFadeTimer = 0.001f; // not zero, will cause fade up
        if ( nextMusicMode == Silent )
        {
            if ( musicMode == Pause )
                mloop.stop();
            musicMode = Silent;
        }
        if ( nextMusicMode == Menu )
        {
            if ( musicMode == Pause )
            {
                mloop.stop();
                mloop.setBuffer(msbMenu);
                mloop.play();
            }
            musicMode = Menu;
        }
        if ( nextMusicMode == Game )
        {
            if ( musicMode == Pause )
            {
                mloop.stop();
                mloop.setBuffer(msbGame);
                mloop.play();
            }
            musicMode = Game;
        }
        if ( nextMusicMode == Pause )
        {
            if ( musicMode != Pause )
            {
                mloop.stop();
                mloop.setBuffer(msbPause);
                mloop.play();
            }
            musicMode = Pause;
        }
    }
    if ( soundOkay && mloop.getStatus() != sf::Sound::Playing ) {
        if ( musicMode == Menu )
            mloop.setBuffer(msbMenu);
        else if ( musicMode == Game )
            mloop.setBuffer(msbGame);
        else if ( musicMode == Pause )
            mloop.setBuffer(msbPause);
        if ( musicMode != Silent )
            mloop.play(); // loop
    }
}
void AudioMusicManager::LaunchMusicLoop( const MLoopMode& loop, const bool& interrupt )
{
    nextMusicMode = loop;
    musicChanged = (nextMusicMode != musicMode);
    if ( interrupt )
        mloop.stop();
}
void AudioMusicManager::LaunchMusicSting( const MStingMode& sting, const bool& interrupt )
{
    if ( musicFadeTimer == 0.f || interrupt )
    {
        stingMode = sting;
        musicFadeLaunched = true;
    }
}

void AudioMusicManager::Testing( const bool& debug )
{
    if ( !debug )
        return;

    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Space) )
    {
        nextMusicMode = Silent;
        musicChanged = (nextMusicMode != musicMode);
    }
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::M) )
    {
        nextMusicMode = Menu;
        musicChanged = (nextMusicMode != musicMode);
    }
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::G) )
    {
        nextMusicMode = Game;
        musicChanged = (nextMusicMode != musicMode);
    }
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::P) )
    {
        nextMusicMode = Pause;
        musicChanged = (nextMusicMode != musicMode);
    }
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::I) )
    {
        if ( musicFadeTimer == 0.f )
        {
            stingMode = Iron;
            musicFadeLaunched = true;
        }
    }
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::S) )
    {
        if ( musicFadeTimer == 0.f )
        {
            stingMode = Snare;
            musicFadeLaunched = true;
        }
    }
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::W) )
    {
        if ( musicFadeTimer == 0.f )
        {
            stingMode = Win;
            musicFadeLaunched = true;
        }
    }
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::L) )
    {
        if ( musicFadeTimer == 0.f )
        {
            stingMode = Lose;
            musicFadeLaunched = true;
        }
    }
}

void AudioMusicManager::MusicLoopInit()
{
    if ( !msbMenu.loadFromFile("audio/Music_Loop_Menu.wav") ||
            !msbGame.loadFromFile("audio/Music_Loop_Game.wav") ||
            !msbPause.loadFromFile("audio/Music_Loop_Snare.wav") )
        soundOkay = false;
    mloop.setBuffer(msbMenu);
    mloop.setVolume(MUSIC_LOOP_MAX_VOLUME);
}
void AudioMusicManager::MusicStingInit()
{
    if ( !msbIron.loadFromFile("audio/Music_Sting_Iron.wav") ||
            !msbSnare.loadFromFile("audio/Music_Sting_Snare.wav") ||
            !msbWin.loadFromFile("audio/Music_Sting_Win.wav") ||
            !msbLose.loadFromFile("audio/Music_Sting_Lose.wav") )
        soundOkay = false;
    msting.setVolume(100.f); // stings always full volume
}

// AudioSFXManager implementation

void AudioSFXManager::SFXLoopInit()
{
    if ( !fxbIdle.loadFromFile("audio/SFX_Idle_Loop.wav") ||
            !fxbTurret.loadFromFile("audio/SFX_Turret_Loop.wav") )
        soundOkay = false;
}
void AudioSFXManager::SFXStingInit()
{
    if ( !fxbUIFwd.loadFromFile("audio/SFX_UI_Select_Fwd.wav") ||
            !fxbUIBack.loadFromFile("audio/SFX_UI_Select_Back.wav") ||
            !fxbShot.loadFromFile("audio/SFX_Shot.wav") ||
            !fxbImpact.loadFromFile("audio/SFX_Explosion_Impact.wav") ||
            !fxbKill.loadFromFile("audio/SFX_Explosion_Kill.wav") )
        soundOkay = false;
}
void AudioSFXManager::SFXLoopUpdate( const float& timeDelta )
{
    if ( pIdleEngaged )
    {
        if ( pIdleLoop == -1 )
            pIdleLoop = LaunchSFXLoop( fxbIdle, IDLE_MIN_VOLUME, IDLE_MIN_PITCH );
        else
        {
            pIdleVol += IDLE_SHIFT_SPEED * timeDelta;
            if ( pIdleVol > IDLE_MAX_VOLUME )
                pIdleVol = IDLE_MAX_VOLUME;
            pIdlePitch += IDLE_SHIFT_SPEED * timeDelta;
            if ( pIdlePitch > IDLE_MAX_PITCH )
                pIdlePitch = IDLE_MAX_PITCH;
            TouchSFXLoop( pIdleLoop, pIdleVol, pIdlePitch, false );
        }
    }
    else if ( pIdleLoop > -1 )
    {
        pIdleVol -= IDLE_SHIFT_SPEED * timeDelta;
        if ( pIdleVol < IDLE_MIN_VOLUME )
            pIdleVol = IDLE_MIN_VOLUME;
        pIdlePitch -= IDLE_SHIFT_SPEED * timeDelta;
        if ( pIdlePitch < IDLE_MIN_PITCH )
            pIdlePitch = IDLE_MIN_PITCH;
        TouchSFXLoop( pIdleLoop, pIdleVol, pIdlePitch, false );
    }
    if ( pTurretEngaged )
    {
        if ( pTurretLoop == -1 )
            pTurretLoop = LaunchSFXLoop( fxbTurret, 61.8f, .8f );
    }
    else if ( pTurretLoop > -1 )
    {
        TouchSFXLoop( pTurretLoop, 61.8f, .8f, true );
        pTurretLoop = -1;
    }
}

void AudioSFXManager::LaunchSFXShot()
{
    LaunchSFXSting(fxbShot);
}
void AudioSFXManager::LaunchSFXImpact()
{
    LaunchSFXSting(fxbImpact);
}
void AudioSFXManager::LaunchSFXKill()
{
    LaunchSFXSting(fxbKill);
}

void AudioSFXManager::Testing( const bool& debug, const float& timeDelta )
{
    if ( !debug )
        return;

    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) )
    {
        if ( pIdleLoop == -1 )
            pIdleLoop = LaunchSFXLoop( fxbIdle, IDLE_MIN_VOLUME, IDLE_MIN_PITCH );
        else
        {
            pIdleVol += IDLE_SHIFT_SPEED * timeDelta;
            if ( pIdleVol > IDLE_MAX_VOLUME )
                pIdleVol = IDLE_MAX_VOLUME;
            pIdlePitch += IDLE_SHIFT_SPEED * timeDelta;
            if ( pIdlePitch > IDLE_MAX_PITCH )
                pIdlePitch = IDLE_MAX_PITCH;
            TouchSFXLoop( pIdleLoop, pIdleVol, pIdlePitch, false );
        }
    }
    else if ( pIdleLoop > -1 )
    {
        pIdleVol -= IDLE_SHIFT_SPEED * timeDelta;
        if ( pIdleVol < IDLE_MIN_VOLUME )
            pIdleVol = IDLE_MIN_VOLUME;
        pIdlePitch -= IDLE_SHIFT_SPEED * timeDelta;
        if ( pIdlePitch < IDLE_MIN_PITCH )
            pIdlePitch = IDLE_MIN_PITCH;
        TouchSFXLoop( pIdleLoop, pIdleVol, pIdlePitch, false );
    }
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) )
    {
        if ( pTurretLoop == -1 )
            pTurretLoop = LaunchSFXLoop( fxbTurret, 61.8f, .8f );
    }
    else if ( pTurretLoop > -1 )
    {
        TouchSFXLoop( pTurretLoop, 61.8f, .8f, true );
        pTurretLoop = -1;
    }
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) )
        LaunchSFXSting(fxbUIFwd);
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) )
        LaunchSFXSting(fxbUIBack);
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Num5) )
        LaunchSFXSting(fxbShot);
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Num6) )
        LaunchSFXSting(fxbImpact);
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Num7) )
        LaunchSFXSting(fxbKill);
}
void AudioSFXManager::SFXLoopKill()
{
    if ( pIdleLoop > -1 )
    {
        TouchSFXLoop( pIdleLoop, pIdleVol, pIdlePitch, true );
        pIdleLoop = -1;
    }
    if ( pTurretLoop > -1 )
    {
        TouchSFXLoop( pTurretLoop, 61.8f, .8f, true );
        pTurretLoop = -1;
    }
}

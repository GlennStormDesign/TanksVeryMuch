#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "AudioHandle.h"
#include "TankScene.h"

// Audio Definitions

static AudioMusicManager musicMgr;
static AudioSFXManager sfxMgr;

// AudioMusicManager interface

extern void MusicStingUpdate( const float& timeDelta )
{
    musicMgr.MusicStingUpdate(timeDelta);
}
extern void MusicLoopUpdate( const float& timeDelta )
{
    musicMgr.MusicLoopUpdate(timeDelta);
}
extern void LaunchMusicLoop( const MLoopMode& loop, const bool& interrupt )
{
    musicMgr.LaunchMusicLoop(loop,interrupt);
}
extern void LaunchMusicSting( const MStingMode& sting, const bool& interrupt )
{
    musicMgr.LaunchMusicSting(sting,interrupt);
}

extern void LaunchMusicEnd( const bool& win )
{
    if ( win )
        musicMgr.LaunchMusicSting(Win, false);
    else
        musicMgr.LaunchMusicSting(Lose, false);
    musicMgr.LaunchMusicLoop(Silent, true);
}
extern void MusicTesting( const bool& debug )
{
    musicMgr.Testing(debug);
}

// AudioSFXManager interface

extern int LaunchLoopIdle( const unsigned int& tankID, const float& vol, const float& pitch )
{
    return sfxMgr.LaunchLoopIdle(tankID, vol, pitch);
}
extern int LaunchLoopTurret( const unsigned int& tankID, const float& vol, const float& pitch )
{
    return sfxMgr.LaunchLoopTurret(tankID, vol, pitch);
}
extern void LaunchSFXUIFwd()
{
    sfxMgr.LaunchSFXUIFwd();
}
extern void LaunchSFXUIBack()
{
    sfxMgr.LaunchSFXUIBack();
}
extern void LaunchSFXShot()
{
    sfxMgr.LaunchSFXShot();
}
extern void LaunchSFXShot( const sf::Vector2f& sPos )
{
    sfxMgr.LaunchSFXShot(sPos);
}
extern void LaunchSFXImpact()
{
    sfxMgr.LaunchSFXImpact();
}
extern void LaunchSFXImpact( const sf::Vector2f& sPos )
{
    sfxMgr.LaunchSFXImpact(sPos);
}
extern void LaunchSFXKill()
{
    sfxMgr.LaunchSFXKill();
}
extern void LaunchSFXKill( const sf::Vector2f& sPos )
{
    sfxMgr.LaunchSFXKill(sPos);
}

int LaunchSFXLoop( const sf::SoundBuffer& sb )
{
    if ( !sfxMgr.SafeSFXInterval() )
        return -1;
    int slotIndex = -1;
    for ( int i=0; i<MAX_SFX_LOOPS; i++ ) {
        if ( !fxl[i].getLoop() )
        {
            fxl[i].setBuffer(sb);
            fxl[i].setLoop(true);
            fxl[i].play();
            slotIndex = i;
            break;
        }
    }
    return slotIndex;
}

int LaunchSFXLoop( const sf::SoundBuffer& sb, const float& volume, const float& pitch, const sf::Vector2f& sPos )
{
    if ( !sfxMgr.SafeSFXInterval() )
        return -1;
    int slotIndex = -1;
    for ( int i=0; i<MAX_SFX_LOOPS; i++ ) {
        if ( !fxl[i].getLoop() )
        {
            fxl[i].setBuffer(sb);
            fxl[i].setLoop(true);
            fxl[i].setVolume(volume);
            fxl[i].setPitch(pitch);
            fxl[i].setRelativeToListener(false);
            fxl[i].setMinDistance(61.8f);
            fxl[i].setAttenuation(0.618f);
            fxl[i].setPosition(sPos.x,sPos.y,0.f);
            fxl[i].play();
            slotIndex = i;
            break;
        }
    }
    return slotIndex;
}

void TouchSFXLoop( const int& index, const float& volume, const float& pitch, const sf::Vector2f& sPos, const bool& stop )
{
    fxl[index].setVolume(volume);
    fxl[index].setPitch(pitch);
    fxl[index].setRelativeToListener(false);
    fxl[index].setMinDistance(61.8f);
    fxl[index].setAttenuation(0.618f);
    fxl[index].setPosition(sPos.x,sPos.y,0.f);
    if ( stop )
    {
        fxl[index].setLoop(false);
        fxl[index].stop();
        fxl[index].setPosition(0.f,0.f,0.f);
        fxl[index].setRelativeToListener(true);
        fxl[index].setMinDistance(1.f);
        fxl[index].setAttenuation(1.f);
    }
}

void LaunchSFXSting( const sf::SoundBuffer& sb )
{
    if ( !sfxMgr.SafeSFXInterval() )
        return;
    for ( int i=0; i<MAX_SFX_STINGS; i++ ) {
        if ( fxs[i].getStatus() == sf::Sound::Stopped )
        {
            fxs[i].setBuffer(sb);
            fxs[i].setRelativeToListener(true);
            fxs[i].play();
            break;
        }
    }
}
void LaunchSFXSting( const sf::SoundBuffer& sb, const sf::Vector2f& sPos )
{
    if ( !sfxMgr.SafeSFXInterval() )
        return;
    for ( int i=0; i<MAX_SFX_STINGS; i++ ) {
        if ( fxs[i].getStatus() == sf::Sound::Stopped )
        {
            fxs[i].setBuffer(sb);
            fxs[i].setRelativeToListener(false);
            fxs[i].setMinDistance(38.1f);
            fxs[i].setAttenuation(0.1f);
            fxs[i].setPosition(sPos.x,sPos.y,0.f);
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
    if ( m_musicOkay && mloop.getStatus() != sf::Sound::Playing ) {
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
    if ( !debug || !m_musicOkay )
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
        m_musicOkay = false;
    mloop.setBuffer(msbMenu);
    mloop.setVolume(MUSIC_LOOP_MAX_VOLUME);
    mloop.setRelativeToListener(true);
}
void AudioMusicManager::MusicStingInit()
{
    if ( !msbIron.loadFromFile("audio/Music_Sting_Iron.wav") ||
            !msbSnare.loadFromFile("audio/Music_Sting_Snare.wav") ||
            !msbWin.loadFromFile("audio/Music_Sting_Win.wav") ||
            !msbLose.loadFromFile("audio/Music_Sting_Lose.wav") )
        m_musicOkay = false;
    msting.setVolume(100.f); // stings always full volume
    msting.setRelativeToListener(true);
}

// AudioSFXManager implementation

void AudioSFXManager::SFXLoopInit()
{
    if ( !fxbIdle.loadFromFile("audio/SFX_Idle_Loop.wav") ||
            !fxbTurret.loadFromFile("audio/SFX_Turret_Loop.wav") )
        m_sfxOkay = false;
}
void AudioSFXManager::SFXStingInit()
{
    if ( !fxbUIFwd.loadFromFile("audio/SFX_UI_Select_Fwd.wav") ||
            !fxbUIBack.loadFromFile("audio/SFX_UI_Select_Back.wav") ||
            !fxbShot.loadFromFile("audio/SFX_Shot.wav") ||
            !fxbImpact.loadFromFile("audio/SFX_Explosion_Impact.wav") ||
            !fxbKill.loadFromFile("audio/SFX_Explosion_Kill.wav") )
        m_sfxOkay = false;
}
int AudioSFXManager::LaunchLoopIdle( const unsigned int& tankID, const float& vol, const float& pitch )
{
    // use tankID to assess priority
    Tank t = GetTankByID( tankID );
    return LaunchSFXLoop( fxbIdle, vol, pitch, t.GetBaseSprite().getPosition() );
}
int AudioSFXManager::LaunchLoopTurret( const unsigned int& tankID, const float& vol, const float& pitch )
{
    // use tankID to assess priority
    Tank t = GetTankByID( tankID );
    return LaunchSFXLoop( fxbTurret, vol, pitch, t.GetBaseSprite().getPosition() );
}

bool AudioSFXManager::SafeSFXInterval()
{
    return ( m_sfxStep.restart().asSeconds() > MIN_SFX_INTERVAL );
}
void AudioSFXManager::LaunchSFXUIFwd()
{
    LaunchSFXSting(fxbUIFwd);
}
void AudioSFXManager::LaunchSFXUIBack()
{
    LaunchSFXSting(fxbUIBack);
}
void AudioSFXManager::LaunchSFXShot()
{
    LaunchSFXSting(fxbShot);
}
void AudioSFXManager::LaunchSFXShot( const sf::Vector2f& sPos )
{
    LaunchSFXSting(fxbShot,sPos);
}
void AudioSFXManager::LaunchSFXImpact()
{
    LaunchSFXSting(fxbImpact);
}
void AudioSFXManager::LaunchSFXImpact( const sf::Vector2f& sPos )
{
    LaunchSFXSting(fxbImpact,sPos);
}
void AudioSFXManager::LaunchSFXKill()
{
    LaunchSFXSting(fxbKill);
}
void AudioSFXManager::LaunchSFXKill( const sf::Vector2f& sPos )
{
    LaunchSFXSting(fxbKill,sPos);
}

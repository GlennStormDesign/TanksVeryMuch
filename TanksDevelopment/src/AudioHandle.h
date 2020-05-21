#pragma once

#include <SFML/Audio.hpp>

// Audio Declarations

#define MAX_SFX_LOOPS 8
#define MAX_SFX_STINGS 8

static sf::Sound fxl[MAX_SFX_LOOPS];
static sf::Sound fxs[MAX_SFX_STINGS];
const float MIN_SFX_INTERVAL = 0.05f;

enum MLoopMode { Silent, Menu, Game, Pause };
enum MStingMode { None, Iron, Snare, Win, Lose }; // win and lose change MLoopMode to Silent

const float MUSIC_LOOP_MAX_VOLUME = 61.8f;
const float MUSIC_FADE_DURATION = .381f;

extern void MusicStingUpdate( const float& timeDelta );
extern void MusicLoopUpdate( const float& timeDelta );
extern void LaunchMusicLoop( const MLoopMode& loop, const bool& interrupt );
extern void LaunchMusicSting( const MStingMode& sting, const bool& interrupt );
extern void LaunchMusicEnd( const bool& win );
extern void MusicTesting( const bool& debug );

extern int LaunchLoopIdle( const unsigned int& tankID, const float& vol, const float& pitch );
extern int LaunchLoopTurret( const unsigned int& tankID, const float& vol, const float& pitch );
extern void LaunchSFXUIFwd();
extern void LaunchSFXUIBack();
extern void LaunchSFXShot();
extern void LaunchSFXShot( const sf::Vector2f& sPos );
extern void LaunchSFXImpact();
extern void LaunchSFXImpact( const sf::Vector2f& sPos );
extern void LaunchSFXKill();
extern void LaunchSFXKill( const sf::Vector2f& sPos );

int LaunchSFXLoop( const sf::SoundBuffer& sb );
int LaunchSFXLoop( const sf::SoundBuffer& sb, const float& volume, const float& pitch, const sf::Vector2f& sPos );
void TouchSFXLoop( const int& index, const float& volume, const float& pitch, const sf::Vector2f& sPos, const bool& stop );
void LaunchSFXSting( const sf::SoundBuffer& sb );
void LaunchSFXSting( const sf::SoundBuffer& sb, sf::Vector2f& sPos );

class AudioMusicManager {
public:
    MLoopMode nextMusicMode = Menu;
    bool musicChanged = false;
private:
    // validation
    bool m_musicOkay = true;
    // music loop
    MLoopMode musicMode = Menu;
    sf::SoundBuffer msbMenu, msbGame, msbPause;
    sf::Sound mloop;
    // music sting
    MStingMode stingMode = None;
    bool musicFadeLaunched = false; // fade music loops down upon launch
    float musicFadeTimer = 0.f;
    sf::SoundBuffer msbIron, msbSnare, msbWin, msbLose;
    sf::Sound msting;
public:
    AudioMusicManager()
    {
        MusicLoopInit();
        MusicStingInit();
    }
    ~AudioMusicManager() { }

    void MusicStingUpdate( const float& timeDelta );
    void MusicLoopUpdate( const float& timeDelta );
    void LaunchMusicLoop( const MLoopMode& loop, const bool& interrupt );
    void LaunchMusicSting( const MStingMode& sting, const bool& interrupt );

    void Testing( const bool& debug );
private:
    void MusicLoopInit();
    void MusicStingInit();
};

class AudioSFXManager {
public:
private:
    // validation
    bool m_sfxOkay = true;
    // sfx loop
    sf::SoundBuffer fxbIdle, fxbTurret;
    // sfx sting
    sf::SoundBuffer fxbUIFwd, fxbUIBack, fxbShot, fxbImpact, fxbKill;
    // safety
    sf::Clock m_sfxStep; // keeps from launching multiple sfx, as a minimum interval timer (REVIEW: replace?)
public:
    AudioSFXManager()
    {
        SFXLoopInit();
        SFXStingInit();
    }
    ~AudioSFXManager() { }

    void SFXLoopInit();
    void SFXStingInit();
    int LaunchLoopIdle( const unsigned int& tankID, const float& vol, const float& pitch );
    int LaunchLoopTurret( const unsigned int& tankID, const float& vol, const float& pitch );
    void SFXLoopUpdate( const float& timeDelta );
    bool SafeSFXInterval();
    void LaunchSFXUIFwd();
    void LaunchSFXUIBack();
    void LaunchSFXShot();
    void LaunchSFXShot( const sf::Vector2f& sPos );
    void LaunchSFXImpact();
    void LaunchSFXImpact( const sf::Vector2f& sPos );
    void LaunchSFXKill();
    void LaunchSFXKill( const sf::Vector2f& sPos );
private:
};

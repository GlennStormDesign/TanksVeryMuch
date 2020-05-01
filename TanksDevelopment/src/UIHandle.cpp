#include <SFML/Graphics.hpp>

//#include "FontHandle.h"
#include "ImageHandle.h"
#include "UIHandle.h"

// UI Definitions

static UIManager uiMgr;

// UI interface

sf::Sprite PanelRect( const sf::IntRect& rect )
{
    // returns composed sprite with a 'stretched' texture, aside from the 8x8 corners, based on rect (later: screen space pct)
    //  1) take ui element tex, divide into 9 pieces; four 8x8 corners, four borders and a 16x16 center
    //  2) take minimum size (16x16) from rect and place four corners within
    //  3) place stretched borders vertical and horizontal based on remainder of rect dimensions
    //  4) place stretched center based on border dimensions used
    //  5) make sprite (use sprite from input?) apply composed texture, size sprite to rect, return
    sf::Texture panelTexture = TexUIElement();
    // TODO: see above
    sf::Sprite retSprite;
}

// UIElement implementation

// UIManager implementation

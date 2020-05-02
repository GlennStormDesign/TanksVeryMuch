#include <SFML/Graphics.hpp>

#include "TankCore.h"
#include "ImageHandle.h"
#include "UIHandle.h"

// UI Definitions

static UIManager uiMgr;

// UI interface

sf::Sprite PanelRect( const sf::IntRect& rect )
{
    // REVIEW: need to multiply rect by 1/globalScale before composing?

    // returns composed sprite with a 'stretched' texture, aside from the 8x8 corners, based on rect (later: screen space pct)
    //  1) take ui element tex, divide into 9 pieces; four 8x8 corners, four borders and a 16x16 center
    //  2) take minimum size (16x16) from rect and place four corners within
    //  3) place stretched borders vertical and horizontal based on remainder of rect dimensions
    //  4) place stretched center based on border dimensions used
    //  5) make sprite (use sprite from input?) apply composed texture, size sprite to rect, return
    sf::Sprite piece;
    sf::IntRect segment;
    sf::Vector2f pPos;

    sf::RenderTexture composite;
    sf::ContextSettings settings;
    settings.depthBits = 8;
    composite.create(rect.width, rect.height, settings);

    composite.clear();
    piece.setTexture( TexUIElement() );

    // corners
    segment.height = 8;
    segment.width = 8;
    for ( int x=0; x<32; x+=25 )
    {
        for ( int y=0; y<32; y+=25 )
        {
            segment.left = x;
            // todo: adjust
            segment.top = y;
            // todo: adjust
            piece.setTextureRect( segment );
            // set piece position
            if ( x == 0 )
                pPos.x = 0;
            else
                pPos.x = (rect.width-8);
            if ( y == 0 )
                pPos.y = 0;
            else
                pPos.y = (rect.height-8);
            piece.setPosition( pPos );
            composite.draw(piece);
        }
    }
    // borders
    segment.left = 8;
    segment.width = 16; // todo: handle remainder
    segment.height = 8;
    for ( int x=8; x<(rect.width-24); x+=16 )
    {
        segment.top = 0;
        piece.setTextureRect( segment );
        // set piece position
        pPos.x = x;
        pPos.y = 0;
        piece.setPosition( pPos );
        composite.draw(piece);
        segment.top = 25;
        piece.setTextureRect( segment );
        // set piece position
        pPos.y = (rect.height-8);
        piece.setPosition( pPos );
        composite.draw(piece);
    }
    segment.top = 8;
    segment.width = 8;
    segment.height = 16; // todo: handle remainder
    for ( int y=8; y<(rect.height-24); y+=16 )
    {
        segment.left = 0;
        piece.setTextureRect( segment );
        // set piece position
        pPos.x = 0;
        pPos.y = y;
        piece.setPosition( pPos );
        composite.draw(piece);
        segment.left = 25;
        piece.setTextureRect( segment );
        // set piece position
        pPos.x = (rect.width-8);
        piece.setPosition( pPos );
        composite.draw(piece);
    }
    // center
    segment.left = 8;
    segment.top = 8;
    segment.width = 16;
    segment.height = 16;
    piece.setTextureRect( segment );
    for ( int x=8; x<(rect.width-24); x+=16 )
    {
        for ( int y=8; y<(rect.height-24); y+=16 )
        {
            // set piece position
            pPos.x = x;
            pPos.y = y;
            piece.setPosition( pPos );
            composite.draw(piece);
        }
    }

    sf::Sprite panelSprite;
    panelSprite.setTextureRect( sf::IntRect(0,0,rect.width,rect.height) );

    composite.display();
    panelSprite.setTexture( composite.getTexture() ); // FIXME: no texture from render texture (piece?)
    //panelSprite.setTexture( TexUIElement() ); // testing

    panelSprite.setPosition( rect.left, rect.top );
    panelSprite.setScale(globalScale, globalScale);

    return panelSprite;
}

// UIElement implementation

// UIManager implementation

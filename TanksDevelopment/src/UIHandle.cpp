#include <SFML/Graphics.hpp>

#include "TankCore.h"
#include "ImageHandle.h"
#include "UIHandle.h"

// UI Definitions

static UIManager uiMgr;

// UI interface

sf::Sprite PanelRect( sf::RenderTexture& composite, const sf::IntRect& rect )
{
    // returns composed sprite with a 'stretched' texture, aside from the 8x8 corners, based on rect (later: screen space pct)
    //  1) take ui element tex, divide into 9 pieces; four 8x8 corners, four borders and a 16x16 center
    //  2) take minimum size (16x16) from rect and place four corners within
    //  3) place stretched borders vertical and horizontal based on remainder of rect dimensions
    //  4) place stretched center based on border dimensions used
    //  5) make sprite (use sprite from input?) apply composed texture, size sprite to rect, return
    sf::Sprite piece;
    piece.setTexture( TexUIElement() );
    piece.setScale(2.f,2.f); // hard-coded x2 scale for aesthetics, not using globalScale here
    unsigned short int borderWidth = 8;
    sf::IntRect segment;
    sf::Vector2f pPos;

    composite.create(rect.width, rect.height);

    // corners
    segment.height = borderWidth;
    segment.width = borderWidth;
    for ( int x=0; x<32; x+=25 )
    {
        for ( int y=0; y<32; y+=25 )
        {
            segment.left = x;
            segment.top = y;
            piece.setTextureRect( segment );
            // set piece position
            if ( x == 0 )
                pPos.x = 0;
            else
                pPos.x = (rect.width-(borderWidth*2));
            if ( y == 0 )
                pPos.y = 0;
            else
                pPos.y = (rect.height-(borderWidth*2));
            piece.setPosition( pPos );
            composite.draw(piece);
        }
    }
    // borders
    segment.left = borderWidth;
    segment.width = borderWidth * 2; // todo: handle remainder, if not multiple of borderWidth
    segment.height = borderWidth;
    for ( int x=borderWidth*2; x<(rect.width-(borderWidth*4)); x+=borderWidth*2 )
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
        pPos.y = (rect.height-(borderWidth*2));
        piece.setPosition( pPos );
        composite.draw(piece);
    }
    segment.top = borderWidth;
    segment.width = borderWidth;
    segment.height = borderWidth * 2; // todo: handle remainder, if not multiple of borderWidth
    for ( int y=borderWidth*2; y<(rect.height-(borderWidth*4)); y+=borderWidth*2 )
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
        pPos.x = (rect.width-(borderWidth*2));
        piece.setPosition( pPos );
        composite.draw(piece);
    }
    // center
    segment.left = borderWidth;
    segment.top = borderWidth;
    segment.width = borderWidth * 2;
    segment.height = borderWidth * 2;
    piece.setTextureRect( segment );
    for ( int x=(borderWidth*2); x<(rect.width-(borderWidth*4)); x+=(borderWidth*2) )
    {
        for ( int y=(borderWidth*2); y<(rect.height-(borderWidth*4)); y+=(borderWidth*2) )
        {
            // set piece position
            pPos.x = x;
            pPos.y = y;
            piece.setPosition( pPos );
            composite.draw(piece);
        }
    }

    composite.display();
    sf::Sprite panelSprite( composite.getTexture() );

    panelSprite.setTextureRect( sf::IntRect(0,0,rect.width-1,rect.height-1) );
    panelSprite.setPosition( rect.left, rect.top );

    return panelSprite;
}

// UIElement implementation

void UIElement::UIInit()
{
    // general config
    ElementInit();
    // specific config
}
sf::IntRect& UIElement::GetUIRect()
{
    return m_uiRect;
}
void UIElement::SetUIRect( const sf::IntRect& pos )
{
    m_uiRect = pos;
}
sf::Color& UIElement::GetUIColor()
{
    return m_uiColor;
}
void UIElement::SetUIColor( const sf::Color& col )
{
    m_uiColor = col;
}
unsigned int& UIElement::GetLayerOrder()
{
    return m_uiLayer;
}
void UIElement::SetLayerOrder( const unsigned int& layerOrder )
{
    m_uiLayer = layerOrder;
}

void UIElement::UIUpdate( const float& timeDelta ) { } // define in subclasses
void UIElement::DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset ) { } // define in subclasses

// UIField implementation
void UIField::ElementInit()
{
    //
}
void UIField::UIUpdate( const float& timeDelta )
{
    if ( !active )
        return;

    //
}
void UIField::DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset )
{
    if ( !visible )
        return;

    sf::RectangleShape s;
    s.setPosition( sf::Vector2f( uiOffset.x + m_uiRect.left, uiOffset.y + m_uiRect.top ) );
    s.setSize( sf::Vector2f( m_uiRect.width, m_uiRect.height ) );
    s.setFillColor( m_uiColor );
    window.draw( s );
}

// UIFrame implementation

void UIFrame::ElementInit()
{
    //
}

void UIFrame::UIUpdate( const float& timeDelta )
{
    if ( !active )
        return;

    //
}
void UIFrame::DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset )
{
    if ( !visible )
        return;

    sf::RenderTexture rt;
    sf::Sprite s = PanelRect( rt, m_uiRect );
    s.setPosition( uiOffset.x + m_uiRect.left, uiOffset.y + m_uiRect.top );
    s.setColor( m_uiColor );
    window.draw( s );
}

// UILabel implementation

void UILabel::ElementInit()
{
    //
}

void UILabel::UIUpdate( const float& timeDelta )
{
    if ( !active )
        return;

    //
}
void UILabel::DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset )
{
    if ( !visible )
        return;

    sf::Text t;
    m_heading.InitHeadingText(t);
    t.setString(m_string);
    sf::Vector2f tOffset;
    sf::Color fillColorTmp = t.getFillColor();
    if ( m_dropShadow )
    {
        if ( m_heading.GetAlignment() == Center )
            tOffset.x = ( m_uiRect.width / 2 ) - ( t.getLocalBounds().width / 2 );
        else if ( m_heading.GetAlignment() == Left )
            tOffset.x = ( m_heading.GetFontSpace().x * 8.f );
        else if ( m_heading.GetAlignment() == Right )
            tOffset.x = m_uiRect.width - t.getLocalBounds().width - ( m_heading.GetFontSpace().x * 9.f );
        tOffset.y = ( m_uiRect.height / 2 ) - ( t.getLocalBounds().height / 2 ) - ( m_heading.GetFontSpace().y / 2 );
        t.setPosition( uiOffset.x + m_uiRect.left + tOffset.x, uiOffset.y + m_uiRect.top + tOffset.y );
        t.setFillColor(m_heading.GetShadowColor());
        window.draw(t);
        t.setFillColor(fillColorTmp);
    }
    if ( m_heading.GetAlignment() == Center )
        tOffset.x = ( m_uiRect.width / 2 ) - ( t.getLocalBounds().width / 2 ) - m_heading.GetFontSpace().x;
    else if ( m_heading.GetAlignment() == Left )
        tOffset.x = ( m_heading.GetFontSpace().x * 7.f );
    else if ( m_heading.GetAlignment() == Right )
        tOffset.x = m_uiRect.width - t.getLocalBounds().width - ( m_heading.GetFontSpace().x * 10.f );
    tOffset.y = ( m_uiRect.height / 2 ) - ( t.getLocalBounds().height / 2 ) - m_heading.GetFontSpace().y;
    t.setPosition( uiOffset.x + m_uiRect.left + tOffset.x, uiOffset.y + m_uiRect.top + tOffset.y );
    window.draw( t );
}

// UIIcon implementation

void UIIcon::ElementInit()
{
    //
}

void UIIcon::UIUpdate( const float& timeDelta )
{
    if ( !active )
        return;

    //
}
void UIIcon::DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset )
{
    if ( !visible )
        return;

    sf::Sprite s;
    s.setTexture( m_texture );
    s.setScale( m_scaleFactor, m_scaleFactor );
    sf::Vector2f iOffset( (m_uiRect.width - s.getLocalBounds().width)/2, (m_uiRect.height - s.getLocalBounds().height)/2 );
    iOffset.x -= s.getLocalBounds().width/2;
    iOffset.y -= s.getLocalBounds().height/2;
    s.setPosition( uiOffset.x + m_uiRect.left + iOffset.x, uiOffset.y + m_uiRect.top + iOffset.y );
    s.setColor( m_uiColor );
    window.draw( s );
}

// UIButton implementation

void UIButton::ElementInit()
{
    //
}

const ButtonState& UIButton::GetState()
{
    return m_state;
}
void UIButton::SetState( const ButtonState& state )
{
    m_state = state;
}

void UIButton::UIUpdate( const float& timeDelta )
{
    if ( !active )
        return;

    //
}
void UIButton::DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset )
{
    if ( !visible )
        return;

    // dynamic values
    sf::IntRect uir = m_uiRect;
    sf::Color uic = m_uiColor;

    if ( GetState() == Disabled )
        uic.a /= 4;
    else
    {
        SetState(Normal);
        if ( window.hasFocus() )
        {
            // detect hover
            sf::Vector2i mp = sf::Mouse::getPosition(window);
            if ( uir.contains(mp) )
            {
                uic = sf::Color(128,255,128,255);
                SetState(Hover);
                // detect mouse click
                if ( sf::Mouse::isButtonPressed(sf::Mouse::Left) )
                {
                    uic = sf::Color(64,128,64,255);
                    uir.left += 2;
                    uir.top += 2;
                    uir.width -= 4;
                    uir.height -= 4;
                    SetState(Active);
                }
            }
        }
    }

    sf::RenderTexture rt;
    sf::Sprite p = PanelRect( rt, uir );
    p.setPosition( uir.left + uiOffset.x, uir.top + uiOffset.y );
    p.setColor( uic );
    window.draw( p );
    // overlay button label
    // REVIEW: look into adjusting rect or color of label in relation to button (overload label with RenderTexture?)
    m_buttonLabel.SetUIRect( uir );
    m_buttonLabel.DrawUI(window,uiOffset);
}

// UIBox implementation

void UIBox::ElementInit()
{
    //
}

void UIBox::UIUpdate( const float& timeDelta )
{
    if ( !active )
        return;

    //
}
void UIBox::DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset )
{
    if ( !visible )
        return;

    UIFrame bf(m_uiRect, m_uiColor);
    sf::IntRect boxItemR = m_uiRect;
    boxItemR.top = m_uiRect.top + 8;
    boxItemR.height *= 0.381f;
    int itemSpacing = boxItemR.height;
    PanelTitle ft;
    UILabel bt( boxItemR, sf::Color::White, ft, true, m_boxTitle );
    PanelLabel fd;
    boxItemR.top = m_uiRect.top + (m_uiRect.height / 2) - (boxItemR.height / 2);
    UILabel bd( boxItemR, sf::Color::White, fd, false, m_boxMessage );
    // NOTE: could use a measure of local bounds for the text item within label to help with formatting here
    boxItemR.height = 64;
    boxItemR.top = m_uiRect.top + m_uiRect.height - boxItemR.height - 8;
    boxItemR.width /= 4;
    boxItemR.left += ( ( m_uiRect.width / 2 ) - ( boxItemR.width / 2 ) );
    ClearLargeHeading fb;
    UILabel bl( boxItemR, sf::Color::White, fb, false, m_boxButtonLabel );
    UIButton bb( boxItemR, sf::Color::White, bl );

    bf.DrawUI(window,uiOffset);
    bt.DrawUI(window,uiOffset);
    bd.DrawUI(window,uiOffset);
    bb.DrawUI(window,uiOffset);
}

// UIAlert implementation

void UIAlert::ElementInit()
{
    //
}

void UIAlert::UIUpdate( const float& timeDelta )
{
    if ( !active )
        return;

    //
}
void UIAlert::DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset )
{
    if ( !visible )
        return;

    //
}

// UIConfirm implementation

void UIConfirm::ElementInit()
{
    //
}

void UIConfirm::UIUpdate( const float& timeDelta )
{
    if ( !active )
        return;

    //
}
void UIConfirm::DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset )
{
    if ( !visible )
        return;

    //
}

// UIHUD implementation

void UIHUD::ElementInit()
{
    //
}

void UIHUD::UIUpdate( const float& timeDelta )
{
    if ( !active )
        return;

    //
}
void UIHUD::DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset )
{
    if ( !visible )
        return;

    //
}

// UIMenu implementation

void UIMenu::ElementInit()
{
    //
}

void UIMenu::UIUpdate( const float& timeDelta )
{
    if ( !active )
        return;

    //
}
void UIMenu::DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset )
{
    if ( !visible )
        return;

    //
}

// UIManager implementation

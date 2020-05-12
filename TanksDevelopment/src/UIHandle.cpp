#include <SFML/Graphics.hpp>

#include "TankCore.h"
#include "ImageHandle.h"
#include "UIHandle.h"

// UI Definitions

static UIManager uiMgr;

// UI Manager interface

extern void UIInit( const sf::Vector2u& winSize )
{
    uiMgr.UIInit(winSize);
}
extern void UpdateUI( const float& timeDelta )
{
    uiMgr.UpdateUIMgr(timeDelta);
}
extern void DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset )
{
    uiMgr.DrawUIMgr(window,uiOffset);
}

// PanelRect method

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

std::string UILabel::GetString()
{
    return m_string;
}
void UILabel::SetString( const std::string& s )
{
    m_string = s;
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

int UIBox::GetCallBack()
{
    int retSignal = m_callBackSignal;
    m_callBackSignal = 0; // reset (REVIEW: why need reset each frame?)
    return retSignal;
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

    m_callBackSignal = 0;

    UIFrame bf(m_uiRect, m_uiColor);
    sf::IntRect boxItemR = m_uiRect;
    boxItemR.top = m_uiRect.top + 8;
    boxItemR.height *= 0.381f;
    int itemSpacing = boxItemR.height;
    PanelTitle ft;
    UILabel bt( boxItemR, sf::Color::White, ft, true, m_boxTitle );
    PanelLabel fd;
    boxItemR.top = m_uiRect.top + (m_uiRect.height / 2) - (boxItemR.height / 2);
    m_messageLabel = UILabel( boxItemR, sf::Color::White, fd, false, m_boxMessage );
    // NOTE: could use a measure of local bounds for the text item within label to help with formatting here
    boxItemR.height = 64;
    boxItemR.top = m_uiRect.top + m_uiRect.height - boxItemR.height - 8;
    boxItemR.width /= 4;
    boxItemR.left += ( ( m_uiRect.width / 2 ) - ( boxItemR.width / 2 ) );
    ClearLargeHeading fb;
    UILabel bl( boxItemR, sf::Color::White, fb, false, m_boxButtonLabel );
    m_boxButton = UIButton( boxItemR, m_uiColor, bl );

    bf.DrawUI(window,uiOffset);
    bt.DrawUI(window,uiOffset);
    m_messageLabel.DrawUI(window,uiOffset);
    m_boxButton.DrawUI(window,uiOffset);

    if ( m_boxButton.GetState() == Active )
        m_callBackSignal = 1;
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

    m_callBackSignal = 0;

    UIFrame bf(m_uiRect, m_uiColor);
    sf::IntRect boxItemR = m_uiRect;
    boxItemR.top = m_uiRect.top + 8;
    boxItemR.height *= 0.381f;
    int itemSpacing = boxItemR.height;
    PanelTitle ft;
    UILabel bt( boxItemR, sf::Color::White, ft, true, m_boxTitle );
    PanelLabel fd;
    boxItemR.top = m_uiRect.top + (m_uiRect.height / 2) - (boxItemR.height / 2);
    m_messageLabel = UILabel( boxItemR, sf::Color::White, fd, false, m_boxMessage );
    // NOTE: could use a measure of local bounds for the text item within label to help with formatting here
    boxItemR.height = 64;
    boxItemR.top = m_uiRect.top + m_uiRect.height - boxItemR.height - 8;
    boxItemR.width /= 4;
    boxItemR.left += ( ( m_uiRect.width / 2 ) - ( boxItemR.width / 2 ) );
    ClearLargeHeading fb;
    UILabel bl( boxItemR, sf::Color::White, fb, false, m_boxButtonLabel );
    m_boxButton = UIButton( boxItemR, m_uiColor, bl );

    bf.DrawUI(window,uiOffset);
    bt.DrawUI(window,uiOffset);
    m_messageLabel.DrawUI(window,uiOffset);
    m_boxButton.DrawUI(window,uiOffset);

    if ( m_boxButton.GetState() == Active )
        m_callBackSignal = 1;
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

    m_callBackSignal = 0;

    UIFrame bf(m_uiRect, m_uiColor);
    sf::IntRect boxItemR = m_uiRect;
    boxItemR.top = m_uiRect.top + 8;
    boxItemR.height *= 0.381f;
    int itemSpacing = boxItemR.height;
    PanelTitle ft;
    UILabel bt( boxItemR, sf::Color::White, ft, true, m_boxTitle );
    PanelLabel fd;
    boxItemR.top = m_uiRect.top + (m_uiRect.height / 2) - (boxItemR.height / 2);
    m_messageLabel = UILabel( boxItemR, sf::Color::White, fd, false, m_boxMessage );
    // NOTE: could use a measure of local bounds for the text item within label to help with formatting here
    boxItemR.height = 64;
    boxItemR.top = m_uiRect.top + m_uiRect.height - boxItemR.height - 8;

    bf.DrawUI(window,uiOffset);
    bt.DrawUI(window,uiOffset);
    m_messageLabel.DrawUI(window,uiOffset);

    // format multiple buttons
    boxItemR.width = (m_uiRect.width * 0.275f );
    ClearLargeHeading fb;
    boxItemR.left = m_uiRect.left;
    boxItemR.left += (m_uiRect.width / 16);
    UILabel bl( boxItemR, sf::Color::White, fb, false, m_boxButtonLabel );
    m_boxButton = UIButton( boxItemR, m_uiColor, bl );
    m_boxButton.DrawUI(window,uiOffset);
    if ( m_boxButton.GetState() == Active )
        m_callBackSignal = 1;
    if ( m_altButtonLabel != "" )
    {
        boxItemR.left = m_uiRect.left;
        boxItemR.left += ( ( m_uiRect.width / 2 ) - ( boxItemR.width / 2 ) );
        bl = UILabel( boxItemR, sf::Color::White, fb, false, m_altButtonLabel );
        m_altButton = UIButton( boxItemR, m_uiColor, bl );
        m_altButton.DrawUI(window,uiOffset); // REVIEW: do ui elment visible = false instead?
        if ( m_altButton.GetState() == Active )
            m_callBackSignal = 2;
    }
    boxItemR.left = m_uiRect.left + m_uiRect.width - boxItemR.width;
    boxItemR.left -= (m_uiRect.width / 16);
    bl = UILabel( boxItemR, sf::Color::White, fb, false, m_cancelButtonLabel );
    m_cancelButton = UIButton( boxItemR, m_uiColor, bl );
    m_cancelButton.DrawUI(window,uiOffset);
    if ( m_cancelButton.GetState() == Active )
        m_callBackSignal = 3;
}

// UIProgressBar implementation

void UIProgressBar::ElementInit()
{
    //
}

float UIProgressBar::SetProgress( const float& progress )
{
    m_progress = progress;
    // clamp between 0 and 1
    if ( m_progress < 0.f )
        m_progress = 0.f;
    else if ( m_progress > 1.f )
        m_progress = 1.f;
}

void UIProgressBar::UIUpdate( const float& timeDelta )
{
    if ( !active )
        return;

    //
}
void UIProgressBar::DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset )
{
    if ( !visible )
        return;

    UIField pf( m_uiRect, m_backgroundColor );
    pf.DrawUI( window, uiOffset );
    sf::IntRect pr = m_uiRect;
    pr.top += m_border;
    pr.left += m_border;
    pr.width -= m_border * 2;
    pr.height -= m_border * 2;
    pr.width *= m_progress;
    if ( m_toLeft )
        pr.left = m_uiRect.left + m_uiRect.width - pr.width;
    pf = UIField( pr, m_uiColor );
    pf.DrawUI( window, uiOffset );
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

void UIManager::UIInit( const sf::Vector2u& winSize )
{
    m_windowSize = winSize;
    SplashInit();
    HUDInit();
    MenuInit();
    m_uiStateTimer.restart();
}

void UIManager::SplashInit()
{
    m_splashField = UIField( sf::IntRect(0,0,(m_windowSize.x),(m_windowSize.y)), sf::Color(128,128,100,255) );
    MainTitle sf;
    m_splashTitleLabel = UILabel( sf::IntRect(0,(m_windowSize.y/8)*1.f,(m_windowSize.x),(m_windowSize.y/6)), sf::Color::White, sf, false, "TANKS VERY MUCH" );
    PanelTitle stf;
    m_splashSubtitleLabel = UILabel( sf::IntRect(0,(m_windowSize.y/8)*3.f,(m_windowSize.x),(m_windowSize.y/6)), sf::Color::White, stf, true, "Gameplay Prototype" );
    PanelTitle sbf;
    UILabel sbl = UILabel( sf::IntRect( sf::IntRect((m_windowSize.x/4)*1.5f,(m_windowSize.y/8)*5.f,(m_windowSize.x/4),(m_windowSize.y/6)) ), sf::Color::White, sbf, true, "START" );
    m_splashStartButton = UIButton( sf::IntRect((m_windowSize.x/4)*1.5f,(m_windowSize.y/8)*5.f,(m_windowSize.x/4),(m_windowSize.y/6)), sf::Color(128,200,100,255), sbl );
    m_splashStartButton.SetState(Disabled);
    // temp
    for ( int i=1; i<4; i++ )
        GetTank(i).SetActiveState(false);
}
void UIManager::MenuInit()
{
    m_menuField = UIField( sf::IntRect(0,0,(m_windowSize.x),(m_windowSize.y)), sf::Color(128,128,100,255) );
    m_menuFrame = UIFrame( sf::IntRect((m_windowSize.x*0.05f),(m_windowSize.y*0.05f),(m_windowSize.x*0.9f),(m_windowSize.y*0.9f)), sf::Color(128,100,64,255) );
    MainTitle mf;
    m_menuTitleLabel = UILabel( sf::IntRect(0,(m_windowSize.y/8)*1.f,(m_windowSize.x),(m_windowSize.y/8)), sf::Color::White, mf, false, "TANKS VERY MUCH" );
    ClearLargeHeading stf;
    m_menuSubtitleLabel = UILabel( sf::IntRect(0,(m_windowSize.y/8)*2.f,(m_windowSize.x),(m_windowSize.y/8)), sf::Color::White, stf, false, "Gameplay Prototype - Pre Alpha - Proof of Concept" );
    m_menuFootnoteLabel = UILabel( sf::IntRect(0,(m_windowSize.y)*0.85f,(m_windowSize.x),(m_windowSize.y/8)), sf::Color::White, stf, false, "Spring 2020 Glenn Storm at Hot Iron Productions" );
    PanelTitle mbf;
    UILabel mbl = UILabel( sf::IntRect((m_windowSize.x/4)*1.5f,(m_windowSize.y/8)*3.f,(m_windowSize.x/4),(m_windowSize.y/8)), sf::Color::White, mbf, true, "How To" );
    m_menuHowToButton = UIButton( sf::IntRect((m_windowSize.x/4)*1.5f,(m_windowSize.y/8)*3.f,(m_windowSize.x/4),(m_windowSize.y/8)), sf::Color(128,200,100,255), mbl );
    mbl.SetString("Play");
    m_menuPlayButton = UIButton( sf::IntRect((m_windowSize.x/4)*1.5f,(m_windowSize.y/8)*4.f,(m_windowSize.x/4),(m_windowSize.y/8)), sf::Color(128,200,100,255), mbl );
    mbl.SetString("Credits");
    m_menuCreditsButton = UIButton( sf::IntRect((m_windowSize.x/4)*1.5f,(m_windowSize.y/8)*5.f,(m_windowSize.x/4),(m_windowSize.y/8)), sf::Color(128,200,100,255), mbl );
    mbl.SetString("Quit");
    m_menuQuitButton = UIButton( sf::IntRect((m_windowSize.x/4)*1.5f,(m_windowSize.y/8)*6.f,(m_windowSize.x/4),(m_windowSize.y/8)), sf::Color(128,200,100,255), mbl );

    m_menuHowToButton.SetState(Disabled);
    m_menuPlayButton.SetState(Disabled);
    m_menuCreditsButton.SetState(Disabled);
    m_menuQuitButton.SetState(Disabled);

    m_menuHowToPop = UIAlert( sf::IntRect( (m_windowSize.x/4), (m_windowSize.y/4), m_windowSize.x*0.5f, m_windowSize.y*0.5f ), sf::Color(128,200,64,255), "How To Play", "Arrow keys move\nBrackets turn the turret\nBackslash fires", "OK" );
    m_menuCreditsPop = UIAlert(  sf::IntRect( (m_windowSize.x/4), (m_windowSize.y/4), m_windowSize.x*0.5f, m_windowSize.y*0.5f ), sf::Color(128,200,64,255), "Credits", "Art, Design, Programming\nGlenn Storm at\nHot Iron Productions", "OK" );

    m_menuHowToPop.visible = false;
    m_menuCreditsPop.visible = false;
}
void UIManager::HUDInit()
{
    //  . game title banner
    m_gameTitleFrame = UIFrame( sf::IntRect((m_windowSize.x - 256 - 8),8,256,64), sf::Color(128,200,64,128) );
    PanelLabel gtl;
    m_gameTitleLabel = UILabel( sf::IntRect((m_windowSize.x - 256 - 8),8,256,64), sf::Color::Black, gtl, true, "Tanks Very Much" );
    //  . progress bar for armor, with label
    m_armorProgressFrame = UIFrame( sf::IntRect(8,8,256,64), sf::Color(128,200,64,128) );
    m_armorProgressBar = UIProgressBar( sf::IntRect(18,18,236,20), sf::Color(200,32,32,255), sf::Color(64,64,64,128), false, 4 );
    ClearSmallHeading af;
    m_armorProgressLabel = UILabel( sf::IntRect(8,40,256,24), sf::Color::White, af, true, "ARMOR" );
    //  . enemy tanks remaining label and number count
    std::string tanksRemainString = "Enemy Tanks ";
    HUDLabelSemi trf;
    m_enemyTanksLabel = UILabel( sf::IntRect(8,(m_windowSize.y - 64 - 8),256,64), sf::Color::White, trf, true, tanksRemainString );
    //  . quit button (launches quit confirm popup)
    ToolipHeading qbf;
    UILabel ql( sf::IntRect((m_windowSize.x - 64 -8),(m_windowSize.y - 32 - 8),64,32), sf::Color::Black, qbf, true, "Exit" );
    m_quitButton = UIButton( sf::IntRect((m_windowSize.x - 64 -8),(m_windowSize.y - 32 - 8),64,32), sf::Color(128,200,64,128), ql );

    // tutorial pops

    // win/lose banners
    MainTitle wlf;
    m_winBanner = UILabel( sf::IntRect((m_windowSize.x/2-256),(m_windowSize.y/2-128),512,128), sf::Color::White, wlf, true, "YOU WIN" );
    m_loseBanner = UILabel( sf::IntRect((m_windowSize.x/2-256),(m_windowSize.y/2-128),512,128), sf::Color::White, wlf, true, "GAME OVER" );

    // quit confirm
    m_quitPop = UIConfirm( sf::IntRect((m_windowSize.x/2-256),(m_windowSize.y/2+32),512,192),sf::Color(128,200,64,128), "Exit", "You are about to exit the game", "Ok", "", "Cancel" );
}

const UIManager::UIState& UIManager::GetUIState()
{
    return m_uiState;
}
void UIManager::SetUIState( const UIManager::UIState& state )
{
    if ( m_uiState != state )
    {
        if ( m_uiState == Splash )
            ResetSplash();
        else if ( m_uiState == Menu )
            ResetMenu();
        else if ( m_uiState == Game )
            ResetHUD();
    }
    m_uiStateTimer.restart();
    m_uiState = state;
}
int UIManager::GetInputCallBack()
{
    int tmp = m_inputCallback;
    m_inputCallback = 0; // reset
    return tmp;
}

void UIManager::ResetSplash()
{
    //
    m_splashStartButton.SetState(Disabled);
    for ( int i=1; i<4; i++ )
        GetTank(i).SetActiveState(false);
}
void UIManager::ResetMenu()
{
    //
    m_menuHowToButton.SetState(Disabled);
    m_menuPlayButton.SetState(Disabled);
    m_menuCreditsButton.SetState(Disabled);
    m_menuQuitButton.SetState(Disabled);
    for ( int i=1; i<4; i++ )
        GetTank(i).SetActiveState(false);
}
void UIManager::ResetHUD()
{
    //
}

void UIManager::UpdateUIMgr( const float& timeDelta )
{
    // update based on ui state
    if ( m_uiState == Splash )
        UpdateSplash( timeDelta );
    else if ( m_uiState == Menu )
        UpdateMenu( timeDelta );
    else if ( m_uiState == Game )
        UpdateHUD( timeDelta );
}
void UIManager::DrawUIMgr( sf::RenderWindow& window, const sf::Vector2f& uiOffset )
{
    // draw based on ui state
    if ( m_uiState == Splash )
        DrawSplash( window, uiOffset );
    else if ( m_uiState == Menu )
        DrawMenu( window, uiOffset );
    else if ( m_uiState == Game )
        DrawHUD( window, uiOffset );
}

void UIManager::UpdateSplash( const float& timeDelta )
{
    if ( m_uiStateTimer.getElapsedTime().asSeconds() > m_UI_STATE_TIMER_INTERVAL && m_splashStartButton.GetState() == Disabled )
    {
        m_splashStartButton.SetState(Normal);
    }
}
void UIManager::UpdateMenu( const float& timeDelta )
{
    if ( m_uiStateTimer.getElapsedTime().asSeconds() > m_UI_STATE_TIMER_INTERVAL && m_splashStartButton.GetState() == Disabled )
    {
        m_menuHowToButton.SetState(Normal);
        m_menuPlayButton.SetState(Normal);
        m_menuCreditsButton.SetState(Normal);
        m_menuQuitButton.SetState(Normal);
    }
}
void UIManager::UpdateHUD( const float& timeDelta )
{
    //
}

void UIManager::DrawSplash( sf::RenderWindow& window, const sf::Vector2f& uiOffset )
{
    //
    m_splashField.DrawUI(window,uiOffset);
    m_splashTitleLabel.DrawUI(window,uiOffset);
    m_splashSubtitleLabel.DrawUI(window,uiOffset);
    m_splashStartButton.DrawUI(window,uiOffset);
    if ( m_splashStartButton.GetState() == Active )
    {
        LaunchSFXUIFwd();
        SetUIState(Menu);
        LaunchMusicLoop((MLoopMode)Menu,true);
    }
}
void UIManager::DrawMenu( sf::RenderWindow& window, const sf::Vector2f& uiOffset )
{
    //
    m_menuField.DrawUI(window,uiOffset);
    m_menuFrame.DrawUI(window,uiOffset);
    m_menuTitleLabel.DrawUI(window,uiOffset);
    m_menuSubtitleLabel.DrawUI(window,uiOffset);
    m_menuFootnoteLabel.DrawUI(window,uiOffset);
    m_menuHowToButton.DrawUI(window,uiOffset);
    m_menuPlayButton.DrawUI(window,uiOffset);
    m_menuCreditsButton.DrawUI(window,uiOffset);
    m_menuQuitButton.DrawUI(window,uiOffset);
    m_menuHowToPop.DrawUI(window,uiOffset);
    m_menuCreditsPop.DrawUI(window,uiOffset);
    if ( m_uiInputTimer.getElapsedTime().asSeconds() > m_UI_INPUT_TIMER_MIN && m_menuHowToButton.GetState() == Active )
    {
        LaunchSFXUIFwd();
        m_menuHowToButton.visible = false;
        m_menuPlayButton.SetState(Disabled);
        m_menuCreditsButton.SetState(Disabled);
        m_menuQuitButton.SetState(Disabled);
        m_menuHowToPop.visible = true;
        m_uiInputTimer.restart();
    }
    if ( m_uiInputTimer.getElapsedTime().asSeconds() > m_UI_INPUT_TIMER_MIN && m_menuHowToPop.GetCallBack() == 1 )
    {
        LaunchSFXUIBack();
        m_menuHowToPop.visible = false;
        m_menuHowToButton.visible = true;
        m_uiInputTimer.restart();
    }
    if ( m_uiInputTimer.getElapsedTime().asSeconds() > m_UI_INPUT_TIMER_MIN && m_menuPlayButton.GetState() == Active )
    {
        LaunchSFXUIFwd();
        SetUIState(Game);
        for ( int i=0; i<4; i++ )
        {
            GetTank(i).SetActiveState(true);
            GetTank(i).TankReset();
        }
        LaunchMusicLoop((MLoopMode)Game,true);
    }
    if ( m_uiInputTimer.getElapsedTime().asSeconds() > m_UI_INPUT_TIMER_MIN && m_menuCreditsButton.GetState() == Active )
    {
        LaunchSFXUIFwd();
        m_menuCreditsButton.visible = false;
        m_menuHowToButton.SetState(Disabled);
        m_menuPlayButton.SetState(Disabled);
        m_menuQuitButton.SetState(Disabled);
        m_menuCreditsPop.visible = true;
        m_uiInputTimer.restart();
    }
    if ( m_uiInputTimer.getElapsedTime().asSeconds() > m_UI_INPUT_TIMER_MIN && m_menuCreditsPop.GetCallBack() == 1 )
    {
        LaunchSFXUIBack();
        m_menuCreditsPop.visible = false;
        m_menuCreditsButton.visible = true;
        m_uiInputTimer.restart();
    }
    if ( m_uiInputTimer.getElapsedTime().asSeconds() > m_UI_INPUT_TIMER_MIN && m_menuQuitButton.GetState() == Active )
        window.close();
}
void UIManager::DrawHUD( sf::RenderWindow& window, const sf::Vector2f& uiOffset )
{
    // TODO: use render texture and combine to one draw call
    //
    m_gameTitleFrame.DrawUI(window,uiOffset);
    m_gameTitleLabel.DrawUI(window,uiOffset);
    //
    m_armorProgressFrame.DrawUI(window,uiOffset);
    m_armorProgressBar.SetProgress( (GetLocalPlayerTank().GetArmor() / 100.f) );
    m_armorProgressBar.DrawUI(window,uiOffset);
    m_armorProgressLabel.DrawUI(window,uiOffset);
    //
    std::string tanksRemainString = "Enemy Tanks ";
    int t = GetActiveTankCount();
    if ( GetLocalPlayerTank().GetActiveState() )
        t--;
    tanksRemainString += FormatInt( t );
    HUDLabelSemi trf;
    m_enemyTanksLabel = UILabel( sf::IntRect(8,(m_windowSize.y - 64 - 8),256,64), sf::Color::White, trf, true, tanksRemainString );
    m_enemyTanksLabel.DrawUI(window,uiOffset);
    //
    m_quitButton.DrawUI(window,uiOffset);
    if ( m_quitButton.GetState() == Active )
    {
        LaunchSFXUIFwd();
        m_displayQuit = true;
        m_quitButton.SetState(Disabled);
    }

    // tutorial pops

    // win/lose banners
    if ( GetLocalPlayerTank().GetActiveState() )
    {
        if ( GetActiveTankCount() == 1 )
            m_winBanner.DrawUI(window,uiOffset);
    }
    else
        m_loseBanner.DrawUI(window,uiOffset);

    // quit confirm
    if ( m_displayQuit )
        m_quitPop.DrawUI(window,uiOffset);

    m_inputCallback = m_quitPop.GetCallBack();
    if ( m_inputCallback == 1 )
    {
        LaunchSFXUIBack();
        SetUIState(Menu);
        for ( int i=1; i<4; i++ )
            GetTank(i).SetActiveState(false);
        LaunchMusicLoop((MLoopMode)Menu,true);
        // GAME RESET (temp)
        GetLocalPlayerTank().SetPosition(512.f,512.f);
        GetLocalPlayerTank().SetBaseRotation(0.f);
        GetLocalPlayerTank().SetTurretRotation(0.f);
        GetLocalPlayerTank().SetArmor(100.f);
        SFXLoopKill();
        for ( int i=1; i<4; i++ )
        {
            GetTank(i).SetPosition((200.f+(i*150.f)),300.f);
            GetTank(i).SetBaseRotation( rand() % 360 );
            GetTank(i).SetTurretRotation(0.f);
        }
        m_quitButton.SetState(Normal);
        m_displayQuit = false;
        m_inputCallback = 0;
    }
    else if ( m_inputCallback == 3 )
    {
        LaunchSFXUIBack();
        m_quitButton.SetState(Normal);
        m_displayQuit = false;
        m_inputCallback = 0;
    }
}

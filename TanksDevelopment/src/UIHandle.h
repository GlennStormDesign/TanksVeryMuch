#pragma once

#include "SFML/Graphics.hpp"

#include "FontHandle.h"

// UI Element Handling

// NICETOHAVE: UIElement manager class animates position, adds vfx, animated image
// NICETOHAVE: drop shadow option for labels (shadow color drawn first with only half of vertical 'center offset', no h offset)

// NOTE: all UI Elements are meant to be drawn in an order (layers), and all have component layers within them
// NOTE: movement of UI Elements is an integral factor (position, scale, color / fading) to accent change
// NOTE: buttons have sfx associated (button fwd or button back)

// NICETOHAVE: ideally, minimal formatting information would be required, and UI rules would dictate size, position, etc
//  potential rules:
//    labels are as big as the label text needs, therefore button is as big as label needs to be, etc
//    aside from movement of UI Elements, their positions will be grouped and centered
//    timing of movement staggered by layer order (back to front) with standard delay, speed, etc

// NOTE: follow pattern for polymorphism used with SceneObject ( functionality modules -> base class -> subclasses )
// NOTE: follow pattern for manager used with others ( static instance defined in cpp, extern functions interface project )

sf::Sprite PanelRect( sf::RenderTexture& rt, const sf::IntRect& rect );

class UIElement {
public:
    bool active = false; // use to skip update
    bool visible = true; // use to skip draw
protected:
	sf::IntRect m_uiRect = sf::IntRect(0,0,0,0); // left and top relative to view pos x,y
	sf::Color m_uiColor = sf::Color::White; // primary color (foreground)
	unsigned int m_uiLayer = 0; // ui elements drawn last; ui depth layer sorts draw; - = behind, + = in front
public:
    UIElement() { UIInit(); }
    ~UIElement() { }

    virtual void ElementInit() { } // define in subclasses

    void UIInit();

    sf::IntRect& GetUIRect();
    void SetUIRect( const sf::IntRect& pos );
    sf::Color& GetUIColor();
    void SetUIColor( const sf::Color& col );
    unsigned int& GetLayerOrder();
    void SetLayerOrder( const unsigned int& layerOrder );

    virtual void UIUpdate( const float& timeDelta ); // define in subclasses
    virtual void DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset ); // define in subclasses
private:
};

// UI Field is a rectangle area of solid visible color (a background rectangle)
class UIField : public UIElement {
public:
private:
public:
    UIField( const sf::IntRect& r, const sf::Color& c )
        { SetUIRect(r); SetUIColor(c); UIInit(); }
    void ElementInit() override;

    void UIUpdate( const float& timeDelta ) override;
    void DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset ) override;
private:
};

// UI Frame is a background made from the 'stretched' ui graphic with a pixelated border
class UIFrame : public UIElement {
public:
private:
public:
    UIFrame( const sf::IntRect& r, const sf::Color& c )
        { SetUIRect(r); SetUIColor(c); UIInit(); }
    void ElementInit() override;

    void UIUpdate( const float& timeDelta ) override;
    void DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset ) override;
private:
};

// UI Label is a text display with a particular font heading, against a particular background, either field or frame
class UILabel : public UIElement {
public:
private:
    FontHeading m_heading;
    bool m_dropShadow = false;
    std::string m_string = "";
public:
    UILabel() { }
    UILabel( const sf::IntRect& r, const sf::Color& c, const FontHeading& f, const bool& dropShadow, const std::string& s )
        { SetUIRect(r); SetUIColor(c); m_heading = f; m_dropShadow = dropShadow; m_string = s; UIInit(); }
    void ElementInit() override;

    void UIUpdate( const float& timeDelta ) override;
    void DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset ) override;
private:
};

// UI Icon is a graphic image display, against a particular background, either field or frame
class UIIcon : public UIElement {
public:
private:
    sf::Texture m_texture;
public:
    UIIcon( const sf::IntRect& r, const sf::Color& c, const sf::Texture& t )
        { SetUIRect(r); SetUIColor(c); m_texture = t; UIInit(); }
    void ElementInit() override
    {
        //
    }

    void UIUpdate( const float& timeDelta ) override
    {
        if ( !active )
            return;
    }
    void DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset ) override
    {
        if ( !visible )
            return;
    }
private:
};

// UI Button is an interactive element (accepts touch or mouse click as a press), with label or icon against field or frame
class UIButton : public UIElement {
public:
private:
    UILabel m_buttonLabel;
public:
    UIButton( const sf::IntRect& r, const sf::Color& c, const UILabel& buttonLabel )
        { SetUIRect(r); SetUIColor(c); m_buttonLabel = buttonLabel; UIInit(); }
    void ElementInit() override
    {
        //
    }

    void UIUpdate( const float& timeDelta ) override
    {
        if ( !active )
            return;
    }
    void DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset ) override
    {
        if ( !visible )
            return;
    }
private:
};

// UI Panel is a collection of labels, icons and buttons against a single background, either field or frame (base for others)
class UIPanel : public UIElement {
public:
private:
public:
    // REVIEW: consider how various elements contained with this panel are defined and initialized (pool?)
    UIPanel() { UIInit(); }
    void ElementInit() override
    {
        //
    }
private:
};

// UI HUD is a label or icon display meant to be displayed directly over the rendered viewport (usually without background)
class UIHUD : public UIPanel {
public:
private:
public:
    UIHUD() { UIInit(); }
    void ElementInit() override
    {
        //
    }
private:
};

// UI Alert is a popup panel with title label, description label and single button to confirm alert was received
class UIAlert : public UIPanel {
public:
private:
public:
    UIAlert() { UIInit(); }
    void ElementInit() override
    {
        //
    }
private:
};

// UI Confirm is a popup panel with title label, description label and more than one button to accept or cancel an action
class UIConfirm : public UIPanel {
public:
private:
public:
    UIConfirm() { UIInit(); }
    void ElementInit() override
    {
        //
    }
private:
};

// UI Menu is a collection of labels, icons and buttons within a panel, meant to become a screen separate from gameplay
class UIMenu : public UIPanel {
public:
private:
public:
    UIMenu() { UIInit(); }
    void ElementInit() override
    {
        //
    }
private:
};

// UI Manager is the primary handler of UI Element initialization, updating and drawing in sorted layers
class UIManager {
public:
private:
public:
    UIManager() { }
    ~UIManager() { }

private:
};

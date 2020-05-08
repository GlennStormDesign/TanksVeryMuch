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

enum ButtonState {
    Normal,
    Hover,
    Active,
    Disabled
};

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
    float m_scaleFactor;
public:
    UIIcon( const sf::IntRect& r, const sf::Color& c, const sf::Texture& t, const float& scale )
        { SetUIRect(r); SetUIColor(c); m_texture = t; m_scaleFactor = scale; UIInit(); }
    void ElementInit() override;

    void UIUpdate( const float& timeDelta ) override;
    void DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset ) override;
private:
};

// UI Button is an interactive element (accepts touch or mouse click as a press), with label or icon against field or frame
class UIButton : public UIElement {
public:
private:
    UILabel m_buttonLabel;
    ButtonState m_state = Normal;
public:
    UIButton( const sf::IntRect& r, const sf::Color& c, const UILabel& buttonLabel )
        { SetUIRect(r); SetUIColor(c); m_buttonLabel = buttonLabel; UIInit(); }
    void ElementInit() override;

    const ButtonState& GetState();
    void SetState( const ButtonState& state );

    void UIUpdate( const float& timeDelta ) override;
    void DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset ) override;
private:
};

// UI Box is a collection of labels and buttons against a single background, either field or frame (base for others)
// REVIEW: consider how various elements contained with this panel are defined and initialized (pool?)
class UIBox : public UIElement {
public:
protected:
    std::string m_boxTitle;
    std::string m_boxMessage;
    std::string m_boxButtonLabel;
public:
    UIBox() { UIInit(); }
    UIBox( const sf::IntRect& r, const sf::Color& c, const std::string& title, const std::string& message, const std::string& buttonLabel )
        { m_uiRect = r; m_uiColor = c; m_boxTitle = title; m_boxMessage = message; m_boxButtonLabel = buttonLabel; UIInit(); }
    void ElementInit() override;

    void UIUpdate( const float& timeDelta ) override;
    void DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset ) override;
private:
};

// UI Alert is a popup panel with title label, description label and single button to confirm alert was received
class UIAlert : public UIBox {
public:
private:
public:
    UIAlert() { UIInit(); }
    UIAlert( const sf::IntRect& r, const sf::Color& c, const std::string& title, const std::string& message, const std::string& buttonLabel )
        { m_boxTitle = title; m_boxMessage = message; m_boxButtonLabel = buttonLabel; UIInit(); }
    void ElementInit() override;

    void UIUpdate( const float& timeDelta ) override;
    void DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset ) override;
private:
};

// UI Confirm is a popup panel with title label, description label and more than one button to accept or cancel an action
class UIConfirm : public UIBox {
public:
private:
    std::string m_altButtonLabel;
    std::string m_cancelButtonLabel;
public:
    UIConfirm() { UIInit(); }
    UIConfirm( const sf::IntRect& r, const sf::Color& c, const std::string& title, const std::string& message, const std::string& buttonLabel, const std::string& altButtonLabel, const std::string& canceButtonLabel )
        { m_boxTitle = title; m_boxMessage = message; m_boxButtonLabel = buttonLabel; m_altButtonLabel = altButtonLabel; m_cancelButtonLabel = canceButtonLabel; UIInit(); }
    void ElementInit() override;

    void UIUpdate( const float& timeDelta ) override;
    void DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset ) override;
private:
};

// UI HUD is a label or icon display meant to be displayed directly over the rendered viewport (usually without background)
class UIHUD : public UIElement {
public:
private:
public:
    UIHUD() { UIInit(); }
    void ElementInit() override;

    void UIUpdate( const float& timeDelta ) override;
    void DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset ) override;
private:
};

// UI Menu is a collection of labels, icons and buttons within a panel, meant to become a screen separate from gameplay
class UIMenu : public UIElement {
public:
private:
public:
    UIMenu() { UIInit(); }
    void ElementInit() override;

    void UIUpdate( const float& timeDelta ) override;
    void DrawUI( sf::RenderWindow& window, const sf::Vector2f& uiOffset ) override;
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

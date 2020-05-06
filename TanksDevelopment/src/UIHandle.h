#pragma once

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
private:
	sf::Vector2f m_uiPos = sf::Vector2f(0.f,0.f); // relative to view pos
	// may need vector2f size as well, essentially a float rect
	unsigned int m_uiLayer = 0; // ui elements drawn last; ui depth layer sorts draw; - = behind, + = in front
public:
    UIElement() { }
    ~UIElement() { }

    sf::Vector2f& GetUIPos()
    {
        return m_uiPos;
    }
    void SetUIPos( const sf::Vector2f& pos )
    {
        m_uiPos = pos;
    }
    unsigned int& GetLayerOrder()
    {
        return m_uiLayer;
    }
    void SetLayerOrder( const unsigned int& layerOrder )
    {
        m_uiLayer = layerOrder;
    }

    virtual void UIUpdate( const float& timeDelta ) { /* define in subclasses */ }
    virtual void DrawUI( const sf::RenderWindow& window, const sf::Vector2f& viewPos ) { /* define in subclasses */ }
private:
};

class UIField : public UIElement {
public:
private:
public:
private:
};

class UILabel : public UIElement {
public:
private:
public:
private:
};

class UIIcon : public UIElement {
public:
private:
public:
private:
};

class UIButton : public UIElement {
public:
private:
public:
private:
};

class UIPanel : public UIElement {
public:
private:
public:
private:
};

class UIHUD : public UIPanel {
public:
private:
public:
private:
};

class UIAlert : public UIPanel {
public:
private:
public:
private:
};

class UIConfirm : public UIPanel {
public:
private:
public:
private:
};

class UIMenu : public UIPanel {
public:
private:
public:
private:
};

class UIManager {
public:
private:
public:
    UIManager() { }
    ~UIManager() { }

private:
};

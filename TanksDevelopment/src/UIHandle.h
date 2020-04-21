#pragma once

// UI Element Handling

// TODO: take 32x32 from UI Element image, use corners as-is, and stretch remainder to fit rect
// TODO: use fonts as header levels for labels
// TODO: UIElement manager class that sorts layer draw order
// NICETOHAVE: UIElement manager class animates position, adds vfx, animated image
// NICETOHAVE: drop shadow option for labels

class UIElement {
public:
    bool active = false; // use to skip update
    bool visible = true; // use to skip draw
private:
	sf::Vector2f m_uiPos = sf::Vector2f(0.f,0.f); // relative to view pos
	int m_uiLayer = 0; // ui elements drawn last; ui depth layer sorts draw; - = behind, + = in front
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
    int& GetLayerOrder()
    {
        return m_uiLayer;
    }
    void SetLayerOrder( const int& layerOrder )
    {
        m_uiLayer = layerOrder;
    }

    virtual void UIUpdate( const float& timeDelta ) { /* define in subclasses */ }
    virtual void DrawUI( const sf::RenderWindow& window, const sf::Vector2f& viewPos ) { /* define in subclasses */ }
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

#ifndef MainMenuState_hpp
#define MainMenuState_hpp

#include "BaseState.hpp"
#include "../../input/Binding.hpp"

class MainMenuState : public BaseState {
public:
    MainMenuState(StateManager* stateManager);
    ~MainMenuState();
    
    void onCreate() override;
    void onDestroy() override;

    void activate() override;
    void deactivate() override;

    void update(const sf::Time& time) override;
    void draw() override;

    void select(EventDetails* details);

private:
    sf::Text m_text;
    sf::Font m_font;

    sf::Vector2f m_buttonSize;
    sf::Vector2f m_buttonPos;
    sf::Uint32 m_buttonPadding;

    sf::RectangleShape m_rects[3];
    sf::Text m_labels[3];
};

#endif
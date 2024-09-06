#ifndef IntroState_hpp
#define IntroState_hpp

#include "BaseState.hpp"
#include "../../input/Binding.hpp"

#include <SFML/Graphics.hpp>

class IntroState : public BaseState {
public:
    IntroState(StateManager* stateManager);
    ~IntroState();

    void onCreate() override;
    void onDestroy() override;

    void activate() override;
    void deactivate() override;

    void update(const sf::Time& time) override;

    void draw() override;
    
    void keepGoing(EventDetails* details);

private:
    sf::Texture m_introTexture;
    sf::Sprite m_introSprite;
    sf::Text m_text;
    float m_timePassed;
};

#endif
#ifndef MainMenuState_hpp
#define MainMenuState_hpp

#include "BaseState.hpp"

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
};

#endif
#ifndef GameState_hpp
#define GameState_hpp

#include "BaseState.hpp"

class GameState : public BaseState {
public:
    GameState(StateManager* stateManager);
    ~GameState();

    void onCreate() override;
    void onDestroy() override;

    void activate() override;
    void deactivate() override;

    void update(const sf::Time& time) override;
    void draw() override;
};

#endif
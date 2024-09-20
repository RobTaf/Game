#ifndef GameState_hpp
#define GameState_hpp

#include "BaseState.hpp"
#include "../../input/Binding.hpp"
#include "../../animation/spritesheet/SpriteSheet.hpp"
#include "../../map/Map.hpp"

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

    void mainMenu(EventDetails* details);

private:
    Map* m_map;
};

#endif
#ifndef Game_hpp
#define Game_hpp

#include "Window.hpp"
#include "state/StateManager.hpp"

class Game {
public:
    Game();
    ~Game();

    void handleInput();
    void render();
    void update();
    void lateUpdate();
    Window* getWindow();

    /// @brief Measures elapsed time since the last instance of the
    /// clock being restarted, or since its creation.
    /// @return The elapsed time expressed in milliseconds.
    sf::Time getElapsedTime();
    /// @brief Unlike, the variable time-step, where the next update
    /// and draw happens as soon as the previous one is done, the
    /// fixed time-step approach will ensure that certain game logic
    /// is only happening at a provided frame rate.
    void restartClock();

private:
    SharedContext m_context;
    StateManager m_stateManager;
    EntityManager m_entityManager;
    Window m_window;
    sf::Clock m_clock;
    sf::Time m_elapsed;
};

#endif
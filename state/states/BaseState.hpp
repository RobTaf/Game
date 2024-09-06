#ifndef BaseState_hpp
#define BaseState_hpp

#include <SFML/Graphics.hpp>

class StateManager;

class BaseState {
    friend class StateManager;
public:
    BaseState(StateManager* stateManager) :
        m_stateManager(stateManager), m_transparent(false), m_transcendent(false) {}
    virtual ~BaseState();

    virtual void onCreate() = 0;
    virtual void onDestroy() = 0;

    virtual void activate() = 0;
    virtual void deactivate() = 0;

    virtual void update(const sf::Time& time) = 0;
    virtual void draw() = 0;

    void setTransparent(const bool& transparent);
    bool isTransparent();

    void setTranscendent(const bool& transcendent);
    bool isTranscendent();

    StateManager* getStateManager();

protected:
    StateManager* m_stateManager;
    /// @brief Flag that allows to draw states under the current state.
    bool m_transparent;
    /// @brief Flag that allows to update states under the current state.
    bool m_transcendent;
};

#endif
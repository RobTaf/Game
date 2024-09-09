#ifndef BaseState_hpp
#define BaseState_hpp

#include <SFML/Graphics.hpp>

class StateManager;

class BaseState {
    friend class StateManager;
public:
    BaseState(StateManager* stateManager);
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
    sf::View& getView();

protected:
    StateManager* m_stateManager;
    sf::View m_view;
    /// @brief Flag that allows to draw states under the current state.
    bool m_transparent;
    /// @brief Flag that allows to update states under the current state.
    bool m_transcendent;
};

#endif
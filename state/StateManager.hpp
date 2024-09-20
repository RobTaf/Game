#ifndef StateManager_hpp
#define StateManager_hpp

#include <vector>
#include <unordered_map>
#include <functional>
#include <utility>

#include "StateType.hpp"
#include "states/IntroState.hpp"
#include "states/MainMenuState.hpp"
#include "states/GameState.hpp"
#include "../utility/Utility.hpp"
#include "../utility/SharedContext.hpp"


using StateFactory = std::unordered_map<StateType, std::function<BaseState*(void)>>;
using StateContainer = std::vector<std::pair<StateType, BaseState*>>;
using TypeContainer = std::vector<StateType>;

class StateManager {
public:
    StateManager(SharedContext* shared);
    ~StateManager();

    void update(const sf::Time& time);
    void draw();

    void processRequests();

    SharedContext* getContext();

    bool hasState(const StateType& type);
    void switchTo(const StateType& type);
    void remove(const StateType& type);
private:
    void createState(const StateType& type);
    void removeState(const StateType& type);

    template<class T>
    void registerState(const StateType& type) {
        m_stateFactory[type] = [this]() -> BaseState* {
            return new T(this);
        };
    }

    SharedContext* m_shared;
    StateContainer m_states;
    TypeContainer m_toRemove;
    StateFactory m_stateFactory;
};

#endif
#include "GameState.hpp"
#include "../StateManager.hpp"

GameState::GameState(StateManager* stateManager) : BaseState(stateManager) {

}

GameState::~GameState()
{
}

void GameState::onCreate() {

}

void GameState::onDestroy()
{
}

void GameState::activate()
{
}

void GameState::deactivate()
{
}

void GameState::update(const sf::Time &time)
{
}

void GameState::draw()
{
}

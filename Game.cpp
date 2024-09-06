#include "Game.hpp"

Game::Game() : m_window("Game", sf::Vector2u(800, 600)), m_stateManager(&m_context) {
    m_context.m_window = &m_window;
    m_context.m_eventManager = m_window.getEventManager();
    m_stateManager.switchTo(StateType::Intro);
}

Game::~Game() {}

void Game::update() {
    m_window.update();
    m_stateManager.update(m_elapsed);
}

void Game::lateUpdate() {
    m_stateManager.processRequests();
    restartClock();
}

void Game::render() {
    m_window.beginDraw();
    m_stateManager.draw();
    m_window.endDraw();
}

sf::Time Game::getElapsedTime() {
    return m_elapsed;
}

void Game::restartClock() {
    m_elapsed += m_clock.restart();
}

Window* Game::getWindow() {
    return &m_window;
}

int main(int argc, char* argv[]) {
    Game game;
    while (not game.getWindow()->isDone()) {
        game.update();
        game.render();
        game.lateUpdate();
        sf::sleep(sf::seconds(0.2));
    }
    return 0;
}
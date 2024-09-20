#include "GameState.hpp"
#include "../StateManager.hpp"

GameState::GameState(StateManager* stateManager): BaseState(stateManager) {

}

GameState::~GameState()
{
}

void GameState::onCreate() {
    EventManager* eventManager = m_stateManager->getContext()->m_eventManager;
    eventManager->addCallback(StateType::Game, "Key_Escape", &GameState::mainMenu, this);

    sf::Vector2u size = m_stateManager->getContext()->m_window->windowSize();
    m_view.setSize(size.x, size.y);
    m_view.setCenter(size.x / 2, size.y / 2);
    m_view.zoom(0.6f);

    m_stateManager->getContext()->m_window->getRenderWindow()->setView(m_view);
    // m_stateManager->getContext()->m_entityManager->add(EntityType::Player);

    m_map = new Map(m_stateManager->getContext(), this);
    m_map->loadMap("Level1");
}

void GameState::onDestroy() {
    EventManager* eventManager = m_stateManager->getContext()->m_eventManager;
    eventManager->removeCallback(StateType::Game, "Key_Escape");

    delete m_map;
    m_map = nullptr;
}

void GameState::activate()
{
}

void GameState::deactivate()
{
}

void GameState::update(const sf::Time &time) {
    SharedContext* context = m_stateManager->getContext();
    // BaseEntity* player = context->m_entityManager->find("Player");

    // if (not player) {
    //     std::cout << "Respawning player..." << std::endl;
    //     context->m_entityManager->add(EntityType::Player, "Player");
    //     player = context->m_entityManager->find("Player");
    //     player->setPosition(m_map->getPlayerStart());
    // } else {
    //     m_view.setCenter(player->getPosition());
    //     context->m_window->getRenderWindow()->setView(m_view);
    // }
    sf::Vector2f pos = sf::Vector2f(0, 0);
    m_view.setCenter(pos);
    context->m_window->getRenderWindow()->setView(m_view);
    
    sf::FloatRect viewSpace = context->m_window->viewSpace();
    if (viewSpace.left <= 0) {
        m_view.setCenter(viewSpace.width / 2, m_view.getCenter().y);
        context->m_window->getRenderWindow()->setView(m_view);
    } else if (viewSpace.left + viewSpace.width > (m_map->getMapSize().x + 1) * Sheet::TileSize) {
        m_view.setCenter(((m_map->getMapSize().x + 1) * Sheet::TileSize) - (viewSpace.width / 2),  m_view.getCenter().y);
        context->m_window->getRenderWindow()->setView(m_view);
    }
    m_map->update(time.asSeconds());  
    // m_stateManager->getContext()->m_entityManager->update(time.asSeconds());

    // std::cout << "player: " << m_stateManager->getContext()->m_entityManager->find(0)->getPosition().x << " " << m_stateManager->getContext()->m_entityManager->find(0)->getPosition().y << std::endl;
    // std::cout << "state: " << static_cast<int>(m_stateManager->getContext()->m_entityManager->find(0)->getState()) << std::endl;
    // std::cout << "view: "<< m_view.getCenter().x << " " << m_view.getCenter().y << std::endl;
}

void GameState::draw() {
    m_map->draw();
    // m_stateManager->getContext()->m_entityManager->draw();
}

void GameState::mainMenu(EventDetails* details) {
    m_stateManager->switchTo(StateType::MainMenu);
}

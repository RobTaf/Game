#include "IntroState.hpp"
#include "../StateManager.hpp"

IntroState::IntroState(StateManager* stateManager) : BaseState(stateManager) 
{
}

IntroState::~IntroState()
{
}

void IntroState::onCreate() {
    m_timePassed = 0.0f;

    sf::Vector2u windowSize = m_stateManager->getContext()->m_window->getRenderWindow()->getSize();
    m_introTexture.loadFromFile("source/assets/river.jpg");
    m_introSprite.setTexture(m_introTexture);
    m_introSprite.setOrigin(m_introTexture.getSize().x / 2.0f, m_introTexture.getSize().y / 2.0f);
    m_introSprite.setPosition(windowSize.x / 2.0f, 0);

    EventManager* eventManager = m_stateManager->getContext()->m_eventManager;
    eventManager->addCallback(StateType::Intro, "Intro_Continue", &IntroState::goOn, this);
}

void IntroState::onDestroy() {
    EventManager* eventManager = m_stateManager->getContext()->m_eventManager;
    eventManager->removeCallback(StateType::Intro, "Intro_Continue");
}

void IntroState::activate()
{
}

void IntroState::deactivate()
{
}

void IntroState::update(const sf::Time &time) {
    if (m_timePassed < 5.0f) {
        m_timePassed += time.asSeconds();
        m_introSprite.setPosition(m_introSprite.getPosition().x, m_introSprite.getPosition().y + (48 * time.asSeconds()));
    }
}

void IntroState::draw() {
    sf::RenderWindow* renderWindow = m_stateManager->getContext()->m_window->getRenderWindow();
    renderWindow->draw(m_introSprite);
}

void IntroState::goOn(EventDetails* details) {
    if (m_timePassed > 0.5f) {
        m_stateManager->switchTo(StateType::MainMenu);
        m_stateManager->remove(StateType::Intro);
    }
}

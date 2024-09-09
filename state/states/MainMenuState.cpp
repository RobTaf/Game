#include "MainMenuState.hpp"
#include "../StateManager.hpp"

MainMenuState::MainMenuState(StateManager *stateManager) : BaseState(stateManager)
{
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::onCreate() {
    m_font.loadFromFile("source/assets/fonts/BebasNeue-Regular.ttf");
    m_text.setFont(m_font);
    m_text.setString(sf::String("MAIN MENU"));
    m_text.setCharacterSize(18);

    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    m_text.setPosition(400, 100);

    m_buttonSize = sf::Vector2f(300.0f, 32.0f);
    m_buttonPos = sf::Vector2f(400, 200);
    m_buttonPadding = 4;

    std::string str[3] = {"PLAY", "SHOP", "EXIT"};

    for (int i = 0; i < 3; i++) {
        sf::Vector2f buttonPosition(m_buttonPos.x, m_buttonPos.y + i * (m_buttonSize.y + m_buttonPadding));
        m_rects[i].setSize(m_buttonSize);
        m_rects[i].setFillColor(sf::Color::Red);

        m_rects[i].setOrigin(m_buttonSize.x / 2.0f, m_buttonSize.y / 2.0f);
        m_rects[i].setPosition(buttonPosition);

        m_labels[i].setFont(m_font);
        m_labels[i].setString(sf::String(str[i]));
        m_labels[i].setCharacterSize(12);

        sf::FloatRect rect = m_labels[i].getLocalBounds();
        m_labels[i].setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
        m_labels[i].setPosition(buttonPosition);
    }

    EventManager* eventManager = m_stateManager->getContext()->m_eventManager;
    eventManager->addCallback(StateType::MainMenu, "Mouse_Left", &MainMenuState::select, this);

}

void MainMenuState::onDestroy() {
    EventManager* eventManager = m_stateManager->getContext()->m_eventManager;
    eventManager->removeCallback(StateType::MainMenu, "Mouse_Left");
}

void MainMenuState::activate() {
    // If the game state is on the stack, that means the user
    // paused the game
    if (m_stateManager->hasState(StateType::Game) and m_labels[0].getString() == "PLAY") {
        m_labels[0].setString(sf::String("RESUME"));
        sf::FloatRect rect = m_labels[0].getLocalBounds();
        m_labels[0].setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
    }
}

void MainMenuState::deactivate() 
{
}

void MainMenuState::update(const sf::Time &time)
{
}

void MainMenuState::draw() {
    sf::RenderWindow* window = m_stateManager->getContext()->m_window->getRenderWindow();
    window->draw(m_text);
    for (int i = 0; i < 3; i++) {
        window->draw(m_rects[i]);
        window->draw(m_labels[i]);
    }
}

void MainMenuState::select(EventDetails* details) {
    sf::Vector2i clickPos = details->m_mouse;

    float halfX = m_buttonSize.x / 2.0f;
    float halfY = m_buttonSize.y / 2.0f;

    for (int i = 0; i < 3; i++) {
        if(clickPos.x >= m_rects[i].getPosition().x - halfX &&
            clickPos.x <= m_rects[i].getPosition().x + halfX &&
            clickPos.y >= m_rects[i].getPosition().y - halfY &&
            clickPos.y <= m_rects[i].getPosition().y + halfY) {
                
                if (i == 0) {
                    m_stateManager->switchTo(StateType::Game);
                } else if (i == 1) {
                    // m_stateManager->switchTo(StateType::Credits);
                } else if (i == 2) {
                    m_stateManager->getContext()->m_window->close(details);
                }
        }
    }
}

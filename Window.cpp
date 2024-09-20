#include "Window.hpp"

Window::Window() {
    setup("Window", sf::Vector2u(640, 480));
}

Window::~Window() {
    destroy();
}

Window::Window(std::string title, sf::Vector2u size) {
    setup(title, size);
}

void Window::setup(std::string title, sf::Vector2u size)
{
    m_title = title;
    m_size = size;
    m_isFullscreen = false;
    m_isDone = false;
    m_isFocused = true;

    m_eventManager.addCallback(StateType(0), "Toggle_Fullscreen", &Window::toggleFullscreen, this);
    m_eventManager.addCallback(StateType(0), "Window_Close", &Window::close, this);
    create();
}

void Window::create() {
    auto style = (m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
    m_window.create( {m_size.x, m_size.y}, m_title, style );
}

void Window::destroy() {
    m_window.close();
}

void Window::close(EventDetails* details = nullptr) {
    m_isDone = true;
}

void Window::toggleFullscreen(EventDetails* details = nullptr) {
    m_isFullscreen = !m_isFullscreen;
    destroy();
    create();
}

void Window::update() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            close();
        } else if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::F5) {
            toggleFullscreen();
        } else if (event.type == sf::Event::LostFocus) {
            m_isFocused = false;
            m_eventManager.setFocus(false);
        } else if (event.type == sf::Event::GainedFocus) {
            m_isFocused = true;
            m_eventManager.setFocus(true);
        }
        m_eventManager.handleEvent(event);
    }
    m_eventManager.update();
}

void Window::beginDraw() {
    m_window.clear(sf::Color::Black);
}

void Window::endDraw() {
    m_window.display();
}

bool Window::isDone() {
    return m_isDone;
}

bool Window::isFullscreen() {
    return m_isFullscreen;
}

sf::Vector2u Window::windowSize() {
    return m_size;
}

sf::FloatRect Window::viewSpace() {
    // Reference system is not {(1 0), (0 1)}, rather {(1 0), (0 -1)}
    sf::Vector2f viewCenter = m_window.getView().getCenter();
    sf::Vector2f viewSize = m_window.getView().getSize();
    sf::Vector2f halfViewSize(viewSize.x / 2, viewSize.y / 2);
    return sf::FloatRect(viewCenter - halfViewSize, viewSize);
}

void Window::draw(sf::Drawable& drawable) {
    m_window.draw(drawable);
}

EventManager* Window::getEventManager() {
    return &m_eventManager;
}

TextureManager* Window::getTextureManager() {
    return &m_textureManager;
}

sf::RenderWindow* Window::getRenderWindow() {
    return &m_window;
}

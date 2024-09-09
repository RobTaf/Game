#ifndef Window_hpp
#define Window_hpp

#include <SFML/Graphics.hpp>
#include "input/EventManager.hpp"

class Window {
public:
    Window();
    Window(const std::string title, sf::Vector2u size);
    ~Window();

    void beginDraw();
    void endDraw();

    void update();

    bool isDone();
    bool isFullscreen();
    sf::Vector2u windowSize();
    sf::FloatRect viewSpace();

    void Draw(sf::Drawable& drawable);

    EventManager* getEventManager();
    sf::RenderWindow* getRenderWindow();

    void close(EventDetails* details);

    // Triggerable actions
    void toggleFullscreen(EventDetails* details);

private:
    void setup(std::string title, sf::Vector2u size);
    void destroy();
    void create();

    EventManager m_eventManager;
    sf::RenderWindow m_window;
    sf::Vector2u m_size;
    std::string m_title;
    bool m_isDone;
    bool m_isFocused;
    bool m_isFullscreen;
};

#endif
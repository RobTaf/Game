#ifndef BaseComponent_hpp
#define BaseComponent_hpp

#include <sstream>

enum class Component {
    Transform = 0, SpriteSheet, State, Movable, Collidable, Controller, Sound
};

class BaseComponent {
public:
    BaseComponent(const Component& componentType);
    virtual ~BaseComponent();

    Component getType();

    virtual void readIn(std::stringstream& stream) = 0;
    friend std::stringstream& operator >>(std::stringstream& stream, BaseComponent& component);

protected:
    Component m_type;
};

#endif
#ifndef Transform_hpp
#define Transform_hpp

#include <SFML/Graphics.hpp>
#include "BaseComponent.hpp"

class Transform: public BaseComponent {
public:
    Transform();
    ~Transform();

    void readIn(std::stringstream& stream) override;

    const sf::Vector2f& getPosition();
    const sf::Vector2f& getOldPosition();
    void setPosition(const sf::Vector2f& position);
    void setPosition(float x, float y);

    const sf::Vector2f& getRotation();
    void setRotation(const sf::Vector2f& rotation);
    void setRotation(float x, float y);

    const sf::Vector2f& getScale();
    void setScale(const sf::Vector2f& scale);
    void setScale(float x, float y);

    void moveBy(float x, float y);
    void moveBy(const sf::Vector2f& vector);

    void rotateBy(float x, float y);
    void rotateBy(const sf::Vector2f& vector);

    void scaleBy(float x, float y);
    void scaleBy(const sf::Vector2f& vector);


private:
    /// @brief Position of the object in the x and y coordinates.
    sf::Vector2f m_position;
    /// @brief Old position of the object in the x and y coordinates.
    sf::Vector2f m_positionOld;
    /// @brief Rotation of the object around the x-axis and y-axis,
    /// measured in degrees.
    sf::Vector2f m_rotation;
    /// @brief Scale of the object along the x-axis and y-axis. 
    /// The value "1" is the original size.
    sf::Vector2f m_scale;
};

#endif
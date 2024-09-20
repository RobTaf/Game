#include "BaseComponent.hpp"

BaseComponent::BaseComponent(const Component &componentType): m_type(componentType)
{
}

BaseComponent::~BaseComponent()
{
}

Component BaseComponent::getType() {
    return m_type;
}

std::stringstream &operator>>(std::stringstream& stream, BaseComponent& component) {
    component.readIn(stream);
    return stream;
}

#ifndef DirectionalAnimation_hpp
#define DirectionalAnimation_hpp

#include "BaseAnimation.hpp"

class DirectionalAnimation: public BaseAnimation {
protected:
    void frameStep() override;
    void cropSprite() override;
    void readIn(std::stringstream& stream);
};

#endif
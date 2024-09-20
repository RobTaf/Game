#ifndef EntityEvent_hpp
#define EntityEvent_hpp

/// @brief To handle inter-system communication, every system listens
/// for these events in order to perform their own logic and without
/// them knowing anything about how the other one works.
enum class EntityEvent {
    Spawned, Despawned, CollidingOnX, CollidingOnY,
    MovingLeft, MovingRight, MovingUp, MovingDown,
    BecameIdle, BeganMoving, GotHurt
};

#endif
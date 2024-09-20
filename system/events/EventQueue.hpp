#ifndef EventQueue_hpp
#define EventQueue_hpp

#include <queue>

using EventID = unsigned int;

/// @brief The event queue is a programming pattern, which
/// is used to decouple when an event is fired from when
/// it's actually processed. The data that is pushed onto
/// it earliest is removed first. Its events are processed
/// at a completely different time in relation to them being
/// added.
class EventQueue {
public:
    void addEvent(const EventID& id);
    bool processEvents(EventID& id);
    void clearEvents();

private:
    std::queue<EventID> m_queue;
};

#endif
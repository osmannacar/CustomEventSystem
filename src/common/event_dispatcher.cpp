#include "event_dispatcher.h"

EventDispatcher::EventDispatcher()
    : running(true)
{}

EventDispatcher::~EventDispatcher() {
    running = false;
    queueCondition.notify_all(); // Notify all threads waiting on the condition
}

void EventDispatcher::postEvent(const Event& event) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        eventQueue.push(event);
    }
    queueCondition.notify_one(); // Notify one waiting thread
}

void EventDispatcher::registerHandler(Event::Type type, std::function<void (const Event &)> handler)
{
    handlerContainer[type] = handler;
}

void EventDispatcher::startEventloop() {
    while (running.load()) {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCondition.wait(lock, [this]() { return !eventQueue.empty() || !running; });

        if (!running) break; // Exit if dispatcher is not running

        while (!eventQueue.empty()) {
            Event event = eventQueue.front();
            eventQueue.pop();
            lock.unlock();
            auto it = handlerContainer.find(event.type);
            if (it != handlerContainer.end()) {
                it->second(event); // Call the handler function
            } else {
                std::cerr << "No handler registered for this event type!" << std::endl;
            }
            lock.lock();
        }
    }
}

void EventDispatcher::shutdownEventloop()
{
    handlerContainer.clear();
    running.store(false);
}

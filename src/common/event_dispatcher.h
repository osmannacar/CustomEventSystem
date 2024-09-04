#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include <opencv2/opencv.hpp>

/*!
 * \brief Represents an event with a type and associated data.
 * \details The Event class encapsulates information about an event, including
 * its type and any associated data. This data is typically a pair of OpenCV matrices
 * representing original and processed images.
 */
class Event {
public:
    /*!
     * \brief Enum to define different types of events.
     */
    enum class Type {
        InitialState,            ///< Event indicating initial state.
        FrameCaptureReady,       ///< Event indicating that frame capture is ready.
        FrameDefoggerReady,      ///< Event indicating that frame defogger is ready.
        FrameDetectionReady      ///< Event indicating that frame detection is ready.
    };

    /*!
     * \brief Constructs an Event with a specified type and associated data.
     * \param type The type of the event.
     * \param data A pair of OpenCV matrices representing the original and processed images.
     */
    Event(Type type, std::pair<cv::Mat, cv::Mat> data) : type(type), data(data) {}

    Type type;                ///< Type of the event.
    std::pair<cv::Mat, cv::Mat> data; ///< Pair of OpenCV matrices for event data (original and processed images).
};

/*!
 * \brief Manages event dispatching and handling.
 * \details The EventDispatcher class is responsible for posting events, registering
 * handlers for different event types, and running the event loop. It maintains a queue
 * of events, handles thread synchronization, and ensures events are processed and dispatched
 * to the appropriate handlers.
 */
class EventDispatcher {
public:
    /*!
     * \brief Constructs an EventDispatcher object.
     * \details Initializes the EventDispatcher, preparing it for use in posting and handling events.
     */
    EventDispatcher();

    /*!
     * \brief Destroys the EventDispatcher object.
     * \details Cleans up resources associated with the EventDispatcher and ensures a clean shutdown.
     */
    ~EventDispatcher();

    /*!
     * \brief Posts an event to the dispatcher.
     * \param event The event to be posted.
     * \details Adds an event to the event queue, making it available for processing by
     * the event loop.
     */
    void postEvent(const Event& event);

    /*!
     * \brief Registers or updates a handler for a specific event type.
     * \param type The type of event for which the handler is being registered.
     * \param handler The function to handle events of the specified type.
     * \details Associates a function with a specific event type. When an event of that type
     * is posted, the registered handler will be invoked to process the event.
     */
    void registerHandler(Event::Type type, std::function<void(const Event&)> handler);

    /*!
     * \brief Starts the event loop.
     * \details Begins processing events by entering the event loop. This loop continuously
     * checks for new events and dispatches them to the appropriate handlers.
     */
    void startEventloop();

    /*!
     * \brief Shuts down the event loop.
     * \details Stops the event loop and performs necessary cleanup. This method ensures
     * that the event loop exits gracefully and all resources are properly released.
     */
    void shutdownEventloop();

private:
    /*!
    * \brief Flag indicating whether the event loop is currently running.
    * \details This atomic boolean variable is used to control the lifecycle of the event loop, allowing safe and thread-safe checks and updates of the running state.
    */
    std::atomic<bool> running;

    /*!
    * \brief Queue for storing events to be processed.
    * \details This queue holds events that need to be processed. Events are enqueued by various parts of the system and dequeued by the event loop for handling.
    */
    std::queue<Event> eventQueue;

    /*!
    * \brief Mutex for synchronizing access to the event queue.
    * \details This mutex ensures that access to the event queue is thread-safe, preventing race conditions and ensuring that only one thread can modify or read the queue at a time.
    */
    std::mutex queueMutex;

    /*!
    * \brief Condition variable for waiting and notifying about new events.
    * \details This condition variable allows threads to wait efficiently until new events are available in the queue, and to notify other threads when events are added.
    */
    std::condition_variable queueCondition;

    /*!
    * \brief Container for event handlers.
    * \details This map associates event types with their corresponding handler functions. It is used to register and dispatch handlers for different types of events.
    */
    std::map<Event::Type, std::function<void(const Event&)>> handlerContainer;
};

#endif // EVENTDISPATCHER_H

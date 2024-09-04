#ifndef IPROCESSOR_H
#define IPROCESSOR_H

#include <thread>
#include <atomic>
#include "event_dispatcher.h"

class IProcessor {
public:
    /*!
     * \brief Constructs an IProcessor with the given EventDispatcher.
     * \param dispatcher Reference to an EventDispatcher used for event handling.
     */
    IProcessor(EventDispatcher &dispatcher);

    /*!
     * \brief Starts processing.
     * \details Initiates or activates the necessary processes for the component to perform its tasks.
     * This may involve setting up resources, starting background threads, or beginning data processing operations.
     */
    void start();

    /*!
     * \brief Stops processing and cleans up resources.
     * \details Halts or deactivates ongoing processes, such as background threads or data processing operations.
     * This may involve releasing resources or performing necessary cleanup tasks to ensure a graceful shutdown.
     */
    void stop();

    /*!
     * \brief Handles an event.
     * \param event The event to be processed.
     * \details Processes and responds to the given event object. The specific handling logic depends on the event type
     * and the requirements of the component. The `event` parameter contains details about the event that needs to be processed.
     */
    void handleEvent(const Event& event);

protected:
    /*!
     * \brief Gets the type of events the derived class can handle.
     * \return The type of events that are relevant or accessible to this class.
     * \details Provides information about the type of events that this class can process, returning an enumeration
     * value from the Event::Type enum to indicate the specific event type.
     */
    virtual Event::Type getAccessibleType() = 0;

    /*!
     * \brief Processes events in the derived class.
     * \details This method should be implemented by derived classes to handle or process events based on specific requirements.
     */
    virtual void processEvents() = 0;

    /*!
     * \brief Retrieves the thread associated with the derived class.
     * \return A std::thread object representing the thread used for executing tasks or processing.
     * \details Returns a std::thread object that can be moved to the caller if necessary, representing the thread used by the derived class.
     */
    virtual std::thread getThreadInfo() = 0;

protected:
    /*!
     * \brief Reference to the EventDispatcher used for event management.
     * \details This reference is used to communicate between different components or threads by dispatching events as needed.
     */
    EventDispatcher& dispatcher;

    /*!
     * \brief Thread handling background processing tasks.
     * \details This thread is responsible for executing tasks concurrently, such as processing frames or performing long-running operations.
     */
    std::thread workerThread;

    /*!
     * \brief Atomic flag indicating whether the component is running.
     * \details This flag is used to control the component's lifecycle, allowing safe and thread-safe checks and updates of the running state.
     */
    std::atomic<bool> running;

    /*!
     * \brief Queue to hold frames for processing.
     * \details This queue stores pairs of OpenCV matrices, where each pair represents two frames (e.g., current and previous) to be processed.
     * Frames are processed in the order they are added to the queue.
     */
    std::queue<std::pair<cv::Mat, cv::Mat>> frameQueue;

    /*!
     * \brief Mutex for synchronizing access to the frameQueue.
     * \details Ensures thread-safe access to the frameQueue, preventing race conditions and ensuring only one thread can modify or read
     * the queue at a time.
     */
    std::mutex queueMutex;

    /*!
     * \brief Condition variable for synchronizing threads waiting for frames.
     * \details Allows threads to efficiently wait until frames are available in the frameQueue, avoiding busy-waiting and reducing CPU usage.
     */
    std::condition_variable queueCondition;
};

#endif // IPROCESSOR_H

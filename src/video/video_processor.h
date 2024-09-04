#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <thread>
#include <atomic>
#include "iprocessor.h"

/*!
 * \brief Processes video frames from a specified video file.
 * \details The `VideoProcessor` class inherits from `IProcessor` and is responsible for handling video frame extraction and processing using the OpenCV library. It integrates with the `EventDispatcher` to manage events related to video processing tasks.
 */
class VideoProcessor : public IProcessor {
public:
    /*!
     * \brief Constructs a VideoProcessor with the given video path and EventDispatcher.
     * \param videoPath The path to the video file to be processed.
     * \param dispatcher Reference to an EventDispatcher used for event handling.
     */
    VideoProcessor(const std::string& videoPath, EventDispatcher& dispatcher);

    /*!
     * \brief Destroys the VideoProcessor object.
     * \details Cleans up resources and performs necessary shutdown tasks when the VideoProcessor object is destroyed.
     */
    ~VideoProcessor();

protected:
    /*!
     * \brief Processes events related to video processing.
     * \details This method overrides the `processEvents` method from `IProcessor` to handle specific events related to video frame processing.
     */
    void processEvents() override;

    /*!
     * \brief Returns the type of events the VideoProcessor can handle.
     * \return The type of events relevant to the VideoProcessor.
     * \details Overrides the `getAccessibleType` method from `IProcessor` to specify the event types that the VideoProcessor can process.
     */
    Event::Type getAccessibleType() override;

    /*!
     * \brief Retrieves the thread associated with video processing tasks.
     * \return A std::thread object representing the thread used for video processing.
     * \details Overrides the `getThreadInfo` method from `IProcessor` to provide details about the processing thread.
     */
    std::thread getThreadInfo() override;

private:
    /*!
     * \brief Path to the video file to be processed.
     * \details Stores the file path of the video from which frames will be extracted and processed.
     */
    std::string videoPath;
};

#endif // VIDEOPROCESSOR_H

#include "video_processor.h"
#include <iostream>

VideoProcessor::VideoProcessor(const std::string& videoPath, EventDispatcher& dispatcher)
    : IProcessor(dispatcher)
    , videoPath(videoPath)
{

}

VideoProcessor::~VideoProcessor() {
    stop();
}

void VideoProcessor::processEvents() {
    cv::VideoCapture capture(videoPath);
    if (!capture.isOpened()) {
        std::cerr << "Error: Could not open video file.\n";
        return;
    }

    double fps = capture.get(cv::CAP_PROP_FPS);

    cv::Mat frame;
    while (running.load()) {
        if (!capture.read(frame)) {
            // Reset to the beginning of the video
            capture.set(cv::CAP_PROP_POS_FRAMES, 0);
            continue; // Start reading frames from the beginning again
        }

        dispatcher.postEvent(Event(Event::Type::FrameCaptureReady, std::make_pair(frame, frame)));
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000 / fps))); // Adjust sleep duration as needed
        frame.release();
    }
    capture.release();
}

Event::Type VideoProcessor::getAccessibleType()
{
    return Event::Type::InitialState;
}

std::thread VideoProcessor::getThreadInfo()
{
    return std::thread(&VideoProcessor::processEvents, this);
}
